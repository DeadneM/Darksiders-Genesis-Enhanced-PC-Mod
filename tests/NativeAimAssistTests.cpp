#include "../src/NativeAimAssistFeature.cpp"
#include <vector>
#include <stdexcept>
#include <iostream>
namespace {
using Vec=dg::native_tps::Vec3;
unsigned char actor[0x1000]{},ability[0x230]{},finder[0x80]{},weapon[0x700]{},registry[0x120]{},victim[0x200]{};
unsigned char finderClass[0x80]{},ammoClass[0x80]{},package[0x40]{},table[0x300]{},names[3][128]{};
uintptr_t entries[3]{},chunks[1]{},ammo[4]{};
dg::local_players::Context context{};bool gameThread=true,paused=false,wheel=false,provider=true,throwSelect=false;
unsigned generation=7,calls=0;float expectV=90,expectH=45,expectX=1;void* returned=victim;
template<class T>void Put(void* p,size_t n,T value){std::memcpy(static_cast<unsigned char*>(p)+n,&value,sizeof(value));}
template<class T>T Get(void* p,size_t n){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+n,sizeof(value));return value;}
void Check(bool good,const char* label){if(!good)throw std::runtime_error(label);}
bool Ray(const dg::local_players::Context&,dg::native_tps::MouseRay& out){out.origin={-300,0,300};out.direction={1,0,0};return provider;}
void* Select(void* f,void* a,const Vec* dir,void* callback){
    ++calls;Check(f==finder&&a==actor&&callback==reinterpret_cast<void*>(0x123),"same native helper, owner and callback");
    Check(Get<float>(finder,0x2C)==expectV&&Get<float>(finder,0x30)==expectH,"only requested native angular limits changed during original call");
    Check(Get<float>(finder,0x28)==7500&&dir&&dir->x==expectX,"search radius preserved and direction belongs to the selected mode");
    if(throwSelect)throw std::runtime_error("native callback exception simulation");return returned;
}
Vec* Point(Vec* out,void* target,uint64_t bone){Check(target==victim&&bone==0x888,"same selected native actor and native bone key");*out={2000,200,400};return out;}
void Run(){
    using namespace dg::native_assist;
    std::vector<unsigned char> image(0x3B00000);g_base=reinterpret_cast<uintptr_t>(image.data());
    for(unsigned i=0;i<3;++i)entries[i]=reinterpret_cast<uintptr_t>(names[i]);chunks[0]=reinterpret_cast<uintptr_t>(entries);
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
    unsigned char items[48]{};uintptr_t objectChunks[1]{reinterpret_cast<uintptr_t>(items)};
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,2);
    Put(victim,0xC,1);Put(items,24,reinterpret_cast<uintptr_t>(victim));Put(items,40,33);
    context.index=0;context.character=context.pawn=reinterpret_cast<uintptr_t>(actor);context.localPlayer=0x100;context.controller=0x200;
    g_ready=true;g_select=Select;g_point=Point;g_ray=Ray;auto& rt=dg::runtime::Get();rt.tpsAimAssistEnabled=false;
    const Vec input{0,1,0};auto invoke=[&](){const auto n=calls;auto result=SelectTarget(finder,actor,&input,reinterpret_cast<void*>(0x123));Check(calls==n+1&&result==returned,"original runs exactly once and selected actor is unchanged");Check(Get<float>(finder,0x2C)==90&&Get<float>(finder,0x30)==45,"native angles restored before hook returns");};
    for(unsigned i=0;i<dg::aim_assist::Count;++i){
        strcpy_s(reinterpret_cast<char*>(names[1]+12),116,dg::aim_assist::Classes[i]);strcpy_s(reinterpret_cast<char*>(names[2]+12),116,dg::aim_assist::Packages[i]);
        rt.tpsAimAssistEnabled=false;expectV=90;expectH=45;invoke();
        Vec point{};dg::native_tps::MouseRay ray{};Ray(context,ray);
        Check(Target(context,weapon,ray,Vec{},point)&&point.z==400&&point.y==200,"native selected socket/bounds point used instead of invented actor root");
        rt.tpsAimAssistEnabled=true;rt.tpsAimAssistGlobal=50;expectV=45;expectH=22.5f;invoke();
        rt.tpsAimAssistGlobal=200;expectV=180;expectH=90;invoke();
        rt.tpsAimAssistMode=1;rt.tpsAimAssistAmmo[i]=100;expectV=90;expectH=45;invoke();
        rt.tpsAimAssistAmmo[i]=25;expectV=22.5f;expectH=11.25f;invoke();
        rt.tpsAimAssistMode=0;rt.tpsAimAssistGlobal=100;
    }
    expectV=90;expectH=45;invoke();generation++;Vec point{};dg::native_tps::MouseRay ray{};Ray(context,ray);
    Check(!Target(context,weapon,ray,Vec{},point),"owner generation expires old native choice");invoke();
    Put(items,40,34);Check(!Target(context,weapon,ray,Vec{},point),"reused actor identity cannot authorize native helper call");Put(items,40,33);
    Check(!Target(context,weapon,ray,Vec{3000,0,0},point),"candidate behind muzzle cannot reverse the shot");
    context.index=1;context.localPlayer=0x300;invoke();Check(Target(context,weapon,ray,Vec{},point),"P2 has independent current native choice");
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
