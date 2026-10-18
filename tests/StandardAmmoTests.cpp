#include "../src/LocalPlayerOwnership.cpp"
#include "../src/AmmoConsumptionFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
struct Object {unsigned char bytes[0x1100]{};void* Ptr(){return bytes;}};
struct Type {Object object;void* tree[1]{object.bytes+0x30};};
Object actors[2]{},resources[2]{},proxies[4]{};
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+at,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
int NativeMaximum(void* p){return Get<int>(p,0x350);}
int NativeSet(void* p,int requested){const int cap=dg::ammo::HookMaximum(p);const int result=requested< -1?-1:std::min(requested,cap);Put(p,0x34C,result);return result;}
void Observe(unsigned i){dg::ammo::g_players[i].tick=0;dg::ammo::OnLivePlayer(actors[i].Ptr(),dg::local_players::PublishedSlot(i).generation);}
// Native AUTH_ConsumeAmmo (69839B-6983C6) checks finite max and current
// before calling AUTH_SetAmmo. This fixture uses that native consumer contract.
bool Fire(unsigned i){auto* proxy=proxies[i].Ptr();const int before=Get<int>(proxy,0x34C);
    if(dg::ammo::HookMaximum(proxy)==-1)return true;if(before<=0)return false;
    dg::ammo::Set(proxy,before-1,0x697D92);return true;}
void Run(){
    using namespace dg::ammo;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Type resourceType{},proxyType{};
    for(auto* type:{&resourceType,&proxyType}){Put(type->object.Ptr(),0x30,type->tree+0);Put(type->object.Ptr(),0x38,0);}
    Put(image.data(),0x3810268,resourceType.object.Ptr());Put(image.data(),0x3811D40,proxyType.object.Ptr());
    void* arrays[2][3]{{proxies[0].Ptr(),proxies[2].Ptr(),proxies[3].Ptr()},{proxies[1].Ptr(),nullptr,nullptr}};
    for(unsigned i=0;i<2;++i){
        Put(actors[i].Ptr(),0xEA0,resources[i].Ptr());Put(resources[i].Ptr(),0x10,resourceType.object.Ptr());Put<unsigned char>(resources[i].Ptr(),0x118,3);
        Put(resources[i].Ptr(),0x428,arrays[i]+0);Put(resources[i].Ptr(),0x430,i==0?3:1);
        dg::local_players::Context c{};c.index=int(i);c.localCount=2;c.localPlayer=100+i;c.character=reinterpret_cast<uintptr_t>(actors[i].Ptr());dg::local_players::Publish(c);
    }
    for(unsigned i=0;i<4;++i){auto* p=proxies[i].Ptr();Put(p,0x10,proxyType.object.Ptr());Put<unsigned char>(p,0x118,3);Put(p,0x108,resources[i==1?1:0].Ptr());
        Put<unsigned char>(p,0x348,i<2?1:4);Put(p,0x350,i==3?10:-1);Put(p,0x34C,i==3?10:-1);}
    g_ready=true;g_standardReady=true;g_nativeMaximum=NativeMaximum;g_maximum=HookMaximum;g_original=NativeSet;
    auto& rt=dg::runtime::Get();rt.finiteStandardAmmoEnabled=false;rt.reduceAmmoEnabled=false;rt.unlimitedAmmoEnabled=false;
    Observe(0);Observe(1);Check(HookMaximum(proxies[0].Ptr())==-1&&Fire(0),"OFF retains native infinite Standard");
    rt.finiteStandardAmmoEnabled=true;rt.standardAmmoCapacity=3;Observe(0);Observe(1);
    Check(Get<int>(proxies[0].Ptr(),0x34C)==3&&Get<int>(proxies[1].Ptr(),0x34C)==3,"first enable fills both native Standard reserves");
    Check(Get<int>(proxies[0].Ptr(),0x350)==-1,"native capacity/stat block is never written");
    Check(HookMaximum(proxies[2].Ptr())==-1&&Get<int>(proxies[2].Ptr(),0x34C)==-1,"WorldEnder infinite reserve stays untouched");
    Check(HookMaximum(proxies[3].Ptr())==10,"special ammunition keeps its native capacity");
    Check(Fire(0)&&Fire(0)&&Fire(0)&&!Fire(0)&&Get<int>(proxies[0].Ptr(),0x34C)==0,"native consumption reaches zero and rejects next shot");
    Check(Get<int>(proxies[1].Ptr(),0x34C)==3,"P1 shots do not spend P2 reserve");
    Check(RequestRefill(),"existing fill action queues Standard as well");Observe(0);Observe(1);
    Check(Get<int>(proxies[0].Ptr(),0x34C)==3,"native manual refill restores Standard reserve");
    rt.reduceAmmoEnabled=true;rt.ammoConsumptionDivisor=2;
    Check(Fire(0)&&Get<int>(proxies[0].Ptr(),0x34C)==3&&Fire(0)&&Get<int>(proxies[0].Ptr(),0x34C)==2,"validated fractional consumption applies without a second spend path");
    rt.unlimitedAmmoEnabled=true;Observe(0);Check(Get<int>(proxies[0].Ptr(),0x34C)==3,"Unlimited takes priority and tops up Standard");
    rt.unlimitedAmmoEnabled=false;rt.reduceAmmoEnabled=false;rt.standardAmmoCapacity=1;Observe(0);Check(Get<int>(proxies[0].Ptr(),0x34C)==1,"capacity decrease clamps");
    rt.standardAmmoCapacity=20;Observe(0);Check(Get<int>(proxies[0].Ptr(),0x34C)==1,"capacity increase does not grant ammo");
    rt.finiteStandardAmmoEnabled=false;Observe(0);Observe(1);Check(HookMaximum(proxies[0].Ptr())==-1&&Get<int>(proxies[0].Ptr(),0x34C)==-1&&Get<int>(proxies[1].Ptr(),0x34C)==-1,"OFF restores native infinity for both players");
    rt.finiteStandardAmmoEnabled=true;Put(proxies[0].Ptr(),0x108,resources[1].Ptr());Check(HookMaximum(proxies[0].Ptr())==-1,"foreign or reassigned owner excluded immediately");
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool CinematicActive(unsigned){return false;}bool IsWorldPaused(void*){return false;}}
int main(){try{Run();std::cout<<"Standard native reserve, finite maximum, spend, refill and restoration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
