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
bool validRay=true,contactAvailable=true,contactHit=true,traceFromRay=false;unsigned contactQueries=0;dg::native_objects::Identity contactActor{};
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
    // Initial Static target assignment is distinct from its secondary chain.
    unsigned char staticWeapon[0x700]{},staticClass[0x80]{},package[0x40]{},registry[0x120]{};
    unsigned char staticNames[3][128]{};uintptr_t staticEntries[3]{reinterpret_cast<uintptr_t>(staticNames[0]),reinterpret_cast<uintptr_t>(staticNames[1]),reinterpret_cast<uintptr_t>(staticNames[2])};
    uintptr_t staticChunks[1]{reinterpret_cast<uintptr_t>(staticEntries)},ammo[4]{};
    strcpy_s(reinterpret_cast<char*>(staticNames[0]+12),116,"MayhemStaticShot_Blueprint_C");
    strcpy_s(reinterpret_cast<char*>(staticNames[1]+12),116,"/Game/Blueprints/Player/MayhemStaticShot_Blueprint");
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(staticChunks));Put(package,0x18,1);
    Put(staticClass,0x20,reinterpret_cast<uintptr_t>(package));Put(staticWeapon,0x10,reinterpret_cast<uintptr_t>(staticClass));
    Put(staticWeapon,0xE8,static_cast<void*>(character));Put(staticWeapon,0x688,static_cast<void*>(beam));
    Put(character,0xC78,static_cast<void*>(registry));Put(registry,0xE8,static_cast<void*>(character));ammo[2]=reinterpret_cast<uintptr_t>(staticWeapon);
    Put(registry,0x108,reinterpret_cast<uintptr_t>(ammo));Put(registry,0x110,4);Put(registry,0x114,4);
    character[0x118]=3;context.kind=dg::local_players::PoseKind::Foot;context.localPlayer=1;
    g_staticReady=true;
    static void* assigned=nullptr;static unsigned assignments=0;
    g_setTarget=+[](void* b,void* target){Check(b==beam,"same beam forwarded");assigned=target;++assignments;};
    auto retained=[&](uintptr_t caller=0x6981D0){const auto n=assignments;AssignTarget(beam,root,caller);Check(assignments==n+1&&assigned==root,"noneligible assignment forwarded exactly once");};
    AssignTarget(beam,root,0x6981D0);Check(assignments==1&&!assigned&&g_staticTargets[0]==1,"TPS primary Static omits only initial assisted target");
    context.index=1;AssignTarget(beam,root,0x6981D0);Check(!assigned&&g_staticTargets[1]==1,"P2 has independent primary authority");context.index=0;
    AssignTarget(beam,root,0x5B9F80);Check(!assigned,"ongoing aim update cannot reintroduce an assisted target");
    unsigned char victim[0x200]{},other[0x100]{},hitRecord[0x8C]{},objects[48]{};
    void* objectChunks[1]{objects};Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,2);
    Put(objects,24,reinterpret_cast<uintptr_t>(victim));Put(objects,40,33);contactActor={reinterpret_cast<uintptr_t>(victim),1,33};
    Put(root,0xE8,static_cast<void*>(victim));Put(volume,0x280,reinterpret_cast<uintptr_t>(hitRecord));Put(volume,0x288,1);Put(volume,0x28C,1);
    Put<unsigned char>(hitRecord,4,1);Put(hitRecord,0x6C,std::array<int,2>{1,33});
    retained(0x5B9F80);Check(g_staticContacts[0]==1,"actual blocking contact on both native ray and beam keeps native target and chains");
    Put(root,0xE8,static_cast<void*>(other));AssignTarget(beam,root,0x5B9F80);Check(!assigned,"a different beam or ray victim cannot authorize the assisted enemy");Put(root,0xE8,static_cast<void*>(victim));
    Put<unsigned char>(hitRecord,4,0);AssignTarget(beam,root,0x5B9F80);Check(!assigned,"nonblocking overlap does not invent a confirmed hit");Put<unsigned char>(hitRecord,4,1);
    Put(objects,40,34);AssignTarget(beam,root,0x5B9F80);Check(!assigned,"recycled contact identity cannot authorize targeting");Put(objects,40,33);
    contactActor={};AssignTarget(beam,root,0x76B3E0);Check(!assigned,"reflected target reassignment follows the same root policy");
    contactAvailable=false;retained();contactAvailable=true;
    Put(staticWeapon,0x688,static_cast<void*>(root));retained(0x76B3E0);Put(staticWeapon,0x688,static_cast<void*>(beam)); // child/unregistered beam remains native
    providerEnabled=false;retained();providerEnabled=true;
    validRay=false;retained();validRay=true;
    paused=true;retained();paused=false;wheel=true;retained();wheel=false;
    context.kind=dg::local_players::PoseKind::Horse;retained();context.kind=dg::local_players::PoseKind::Foot;
    Put(staticWeapon,8,uint32_t(0x20));retained();Put(staticWeapon,8,uint32_t(0));
    Put(staticWeapon,0x688,static_cast<void*>(root));retained();Put(staticWeapon,0x688,static_cast<void*>(beam));
    staticNames[0][12]='X';retained();staticNames[0][12]='M';
    staticNames[1][12]='X';retained();staticNames[1][12]='/';
    Put(volume,0xE8,static_cast<void*>(character));retained();Put(volume,0xE8,static_cast<void*>(beam));
    context.character=reinterpret_cast<uintptr_t>(grenade);retained();context.character=context.pawn;
    gameThread=false;retained();gameThread=true;g_staticReady=false;retained();g_staticReady=true;
    const auto n=assignments;AssignTarget(beam,nullptr,0x6981D0);Check(assignments==n+1&&!assigned,"native null target remains null without extra setter");
    gameThread=false;Finish(ability,&out);Check(out.yaw==-70,"other threads stay native");
    Check(lavaCalls==5,"native Lava update runs exactly once for each request");
    gameThread=true;context.index=0;
    unsigned char targetRoot[0x1B0]{};Put(victim,0x158,static_cast<void*>(targetRoot));Put(targetRoot,0xE8,static_cast<void*>(victim));Put(targetRoot,0x1A0,Vec3{2000,100,300});
    Put(character,0xE98,std::array<int,2>{1,33});contactActor={reinterpret_cast<uintptr_t>(victim),1,33};
    auto& rt=dg::runtime::Get();dg::native_tps::MouseRay view{},added{};Provider(context,view);const Vec3 farPoint{100000,0,300};
    const unsigned beforeQueries=contactQueries;
    Check(!AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added)&&contactQueries==beforeQueries,"default zero has no new target read/query or correction");
    rt.tpsAimAssistGlobal=50;
    Check(AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added)&&added.direction.y>0,"global assistance consumes native weak target and confirmed LOS");
    const float halfway=added.direction.y;rt.tpsAimAssistGlobal=100;
    Check(AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added)&&added.direction.y>halfway,"live global slider progressively increases3D pull");
    const unsigned visibleQueries=contactQueries;
    Check(!AssistedRay(context,view.origin,staticWeapon,view,Vec3{500,0,300},true,added)&&contactQueries==visibleQueries,"nearby collision takes priority without extra trace");
    contactHit=false;Check(AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"native Projectile overlap/ignore profiles still permit assistance when there is no blocking obstacle");contactHit=true;
    const auto savedContact=contactActor;const auto savedPoint=hitTarget;contactActor={};
    hitTarget={500,0,300};Check(!AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"a nearer surface on the candidate ray still rejects assistance");
    hitTarget={5000,0,300};Check(AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"the camera ray may hit the world behind a nonblocking enemy");
    contactAvailable=false;Check(!AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"an unavailable query must not count as an unobstructed ray");contactAvailable=true;
    hitTarget=savedPoint;contactActor=savedContact;
    Put(objects,40,34);Check(!AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"recycled native target identity rejected");Put(objects,40,33);
    rt.tpsAimAssistMode=1;Check(!AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"per-ammunition zero does not inherit a nonzero global strength");
    collision=false;traceFromRay=true;
    Put(staticClass,0x30,tree+0);Put(staticClass,0x38,0);Put(image.data(),0x380A4F0,static_cast<void*>(cls));
    for(unsigned i=0;i<dg::aim_assist::Count;++i){
        strcpy_s(reinterpret_cast<char*>(staticNames[0]+12),116,dg::aim_assist::Classes[i]);
        strcpy_s(reinterpret_cast<char*>(staticNames[1]+12),116,dg::aim_assist::Packages[i]);
        rt.tpsAimAssistAmmo[i]=60;Vec3 point{};bool hit=false;
        Check(AmmoKind(staticWeapon)==int(i)&&Target(context,view.origin,point,hit,staticWeapon)&&point.y>0,"all seven ammo classes share the same assisted3D collision target");
        context.index=1;Check(AssistedRay(context,view.origin,staticWeapon,view,farPoint,false,added),"P2 reads its own native character target");context.index=0;
        if(i==4||i==6)Check(BeamWeapon(beam)==staticWeapon,"native beam owner maps to the actual registered ammunition");
        if(i==3){Put(ability,0x10,static_cast<void*>(lavaClass));Put(ability,0x50,static_cast<void*>(staticWeapon));UpdateLava(ability);Check(Get<Rotator>(ability,0x20C).yaw>0,"Lava target assistance reaches the paired native ballistic solver");}
        rt.tpsAimAssistAmmo[i]=0;
    }
    rt.tpsAimAssistMode=0;rt.tpsAimAssistGlobal=100;
    strcpy_s(reinterpret_cast<char*>(staticNames[2]+12),116,"MayhemWorldEnder_Blueprint_C");Put(worldClass,0x18,2);
    Put(character,0xF98,static_cast<void*>(worldWeapon));Put(worldWeapon,0x688,static_cast<void*>(beam));
    BeamForward(volume,&beamDirection,0x61D5E5);Check(beamDirection.y==0,"enabled ammo assistance cannot redirect World Ender");Put(character,0xF98,static_cast<void*>(nullptr));
    unsigned char firing[0x110]{},currentAbility[0x1B8]{};void* currentList[1]{currentAbility};
    Put(character,0xC70,static_cast<void*>(firing));Put(firing,0xE8,static_cast<void*>(character));Put(firing,0xF8,currentList+0);Put(firing,0x100,1);Put(firing,0x104,1);
    Put(currentAbility,0x1B0,static_cast<void*>(character));Put(currentAbility,0x50,static_cast<void*>(staticWeapon));
    Put(staticWeapon,0x688,static_cast<void*>(nullptr));Check(!BeamWeapon(beam),"a secondary beam cannot inherit the primary Static weapon's assistance through its active ability");Put(staticWeapon,0x688,static_cast<void*>(beam));
    providerEnabled=false;Rotation(staticWeapon,&out,&in,0x7CFA3B);Check(out.yaw==23,"positive assistance remains outside native view");providerEnabled=true;
    paused=true;Rotation(staticWeapon,&out,&in,0x7CFA3B);Check(out.yaw==23,"positive assistance remains outside pause");paused=false;
    rt.tpsAimAssistMode=0;rt.tpsAimAssistGlobal=0;traceFromRay=false;collision=true;

}
}
namespace dg::native_tps {
bool TargetReady(){return true;}
bool TraceCameraContact(const local_players::Context&,const MouseRay&,Vec3& target,bool& hit,dg::native_objects::Identity& actor,const Vec3*){++contactQueries;target=hitTarget;hit=contactHit;actor=contactActor;return contactAvailable;}
bool OnGameThread(){return gameThread;}
bool CinematicActive(unsigned){return false;}
bool IsWorldPaused(void*){return paused;}
bool RadialActive(unsigned){return wheel;}
bool TraceCameraTarget(const local_players::Context&,const MouseRay& ray,Vec3& target,bool& hit,const Vec3* source){Check(source&&std::isfinite(source->x),"shared collision receives the real consumer origin");target=traceFromRay?Vec3{ray.origin.x+1500*ray.direction.x,ray.origin.y+1500*ray.direction.y,ray.origin.z+1500*ray.direction.z}:hitTarget;hit=collision;return true;}
}
namespace dg::local_players {
bool ResolveActor(void* a,Context& c){if(a!=character)return false;c=context;return true;}
}
int main(){try{Run();std::cout<<"Actual projectile/Finish/Lava target and paired-ballistic scopes passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
