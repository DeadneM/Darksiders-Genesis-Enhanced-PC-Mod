// Compile the real services and their ownership registry. Only the native
// setters/clock context are substituted; no hooks are installed by this test.
#include "../src/LocalPlayerOwnership.cpp"
#include "../src/ManualGaugeFeature.cpp"
#include "../src/ResourceGainFeature.cpp"
#include "../src/AmmoConsumptionFeature.cpp"
#include "../src/PotionFeature.cpp"
#include "../src/HealthFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace dg::native_tps {
bool OnGameThread(){return true;}
bool CinematicActive(unsigned){return false;}
bool IsWorldPaused(void*){return false;}
}
namespace {
void Check(bool v,const char* message){if(!v)throw std::runtime_error(message);}
template<class T> void Put(void* object,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(value));}
template<class T> T Get(void* object,size_t offset){T value{};std::memcpy(&value,static_cast<unsigned char*>(object)+offset,sizeof(value));return value;}
struct Object {alignas(16) unsigned char bytes[0x1100]{};void* Ptr(){return bytes;}};
struct NativeClass {Object object;void* tree[2]{};};
void Type(Object& object,NativeClass& type){Put(object.Ptr(),0x10,type.object.Ptr());}
void Class(NativeClass& type,int index,NativeClass* parent=nullptr){
    type.tree[index]=type.object.bytes+0x30;
    if(parent)type.tree[0]=parent->object.bytes+0x30;
    Put(type.object.Ptr(),0x30,type.tree+0);Put(type.object.Ptr(),0x38,index);
}
float Wrath(void* p,float v){Put(p,0x3A0,v);return v;}
float Ultimate(void* p,float v){Put(p,0x3A8,v);return v;}
float Synergy(void* p,float v){Put(p,0x5A4,v);return v;}
int Ammo(void* p,int v){Put(p,0x34C,v);return v;}
int MaxAmmo(void*){return 10;}
int MaxPotions(void*){return 3;}
float Threshold(void*){return 10;}
void AddPotions(void* p,int add){auto* data=Get<void*>(p,0x580);Put(data,0xC,Get<int>(data,0xC)+add);}
int Consume(void*,int amount){return amount;}
float MaxHealth(void*,bool){return 100;}
float SetHealth(void* p,float v){Put(p,0x37C,v);return v;}
void Run(){
    std::vector<unsigned char> image(0x3900000);
    const auto base=reinterpret_cast<uintptr_t>(image.data());
    NativeClass resourceType{},proxyType{},stateBase{},stateType{};
    Class(resourceType,0);Class(proxyType,0);Class(stateBase,0);Class(stateType,1,&stateBase);
    Put(image.data(),0x3810268,resourceType.object.Ptr());Put(image.data(),0x3811D40,proxyType.object.Ptr());
    Put(image.data(),0x38115E0,stateBase.object.Ptr());Put(image.data(),0x38115D8,stateType.object.Ptr());
    Object actors[3]{},resources[2]{},states[2]{},proxies[2]{},potionData[2]{};
    void* arrays[2][1]={{proxies[0].Ptr()},{proxies[1].Ptr()}};
    auto& rt=dg::runtime::Get();
    rt.unlimitedWrathEnabled=false;rt.unlimitedUltimateEnabled=false;rt.unlimitedSynergyEnabled=false;
    rt.unlimitedAmmoEnabled=false;rt.unlimitedPotionsEnabled=false;
    for(unsigned i=0;i<2;++i){
        Type(resources[i],resourceType);Type(proxies[i],proxyType);Type(states[i],stateType);
        Put(resources[i].Ptr(),0x118,static_cast<unsigned char>(3));Put(proxies[i].Ptr(),0x118,static_cast<unsigned char>(3));
        Put(states[i].Ptr(),0x118,static_cast<unsigned char>(3));
        Put(actors[i].Ptr(),0xEA0,resources[i].Ptr());Put(actors[i].Ptr(),0x330,states[i].Ptr());
        Put(states[i].Ptr(),0x5C0,actors[i].Ptr());Put(states[i].Ptr(),0x498,100.0f);
        Put(resources[i].Ptr(),0x3A4,100.0f);Put(resources[i].Ptr(),0x3AC,100.0f);
        Put(resources[i].Ptr(),0x428,arrays[i]+0);Put(resources[i].Ptr(),0x430,1);
        Put(proxies[i].Ptr(),0x108,resources[i].Ptr());Put(proxies[i].Ptr(),0x34C,10);
        Put(states[i].Ptr(),0x580,potionData[i].Ptr());
        dg::local_players::Context c{};c.index=int(i);c.localCount=2;c.localPlayer=100+i;
        c.character=reinterpret_cast<uintptr_t>(actors[i].Ptr());c.playerState=reinterpret_cast<uintptr_t>(states[i].Ptr());
        dg::local_players::Publish(c);
    }
    dg::manual_gauges::g_base=base;dg::manual_gauges::g_enabled=true;
    dg::manual_gauges::g_wrath=Wrath;dg::manual_gauges::g_chaos=Ultimate;dg::manual_gauges::g_synergy=Synergy;
    dg::resource_gain::g_base=base;dg::resource_gain::g_ready[0]=true;dg::resource_gain::g_ready[1]=true;
    dg::resource_gain::g_original[0]=Wrath;dg::resource_gain::g_original[1]=Ultimate;
    dg::ammo::g_base=base;dg::ammo::g_ready=true;dg::ammo::g_original=Ammo;dg::ammo::g_maximum=MaxAmmo;
    dg::potions::g_base=base;dg::potions::g_ready=true;dg::potions::g_threshold=Threshold;
    dg::potions::g_maximum=MaxPotions;dg::potions::g_add=AddPotions;dg::potions::g_consume=Consume;
    dg::health::g_base=base;dg::health::g_ready=true;dg::health::g_original=SetHealth;dg::health::g_maximum=MaxHealth;
    for(unsigned i=0;i<2;++i){
        const auto generation=dg::local_players::PublishedSlot(i).generation;
        dg::manual_gauges::OnLivePlayer(actors[i].Ptr(),generation,states[i].Ptr());
        dg::resource_gain::OnLivePlayer(actors[i].Ptr(),generation);
        dg::ammo::OnLivePlayer(actors[i].Ptr(),generation);
        dg::potions::OnLivePlayer(actors[i].Ptr(),generation,states[i].Ptr());
        Put(resources[i].Ptr(),0x37C,50.0f);dg::health::OnLivePlayer(actors[i].Ptr(),generation);
    }
    Check(dg::manual_gauges::GetTelemetry(0).readyMask==7&&dg::manual_gauges::GetTelemetry(1).readyMask==7,"both players own three independent gauges");
    Check(dg::manual_gauges::RequestSet(7,50),"all-gauge request queues");
    for(unsigned i=0;i<2;++i)dg::manual_gauges::OnLivePlayer(actors[i].Ptr(),dg::local_players::PublishedSlot(i).generation,states[i].Ptr());
    Check(Get<float>(resources[0].Ptr(),0x3A0)==50&&Get<float>(resources[1].Ptr(),0x3A0)==50&&Get<float>(states[1].Ptr(),0x5A4)==50,"one request fills P1 and P2 separately");
    rt.wrathChargeEnabled=true;rt.wrathChargeMultiplier=4;
    Check(dg::resource_gain::Set(0,resources[1].Ptr(),51,0x6EA9C9)==54,"P2 reward uses native positive-gain policy");
    Check(Get<float>(resources[0].Ptr(),0x3A0)==50,"P2 reward cannot write P1");
    rt.reduceAmmoEnabled=true;rt.ammoConsumptionDivisor=2;
    Check(dg::ammo::Set(proxies[0].Ptr(),9,0x697D92)==10,"P1 first half-cost shot retains its credit");
    Check(dg::ammo::Set(proxies[1].Ptr(),9,0x697D92)==10,"P2 starts its own fractional credit");
    Check(dg::ammo::Set(proxies[0].Ptr(),9,0x697D92)==9&&Get<int>(proxies[1].Ptr(),0x34C)==10,"P1 second shot cannot consume P2 credit");
    Check(dg::potions::RequestRefill(),"potion refill queues both native holders");
    // Bypass only the sample throttle; request dispatch and native caps stay real.
    for(unsigned i=0;i<2;++i){dg::potions::g_players[i].observed=0;dg::potions::OnLivePlayer(actors[i].Ptr(),dg::local_players::PublishedSlot(i).generation,states[i].Ptr());}
    Check(Get<int>(potionData[0].Ptr(),0xC)==3&&Get<int>(potionData[1].Ptr(),0xC)==3,"potions fill both players through their own native state");
    rt.potionRechargeEnabled=true;rt.potionRechargeMultiplier=2;
    Check(dg::potions::HookThreshold(states[1].Ptr())==5,"P2 potion recharge is eligible");
    Check(dg::health::RequestRefill(),"health refill queues both alive local players");
    for(unsigned i=0;i<2;++i)dg::health::OnLivePlayer(actors[i].Ptr(),dg::local_players::PublishedSlot(i).generation);
    Check(Get<float>(resources[0].Ptr(),0x37C)==100&&Get<float>(resources[1].Ptr(),0x37C)==100,"native health setter fills both player reserves");
    rt.unlimitedHealthEnabled=true;
    Check(dg::health::HookSet(resources[1].Ptr(),0)==100,"P2 Unlimited health intercepts native health depletion");
    rt.unlimitedHealthEnabled=false;
    Check(dg::health::HookSet(resources[1].Ptr(),20)==20,"Unlimited health OFF restores native damage");
    rt.unlimitedWrathEnabled=true;
    Check(dg::manual_gauges::UnlimitedRequest(1,resources[1].Ptr(),1)==100,"P2 Unlimited uses P2 holder");
    rt.unlimitedWrathEnabled=false;
    Check(dg::manual_gauges::UnlimitedRequest(1,resources[1].Ptr(),1)==1,"Unlimited OFF remains native on P2");
    dg::local_players::Context changed{};changed.index=1;changed.localCount=2;changed.localPlayer=101;
    changed.character=reinterpret_cast<uintptr_t>(actors[2].Ptr());changed.playerState=reinterpret_cast<uintptr_t>(states[1].Ptr());
    dg::local_players::Publish(changed);
    rt.unlimitedWrathEnabled=true;
    Check(dg::manual_gauges::UnlimitedRequest(1,resources[1].Ptr(),1)==1,"P2 respawn invalidates stale old-holder Unlimited");
    Check(dg::ammo::Set(proxies[1].Ptr(),9,0x697D92)==9,"P2 respawn invalidates stale ammo credit");
    Check(dg::resource_gain::Set(0,resources[1].Ptr(),55,0x6EA9C9)==55,"P2 respawn invalidates stale reward ownership");
    Check(dg::potions::HookThreshold(states[1].Ptr())==10,"P2 respawn invalidates stale potion holder");
    rt.unlimitedHealthEnabled=true;
    Check(dg::health::HookSet(resources[1].Ptr(),0)==0,"P2 respawn invalidates old health holder");
    rt.reduceAmmoEnabled=false;rt.unlimitedAmmoEnabled=false;
    Check(dg::ammo::Set(proxies[0].Ptr(),8,0x697D92)==8,"ammo OFF preserves native spend");
}
}
int main(){try{Run();std::cout<<"Actual two-player gameplay services, credits, requests and respawn guards passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
