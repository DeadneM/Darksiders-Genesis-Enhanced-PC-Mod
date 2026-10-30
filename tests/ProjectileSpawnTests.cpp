#include "../src/ProjectileAimFeature.cpp"
#include <iostream>
#include <array>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
using Vec3=dg::native_tps::Vec3;
using Rotator=dg::projectile_aim::Rotator;
alignas(16) unsigned char character[0x1000]{},weapon[0x200]{},grenade[0x200]{},ability[0x230]{},cls[0x80]{},foreignClass[0x80]{},lavaClass[0x80]{};
unsigned char projectileClass[0x100]{},defaults[0x100]{},movement[0x180]{},root[0x1B0]{};
unsigned char beam[0x650]{},volume[0x300]{};
void* tree[1]{cls+0x30};void* foreignTree[1]{foreignClass+0x30};void* lavaTree[2]{cls+0x30,lavaClass+0x30};
uintptr_t componentTable[0x5E8/8]{};
bool nativeChoice=false;
Vec3 nativeChosen{1200,0,400};
bool validRay=true;
bool providerEnabled=true,gameThread=true,paused=false,wheel=false,collision=true,badSpeed=false;
dg::local_players::Context context{};
const void* forwarded=nullptr;
Vec3 hitTarget{1000,1000,500},speedTarget{};
float speedPitch=0,angleDistance=0;
unsigned lavaCalls=0,angleCalls=0,speedCalls=0;
template<class T> void Put(void* p,size_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
template<class T> T Get(void* p,size_t o){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+o,sizeof(v));return v;}
void Check(bool b,const char* msg){if(!b)throw std::runtime_error(msg);}
bool Close(float a,float b){return std::fabs(a-b)<0.001f;}
Rotator* Original(void*,Rotator* out,const void* in){forwarded=in;*out={4,23,7};return out;}
Vec3* OriginalForward(void*,Vec3* out){*out={0.9659258f,0,0.258819f};return out;}
Rotator* OriginalFinish(void* a,Rotator* out){
    *out=Get<void*>(a,0x10)==lavaClass?Get<Rotator>(a,0x20C):Rotator{-12,-70,7};
    out->yaw=-70; // native final helper replaces the early yaw with movement
    return out;
}
void OriginalLava(void* a){++lavaCalls;Put(a,0x20C,Rotator{-20,23,7});Put(a,0x218,2222.0f);}
bool Provider(const dg::local_players::Context& c,dg::native_tps::MouseRay& ray){
    Check(c.character==reinterpret_cast<uintptr_t>(character),"provider receives current native owner");
    ray.origin={-300,0,300};ray.direction=validRay?Vec3{1,0,0}:Vec3{0,0,0};return providerEnabled;
}
void* MovementClass(){return cls;}
void* FindComponent(void* actor,void* type){Check(actor==defaults&&type==cls,"same native CDO component lookup");return movement;}
float Gravity(void* world){Check(world==reinterpret_cast<void*>(0x1234),"same owned world");return -980;}
float Angle(float initial,float gravity,float distance){
    Check(initial==3000&&gravity==-490,"native initial speed and scaled gravity preserved");
    angleDistance=distance;++angleCalls;return 12;
}
float Speed(const Vec3* origin,const Vec3* target,float pitch,float gravity){
    Check(origin->x==0&&origin->z==0&&gravity==-490,"native Lava source position and gravity preserved");
    speedTarget=*target;speedPitch=pitch;++speedCalls;
    return badSpeed?std::numeric_limits<float>::quiet_NaN():3200;
}
void Run(){
    std::vector<unsigned char> image(0x3B00000);
    using namespace dg::projectile_aim;
    g_base=reinterpret_cast<uintptr_t>(image.data());
    for(auto slot:{0x380C0E0,0x3811260,0x3A58AB0})Put(image.data(),slot,static_cast<void*>(cls));
    Put(image.data(),0x380A4B0,static_cast<void*>(cls));
    Put(beam,0x10,static_cast<void*>(cls));Put(beam,0x108,static_cast<void*>(character));
    Put(beam,0x158,static_cast<void*>(volume));Put(beam,0x5E8,static_cast<void*>(volume));
    Put(volume,0xE8,static_cast<void*>(beam));Put(volume,0x1A0,Vec3{1000,0,500});
    Put(image.data(),0x3811050,static_cast<void*>(lavaClass));Put(lavaClass,0x30,lavaTree+0);Put(lavaClass,0x38,1);
    Put(cls,0x30,tree+0);Put(cls,0x38,0);
    Put(foreignClass,0x30,foreignTree+0);Put(foreignClass,0x38,0);
    Put(weapon,0x10,static_cast<void*>(cls));Put(weapon,0xE8,static_cast<void*>(character));
    Put(grenade,0x10,static_cast<void*>(foreignClass));Put(grenade,0xE8,static_cast<void*>(character));
    Put(ability,0x10,static_cast<void*>(cls));Put(ability,0x1B0,static_cast<void*>(character));Put(ability,0x50,static_cast<void*>(weapon));
    character[0xC18]=1;Put(character,0x10,static_cast<void*>(cls));Put(character,0x158,static_cast<void*>(root));
    Put(ability,0x48,static_cast<void*>(character));Put(ability,0x1E0,static_cast<void*>(projectileClass));
    Put(projectileClass,0xF8,static_cast<void*>(defaults));Put(defaults,0,componentTable+0);
    componentTable[0x5E0/8]=reinterpret_cast<uintptr_t>(&FindComponent);
    Put(movement,8,uint32_t(0x40020));Put(movement,0x10,static_cast<void*>(cls));Put(movement,0x138,3000.0f);Put(movement,0x154,0.5f);
    Put(ability,0x204,5.0f);Put(ability,0x208,45.0f);
    context.index=0;context.character=context.pawn=reinterpret_cast<uintptr_t>(character);
    context.controller=0x5678;context.world=0x1234;
    g_original=Original;g_finish=OriginalFinish;g_lava=OriginalLava;g_provider=Provider;
    g_forward=OriginalForward;
    g_movementClass=MovementClass;g_gravity=Gravity;g_arcAngle=Angle;g_arcSpeed=Speed;
    Rotator out{},in{0,0,0};
    auto* result=Rotation(weapon,&out,&in,0x7CFA3B);
    Check(result==&out&&Close(out.pitch,19.4712206f)&&Close(out.yaw,45)&&out.roll==7,"native return aims from native reference to actual 3D camera hit, preserving roll");
    Check(forwarded==&in&&in.pitch==0&&in.yaw==0&&in.roll==0,"native input ABI forwarded untouched");
    in={1000,0,500};Rotation(weapon,&out,&in,0x7CFA3B);
    Check(Close(out.yaw,90)&&Close(out.pitch,0),"actual SpawnLocation argument removes reference-origin parallax instead of inventing a horizontal offset");in={0,0,0};
    Finish(ability,&out);
    Check(Close(out.yaw,45)&&Close(out.pitch,19.4712206f),"native Finish uses target independently of the released physical binding or actor heading");
    context.index=1;Finish(ability,&out);
    Check(Close(out.yaw,45)&&g_finished[1]==1,"P2 Finish gets its own verified owner");
    Put(ability,0x10,static_cast<void*>(lavaClass));UpdateLava(ability);
    const auto arc=Get<Rotator>(ability,0x20C);
    Check(Close(arc.pitch,31.4712206f)&&Close(arc.yaw,45)&&arc.roll==7&&Get<float>(ability,0x218)==3200,"Lava commits native ballistic rotation and speed as a pair");
    Check(Close(angleDistance,1500)&&Close(speedPitch,arc.pitch)&&speedTarget.z==500,"both native ballistic formulas consume the camera target in 3D");
    const auto usedAngles=angleCalls,usedSpeeds=speedCalls;
    collision=false;UpdateLava(ability);
    Check(angleCalls==usedAngles&&speedCalls==usedSpeeds&&Get<float>(ability,0x218)==3000&&Close(Get<Rotator>(ability,0x20C).pitch,44.4712206f),"sky/no-hit retains native default launch speed and mean arc lift, no infinite-range solve");
    collision=true;badSpeed=true;UpdateLava(ability);
    Check(Get<Rotator>(ability,0x20C).pitch==-20&&Get<float>(ability,0x218)==2222,"invalid paired solve keeps both native outputs");
    badSpeed=false;providerEnabled=false;Put(ability,0x10,static_cast<void*>(cls));Finish(ability,&out);
    Check(out.pitch==-12&&out.yaw==-70,"native view, vanilla aim or stale view rejected by provider keeps Finish native");
    Put(ability,0x10,static_cast<void*>(lavaClass));UpdateLava(ability);Check(Get<float>(ability,0x218)==2222,"provider rejection keeps native Lava cache");Put(ability,0x10,static_cast<void*>(cls));
    providerEnabled=true;dg::runtime::Get().tps3DAimEnabled=false;
    Rotation(weapon,&out,&in,0x7CFA3B);Check(out.pitch==4&&Close(out.yaw,45),"simple mode keeps native height and the shared camera-target yaw correction");
    Finish(ability,&out);Check(out.pitch==-12&&Close(out.yaw,45),"simple mode still corrects final movement yaw at charge release while keeping native height");
    Put(ability,0x10,static_cast<void*>(lavaClass));UpdateLava(ability);Check(Get<Rotator>(ability,0x20C).pitch==-20&&Get<float>(ability,0x218)==2222,"simple mode keeps the native ballistic pitch/speed pair");
    Finish(ability,&out);Check(out.pitch==-20&&Close(out.yaw,45)&&Get<float>(ability,0x218)==2222,"Lava shares the same final yaw authority in simple mode, native pitch/speed retained");
    dg::runtime::Get().tps3DAimEnabled=true;Finish(ability,&out);Check(out.pitch==-20&&Close(out.yaw,45)&&Get<float>(ability,0x218)==2222,"3D Lava final yaw cannot break its paired native ballistic pitch");Put(ability,0x10,static_cast<void*>(cls));
    dg::runtime::Get().tps3DAimEnabled=true;
    providerEnabled=true;Rotation(grenade,&out,&in,0x7CFA3B);Check(out.yaw==23,"gear stays outside pistol ancestry");
    Rotation(weapon,&out,&in,0x123);Check(out.yaw==23,"unproved ordinary caller stays native");
    character[0xC18]=2;Finish(ability,&out);Check(out.yaw==-70,"War stays native");character[0xC18]=1;
    wheel=true;Finish(ability,&out);Check(out.yaw==-70,"native selector owns input/aim");wheel=false;
    paused=true;Finish(ability,&out);Check(out.yaw==-70,"pause stays native");paused=false;
    Vec3 beamDirection{};
    BeamForward(volume,&beamDirection,0x61D5E5);
    Check(Close(beamDirection.x,0)&&Close(beamDirection.y,1)&&Close(beamDirection.z,0),"actual native beam consumer aims from volume origin to shared target");
    const auto beamSnapshot=std::to_array(beam);
    const auto volumeSnapshot=std::to_array(volume);
    dg::runtime::Get().tps3DAimEnabled=false;
    BeamForward(volume,&beamDirection,0x61D5E5);
    Check(Close(beamDirection.x,0)&&Close(beamDirection.y,0.9659258f)&&Close(beamDirection.z,0.258819f),"simple beams share target/yaw and retain native vertical pitch");
    dg::runtime::Get().tps3DAimEnabled=true;context.index=0;
    BeamForward(volume,&beamDirection,0x61D5E5);Check(g_beams[0]==1&&g_beams[1]==2,"beam authority isolated for both local players");
    Check(std::memcmp(beam,beamSnapshot.data(),sizeof(beam))==0&&std::memcmp(volume,volumeSnapshot.data(),sizeof(volume))==0,"no beam actor, range, damage, target chain or component write");
    BeamForward(volume,&beamDirection,0x123);Check(beamDirection.x>0.9f&&beamDirection.y==0,"every other UE forward consumer stays native");
    providerEnabled=false;BeamForward(volume,&beamDirection,0x61D5E5);Check(beamDirection.y==0,"native view rejects beam rewrite");providerEnabled=true;
    wheel=true;BeamForward(volume,&beamDirection,0x61D5E5);Check(beamDirection.y==0,"wheel retains native beam direction");wheel=false;
    Put(beam,0x108,static_cast<void*>(grenade));BeamForward(volume,&beamDirection,0x61D5E5);Check(beamDirection.y==0,"foreign beam cannot use local player ray");Put(beam,0x108,static_cast<void*>(character));
    Put(beam,0x5E8,static_cast<void*>(root));BeamForward(volume,&beamDirection,0x61D5E5);Check(beamDirection.y==0,"only native reciprocal root volume qualifies");Put(beam,0x5E8,static_cast<void*>(volume));
    unsigned char worldWeapon[0x700]{},worldClass[0x80]{},worldName[96]{};
    uintptr_t entries[1]{reinterpret_cast<uintptr_t>(worldName)},chunks[1]{reinterpret_cast<uintptr_t>(entries)};
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    strcpy_s(reinterpret_cast<char*>(worldName+12),sizeof(worldName)-12,"MayhemWorldEnder_Blueprint_C");
    Put(character,0xF98,static_cast<void*>(worldWeapon));Put(worldWeapon,0xE8,static_cast<void*>(character));
    Put(worldWeapon,0x10,reinterpret_cast<uintptr_t>(worldClass));Put(worldWeapon,0x688,static_cast<void*>(beam));
    hitTarget={-10000,-10000,500};
    for(bool freePitch:{false,true}){
        dg::runtime::Get().tps3DAimEnabled=freePitch;BeamForward(volume,&beamDirection,0x61D5E5);
        Check(Close(beamDirection.x,0.9659258f)&&Close(beamDirection.y,0)&&Close(beamDirection.z,0.258819f),
            "World Ender follows native muzzle with camera behind, in both aim modes");
    }
    Put(worldWeapon,0x688,static_cast<void*>(nullptr));BeamForward(volume,&beamDirection,0x61D5E5);
    Check(beamDirection.x<0&&beamDirection.y<0,"ordinary ammo beam still follows camera despite World Ender component existing");
    Put(character,0xF98,static_cast<void*>(nullptr));hitTarget={1000,1000,500};
    gameThread=false;Finish(ability,&out);Check(out.yaw==-70,"other threads stay native");
    Check(lavaCalls==5,"native Lava update runs once per request");
    gameThread=true;nativeChoice=true;dg::runtime::Get().tps3DAimEnabled=true;
    Rotation(weapon,&out,&in,0x7CFA3B);Check(Close(out.yaw,0),"ordinary shot uses original native selected point instead of camera background");
    Finish(ability,&out);Check(Close(out.yaw,0),"charged release uses the same native selected point");
    Put(ability,0x10,static_cast<void*>(lavaClass));UpdateLava(ability);
    Check(Close(speedTarget.x,nativeChosen.x)&&Close(speedTarget.z,nativeChosen.z),"native chosen point feeds unchanged Lava paired solver");
    nativeChoice=false;


}
}
namespace dg::native_tps {
bool TargetReady(){return true;}
bool OnGameThread(){return gameThread;}
bool CinematicActive(unsigned){return false;}
bool IsWorldPaused(void*){return paused;}
bool RadialActive(unsigned){return wheel;}
bool TraceCameraTarget(const local_players::Context&,const MouseRay&,Vec3& target,bool& hit,const Vec3* source){Check(source&&std::isfinite(source->x),"shared collision receives the real consumer origin");target=hitTarget;hit=collision;return true;}
}
namespace dg::local_players {
bool ResolveActor(void* a,Context& c){if(a!=character)return false;c=context;return true;}
}
int main(){try{Run();std::cout<<"Actual projectile/Finish/Lava target and paired-ballistic scopes passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

namespace dg::native_assist {
void Initialize(dg::projectile_aim::LogFn,bool,dg::projectile_aim::RayProvider){}
int AmmoKind(void*){return -1;}
bool Target(const dg::local_players::Context&,void*,const dg::native_tps::MouseRay&,const dg::native_tps::Vec3&,dg::native_tps::Vec3& point){if(!nativeChoice)return false;point=nativeChosen;return true;}
void Report(){}
}
