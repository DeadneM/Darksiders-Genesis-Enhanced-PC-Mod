#include "../src/NativeAimAssistFeature.cpp"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
namespace {
using Vec=dg::native_tps::Vec3;
unsigned char actor[0x1100]{},ability[0x230]{},finder[0x80]{},weapon[0x700]{},registry[0x120]{},victim[0x200]{};
unsigned char finderClass[0x80]{},ammoClass[0x80]{},package[0x40]{},table[0x300]{},names[7][128]{};
unsigned char common[0x700]{},commonClass[0x80]{},otherAmmo[0x700]{},otherClass[0x80]{},otherPackage[0x40]{},actionTable[0x20]{};
uintptr_t entries[7]{},chunks[1]{},ammo[6]{};
dg::local_players::Context context{};bool gameThread=true,paused=false,wheel=false,provider=true,throwSelect=false;
unsigned generation=7,calls=0,providerCalls=0;float expectV=90,expectH=45,expectX=0;void* returned=victim;
template<class T>void Put(void* p,size_t n,T value){std::memcpy(static_cast<unsigned char*>(p)+n,&value,sizeof(value));}
template<class T>T Get(void* p,size_t n){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+n,sizeof(value));return value;}
void Check(bool good,const char* label){if(!good)throw std::runtime_error(label);}
bool Ray(const dg::local_players::Context&,dg::native_tps::MouseRay& out){++providerCalls;out.origin={-300,0,300};out.direction={1,0,0};return provider;}
void* Select(void* f,void* a,const Vec* dir,void* callback){
    ++calls;Check(f==finder&&a==actor&&callback==reinterpret_cast<void*>(0x123),"same native helper, owner and callback");
    if(Get<float>(finder,0x2C)!=expectV||Get<float>(finder,0x30)!=expectH)
        throw std::runtime_error("native angular transaction call="+std::to_string(calls)+" expected="+std::to_string(expectV)+"/"+std::to_string(expectH)+" actual="+std::to_string(Get<float>(finder,0x2C))+"/"+std::to_string(Get<float>(finder,0x30)));
    Check(Get<float>(finder,0x28)==7500&&dir&&dir->x==expectX,"search radius preserved and direction belongs to the selected mode");
    if(throwSelect)throw std::runtime_error("native callback exception simulation");return returned;
}
Vec* Point(Vec* out,void* target,uint64_t bone){Check(target==victim&&bone==0x888,"same selected native actor and native bone key");*out={2000,200,400};return out;}
void Run(){
    using namespace dg::native_assist;
    std::vector<unsigned char> image(0x3B00000);g_base=reinterpret_cast<uintptr_t>(image.data());
    for(unsigned i=0;i<7;++i)entries[i]=reinterpret_cast<uintptr_t>(names[i]);chunks[0]=reinterpret_cast<uintptr_t>(entries);
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    strcpy_s(reinterpret_cast<char*>(names[0]+12),116,"MayhemTargetFinder");Put(finderClass,0x18,0);
    Put(ammoClass,0x18,1);Put(ammoClass,0x20,reinterpret_cast<uintptr_t>(package));Put(package,0x18,2);
    Put(finder,0x10,reinterpret_cast<uintptr_t>(finderClass));Put(finder,0x20,reinterpret_cast<uintptr_t>(ability));Put(finder,0x58,reinterpret_cast<uintptr_t>(actor));
    Put(finder,0x28,7500.0f);Put(finder,0x2C,90.0f);Put(finder,0x30,45.0f);Put<uint64_t>(finder,0x3C,0x888);
    Put(ability,0,reinterpret_cast<uintptr_t>(table));Put(table,0x2F0,g_base+0x5A1620);Put(ability,0x218,reinterpret_cast<uintptr_t>(finder));
    Put(ability,0x48,reinterpret_cast<uintptr_t>(actor));Put(ability,0x1B0,reinterpret_cast<uintptr_t>(actor));Put(ability,0x1D8,reinterpret_cast<uintptr_t>(weapon));
    Put(weapon,0x10,reinterpret_cast<uintptr_t>(ammoClass));Put(weapon,0xE8,reinterpret_cast<uintptr_t>(actor));Put(actor,0xC18,uint8_t(1));
    Put(actor,0xC78,reinterpret_cast<uintptr_t>(registry));Put(registry,0xE8,reinterpret_cast<uintptr_t>(actor));Put(registry,0x108,reinterpret_cast<uintptr_t>(ammo));
    Put(registry,0x110,4);Put(registry,0x114,4);ammo[2]=reinterpret_cast<uintptr_t>(weapon);
    context.index=0;context.character=context.pawn=reinterpret_cast<uintptr_t>(actor);context.localPlayer=0x100;context.controller=0x200;
    g_ready=true;g_select=Select;g_point=Point;g_ray=Ray;auto& rt=dg::runtime::Get();rt.tpsAimAssistEnabled=false;
    const Vec input{0,1,0};auto invoke=[&](){const auto n=calls;auto result=SelectTarget(finder,actor,&input,reinterpret_cast<void*>(0x123));Check(calls==n+1&&result==returned,"original runs exactly once and selected actor is unchanged");Check(Get<float>(finder,0x2C)==90&&Get<float>(finder,0x30)==45,"native angles restored before hook returns");};
    for(unsigned i=0;i<dg::aim_assist::Count;++i){
        strcpy_s(reinterpret_cast<char*>(names[1]+12),116,dg::aim_assist::Classes[i]);strcpy_s(reinterpret_cast<char*>(names[2]+12),116,dg::aim_assist::Packages[i]);
        rt.tpsAimAssistEnabled=false;expectV=90;expectH=45;const auto before=providerCalls;const auto disabled=g_disabled.load();invoke();
        Check(providerCalls==before&&g_disabled==disabled+1,"OFF calls the native selector directly without evaluating the TPS ray or changing its arguments");
        rt.tpsAimAssistEnabled=true;rt.tpsAimAssistGlobal=50;expectV=45;expectH=22.5f;invoke();
        rt.tpsAimAssistGlobal=200;expectV=180;expectH=90;invoke();
        rt.tpsAimAssistMode=1;rt.tpsAimAssistAmmo[i]=100;expectV=90;expectH=45;invoke();
        rt.tpsAimAssistAmmo[i]=25;expectV=22.5f;expectH=11.25f;invoke();
        rt.tpsAimAssistMode=0;rt.tpsAimAssistGlobal=100;
    }
    expectV=90;expectH=45;invoke();
    // Native common targeting must follow actual firing actions, not a
    // fixed mouse button, controller trigger or a previous ammo's setting.
    strcpy_s(reinterpret_cast<char*>(names[3]+12),116,"MayhemWeapon_DualPistols_Blueprint_C");Put(commonClass,0x18,3);
    strcpy_s(reinterpret_cast<char*>(names[5]+12),116,dg::aim_assist::Classes[0]);Put(otherClass,0x18,5);
    strcpy_s(reinterpret_cast<char*>(names[6]+12),116,dg::aim_assist::Packages[0]);Put(otherPackage,0x18,6);Put(otherClass,0x20,reinterpret_cast<uintptr_t>(otherPackage));
    Put(common,0x10,reinterpret_cast<uintptr_t>(commonClass));Put(common,0xE8,reinterpret_cast<uintptr_t>(actor));
    Put(otherAmmo,0x10,reinterpret_cast<uintptr_t>(otherClass));Put(otherAmmo,0xE8,reinterpret_cast<uintptr_t>(actor));
    ammo[3]=reinterpret_cast<uintptr_t>(otherAmmo);ammo[5]=reinterpret_cast<uintptr_t>(common);
    Put(registry,0x110,6);Put(registry,0x114,6);Put(actor,0x1038,reinterpret_cast<uintptr_t>(common));
    Put(actor,0x8D0,reinterpret_cast<uintptr_t>(actionTable));Put(actionTable,0x18,g_base+0x669D30);g_actionLayoutKnown=true;
    Put(ability,0x1D8,reinterpret_cast<uintptr_t>(common));rt.tpsAimAssistMode=1;rt.tpsAimAssistAmmo[6]=25;rt.tpsAimAssistAmmo[0]=200;
    Put(actor,0xE10+18,uint8_t(1));expectV=22.5f;expectH=11.25f;invoke();
    Check(CommonAmmo(reinterpret_cast<uintptr_t>(actor),reinterpret_cast<uintptr_t>(common))==6,"native primary action resolves registered Static ammo");
    Put(actor,0xE10+18,uint8_t(0));Put(actor,0xE10+20,uint8_t(1));expectV=180;expectH=90;invoke();
    Check(CommonAmmo(reinterpret_cast<uintptr_t>(actor),reinterpret_cast<uintptr_t>(common))==0,"native secondary action independently resolves registered Standard ammo");
    context.index=1;invoke();context.index=0;
    Put(actor,0xE10+18,uint8_t(1));expectV=90;expectH=45;invoke();
    Check(CommonAmmo(reinterpret_cast<uintptr_t>(actor),reinterpret_cast<uintptr_t>(common))==-1,"different simultaneous ammo leaves the shared query native");
    ammo[3]=reinterpret_cast<uintptr_t>(weapon);expectV=22.5f;expectH=11.25f;invoke();
    Put(actor,0xE10+18,uint8_t(0));Put(actor,0xE10+20,uint8_t(0));expectV=90;expectH=45;invoke();
    Put(actor,0xE10+18,uint8_t(2));invoke();Put(actor,0xE10+18,uint8_t(1));
    Put(actionTable,0x18,g_base+0x669D31);invoke();Put(actionTable,0x18,g_base+0x669D30);
    g_actionLayoutKnown=false;invoke();g_actionLayoutKnown=true;
    ammo[5]=0;invoke();ammo[5]=reinterpret_cast<uintptr_t>(common);
    Put(actor,0x1038,uintptr_t(0));invoke();Put(actor,0x1038,reinterpret_cast<uintptr_t>(common));
    Put(weapon,0xE8,uintptr_t(0));invoke();Put(weapon,0xE8,reinterpret_cast<uintptr_t>(actor));
    rt.tpsAimAssistEnabled=false;invoke();rt.tpsAimAssistEnabled=true;
    rt.tpsAimAssistAmmo[6]=100;invoke();
    rt.tpsAimAssistAmmo[6]=0;expectV=0;expectH=0;invoke();
    Put(ability,0x1D8,reinterpret_cast<uintptr_t>(weapon));rt.tpsAimAssistMode=0;rt.tpsAimAssistGlobal=100;expectV=90;expectH=45;invoke();
    // Return the fixture to one registered ammo before the original tests
    // remove that ammo. The simultaneous-fire case above populated slot 3.
    ammo[3]=0;ammo[5]=0;Put(registry,0x110,4);Put(registry,0x114,4);
    Put(actor,0x1038,uintptr_t(0));Put(actor,0xE10+18,uint8_t(0));Put(actor,0xE10+20,uint8_t(0));
    Vec point{};
    ResolvePoint(&point,victim,0x888,0x59D3BD);
    Check(!g_shot,"unscoped native resolver cannot retain a point for later shots");
    {
        ShotScope outer;
        ResolvePoint(&point,victim,0x888,0x123);
        Check(!outer.used,"unrelated native callers cannot authorize a shot point");
        {
            ShotScope inner;
            ResolvePoint(&point,victim,0x888,0x59D3BD);
            Check(inner.used&&!outer.used&&inner.point.y==200,"nested native consumer keeps its point in its own call");
        }
        Check(g_shot==&outer&&!outer.used,"nested completion restores outer scope without inheriting another point");
        ResolvePoint(&point,victim,0x888,0x59D3BD);outer.Close();
        Check(outer.used&&outer.point.z==400&&!g_shot,"original-consumed point is copied then observation closes synchronously");
    }
    {ShotScope next;Check(!next.used,"next shot cannot reuse a previous target");}
    Check(!g_shot,"no cross-frame target cache");
    context.index=1;context.localPlayer=0x300;invoke();
    rt.tpsAimAssistGlobal=50;expectV=45;expectH=22.5f;throwSelect=true;try{invoke();}catch(const std::runtime_error&){}throwSelect=false;
    Check(!g_nested&&Get<float>(finder,0x2C)==90&&Get<float>(finder,0x30)==45,"angular transaction and nesting restored during C++ callback unwind");
    expectV=90;expectH=45;expectX=0;gameThread=false;invoke();gameThread=true;
    paused=true;invoke();paused=false;wheel=true;invoke();wheel=false;provider=false;invoke();provider=true;
    ammo[2]=0;invoke();ammo[2]=reinterpret_cast<uintptr_t>(weapon);
    context.kind=dg::local_players::PoseKind::Horse;invoke();context.kind=dg::local_players::PoseKind::Foot;
    g_ready=false;rt.tpsAimAssistEnabled=false;
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool CinematicActive(unsigned){return false;}bool IsWorldPaused(void*){return paused;}bool RadialActive(unsigned){return wheel;}}
namespace dg::local_players {bool ResolveActor(void* a,Context& c){if(a!=actor)return false;c=context;return true;}Slot PublishedSlot(unsigned){Slot s;s.generation=generation;return s;}}
int main(){try{Run();std::cout<<"Native acquisition ownership, original-call transaction and selected point scopes passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
