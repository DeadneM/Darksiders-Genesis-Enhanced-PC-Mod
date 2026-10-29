#include "../src/PistolVisualFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <limits>
namespace {
using dg::pistol_visual::Matrix;using dg::pistol_visual::Matrices;
struct Transform {float q[4]{},t[4]{},s[4]{};}; // unrelated native socket-pose fixture
unsigned char actors[2][0x1100]{},meshes[2][0x800]{},skeletons[2][0x400]{},renderObjects[2][0x80]{};
unsigned char packets[2][0x110]{},guns[2][2][0x600]{},items[48*24]{},names[48][128]{},notify[0x100]{},klass[0x40]{},effect[0x40]{};
uintptr_t nameEntries[48]{},nameChunks[1]{reinterpret_cast<uintptr_t>(nameEntries)};void* chunks[1]{items};
Matrices current[2]{},previous[2]{};std::array<Transform,80> poses[2]{};
unsigned char controllers[2][0x360]{},actorClass[0x40]{};
dg::local_players::Context owners[2]{};
bool gameThread=true,paused=false,cinematic=false,radial=false,presentation=true;unsigned hiddenCalls=0;
std::vector<std::pair<bool,unsigned char>> hiddenEvents;
uintptr_t forwarded[7]{};bool initResult=true;
template<class T> void Put(void* p,size_t offset,const T& value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T> T Get(void* p,size_t offset){T value{};std::memcpy(&value,static_cast<unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
Matrix Translation(float x,float y,float z){Matrix a{};for(unsigned i=0;i<4;++i)a.m[i][i]=1;a.m[3][0]=x;a.m[3][1]=y;a.m[3][2]=z;return a;}
bool Equal(const Matrix& a,const Matrix& b){for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j)if(std::abs(a.m[i][j]-b.m[i][j])>.001f)return false;return true;}
dg::native_objects::Identity Register(void* p,int index){
    Put(p,0xC,index);const uintptr_t pointer=reinterpret_cast<uintptr_t>(p);Put(items,size_t(index)*24,pointer);Put(items,size_t(index)*24+16,index+17);
    return {pointer,index,index+17};
}
void Name(unsigned index,const char* text){nameEntries[index]=reinterpret_cast<uintptr_t>(names[index]);strcpy_s(reinterpret_cast<char*>(names[index]+12),sizeof(names[index])-12,text);}
void HiddenSetter(void* object,bool hidden,unsigned char propagation){
    Check(propagation<=2,"native propagation enum valid");++hiddenCalls;hiddenEvents.emplace_back(hidden,propagation);
    auto flags=Get<unsigned char>(object,0x1CC);flags=static_cast<unsigned char>((flags&~0x40u)|(hidden?0x40u:0));Put(object,0x1CC,flags);
}
void Policy(){
    using namespace dg::pistol_visual;
    Matrix inverse{};const auto translated=Translation(12,-5,0);
    Check(Inverse(translated,inverse)&&Equal(Multiply(translated,inverse),Translation(0,0,0)),"row-vector inverse cancels translation");
    Matrix singular{};Check(!Inverse(singular,inverse),"singular matrix rejected");
    // Noncommuting bind/spine rotations: this is the actual skinning
    // identity, rather than only a translation fixture for the solver.
    Matrix bindSpine=Translation(15,-6,2);bindSpine.m[0][0]=bindSpine.m[1][1]=0;
    bindSpine.m[0][1]=1;bindSpine.m[1][0]=-1;
    Matrix invBindSpine{},invBindArm{},invRestSpine{};
    const auto restSpine=Translation(8,3,4),nowSpine=bindSpine,restArm=Translation(40,10,5);
    Check(Inverse(bindSpine,invBindSpine)&&Inverse(Translation(2,9,-4),invBindArm)&&Inverse(restSpine,invRestSpine),"native bind matrices are invertible");
    Matrices skin{},skinOld{};for(auto& bone:skin)bone=Translation(0,0,0);
    skin[41]=Multiply(invBindArm,restArm);ArmPose bound{};
    std::array<Matrix,2> packetSpine{Multiply(invBindSpine,restSpine),Multiply(invBindSpine,restSpine)};
    Check(bound.Capture(skin,packetSpine),"capture real inverse-bind representation");
    packetSpine={Multiply(invBindSpine,nowSpine),Multiply(invBindSpine,nowSpine)};skinOld=skin;
    Check(bound.Apply(1,packetSpine,500,skin,&skinOld)&&Equal(skin[41],Multiply(Multiply(Multiply(invBindArm,restArm),invRestSpine),nowSpine)),"spine bind cancels with rotation, leaving the correct native skinning transform");
    Matrices source{},output{},old{};
    for(unsigned i=0;i<BoneCount;++i)source[i]=Translation(float(i),2,3);
    ArmPose cache{};std::array<Matrix,2> clav{Translation(10,0,0),Translation(-10,0,0)};
    Check(cache.Capture(source,clav),"capture both whole arm branches relative to the spine");
    output=source;old=source;clav[0]=Translation(20,7,0);
    Check(cache.Apply(1,clav,1000,output,&old),"right only application");
    for(unsigned i=0;i<BoneCount;++i){
        const bool right=i>=Clavicle[0]&&i<Upper[0]+ArmBones;
        Check(Equal(output[i],right?Translation(float(i)+10,9,3):source[i]),"opposite arm, legs, spine, scarf and holsters preserved");
        Check(Equal(old[i],output[i]),"first frame has no phantom old aim pose");
    }
    const auto last=output;source[41].m[3][1]+=3;
    Check(cache.Capture(source,{Translation(10,0,0),Translation(-10,0,0)}),"refresh animated source without discarding render history");
    clav[0]=Translation(25,7,0);output=source;old=source;
    Check(cache.Apply(1,clav,1050,output,&old)&&Equal(old[41],last[41]),"previous render packet uses last transformed arm");
    auto bad=clav;bad[1].m[0][0]=std::numeric_limits<float>::infinity();const auto before=output;
    Check(!cache.Apply(3,bad,1060,output,&old)&&std::memcmp(&before,&output,sizeof(output))==0,"invalid opposite arm rejects transaction without partial write");
    Check(!cache.Apply(4,clav,1070,output,nullptr),"unsupported mask rejected");
    output=source;old=source;clav[1]=Translation(-20,5,0);
    Check(cache.Apply(2,clav,1200,output,&old),"left complete arm branch remains independent");
    for(unsigned i=0;i<BoneCount;++i){
        const bool left=i>=17&&i<=39;
        Check(Equal(output[i],left?Translation(float(i)-10,7,3):source[i]),"left OFF includes clavicle and preserves native right branch");
    }
    output=source;old=source;Check(HideHolsters(2,output,&old),"left holster render collapse");
    for(unsigned i=0;i<BoneCount;++i)if(i!=78)Check(Equal(output[i],source[i]),"holster suppression cannot alter arms, root, or opposite holster");
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j)Check(output[78].m[i][j]==0&&old[78].m[i][j]==0,"current and previous holster triangles collapse together");
    Check(output[78].m[3][0]==source[78].m[3][0],"holster collapse preserves finite translation");
    output=source;old=source;output[78].m[0][0]=std::numeric_limits<float>::infinity();const auto untouched=old;
    Check(!HideHolsters(3,output,&old)&&std::memcmp(&old,&untouched,sizeof(old))==0,"bad holster matrix cannot partially change other packet");
    Check(!HideHolsters(4,output,nullptr),"unknown holster mask rejected");
    Check(HideEffect(EffectSide::Right,1)&&!HideEffect(EffectSide::Left,1)&&!HideEffect(EffectSide::Shared,1)&&
          HideEffect(EffectSide::Shared,3)&&!HideEffect(EffectSide::Unknown,3),"per-hand and shared root effects remain distinct");
    AnimationClock clock{};const std::array<float,3> lengths{.8666667f,2.6666667f,1.0666667f};
    Check(clock.Step(10,0,.25f,lengths),"seed clip phase from native unarmed node");
    const float phase=clock.phase;
    Check(clock.Step(10,1,0,lengths)&&clock.phase==phase&&clock.speed==0,"paused world cannot advance cosmetic animation");
    Check(clock.Step(10.1f,1,0,lengths)&&clock.phase>phase&&clock.speed>0&&clock.speed<1,"live time advances clips and blends speed");
    std::array<BlendSample,2> samples{};const auto n=clock.Samples(lengths,samples);float sum=0;
    for(unsigned i=0;i<n;++i){sum+=samples[i].weight;Check(samples[i].index>=0&&samples[i].index<3&&samples[i].time>=0&&samples[i].time<lengths[size_t(samples[i].index)],"native sample time is within its own clip");}
    Check(n<=2&&std::abs(sum-1)<.001f,"two adjacent samples carry normalized weights");
    Check(clock.Step(2,1,.5f,lengths)&&clock.phase==.5f&&clock.speed==1,"travel/time reversal resets instead of catching up");
    Check(!clock.Step(2,std::numeric_limits<float>::quiet_NaN(),0,lengths),"invalid speed rejected");
    AnimationRig rig{};std::array<LocalTransform,80> local{};Matrices skinning{};
    for(unsigned i=0;i<80;++i){rig.parents[i]=i?int(i)-1:-1;rig.inverseBind[i]=Translation(0,0,0);local[i].q[3]=1;local[i].t[0]=1;for(unsigned j=0;j<3;++j)local[i].s[j]=1;}
    Check(rig.Skin(local,skinning)&&Equal(skinning[79],Translation(80,0,0)),"local native clips compose through the actual parent hierarchy");
    const auto saved=skinning;rig.parents[40]=41;
    Check(!rig.Skin(local,skinning)&&std::memcmp(&saved,&skinning,sizeof(saved))==0,"invalid hierarchy fails without partial output");
    LocalTransform rotated{};rotated.q[2]=rotated.q[3]=std::sqrt(.5f);rotated.s[0]=2;rotated.s[1]=3;rotated.s[2]=4;
    Matrix rotation{};Check(LocalMatrix(rotated,rotation)&&std::abs(rotation.m[0][1]-2)<.001f&&std::abs(rotation.m[1][0]+3)<.001f&&rotation.m[2][2]==4,"UE row-vector quaternion conversion retains nonuniform scale");
}
void Runtime(){
    using namespace dg::pistol_visual;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,48);
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    Name(30,"Strife_Blueprint_C");Put(actorClass,0x18,30);
    g_ready=true;g_hiddenFn=HiddenSetter;g_players[0]={};g_players[1]={};
    dg::runtime::Get().pistolRightModel=0;dg::runtime::Get().pistolLeftModel=0;
    g_init=+[](void* d,void* c,void* r,int lod,void* m,void* w,int mode){
        forwarded[0]=reinterpret_cast<uintptr_t>(d);forwarded[1]=reinterpret_cast<uintptr_t>(c);forwarded[2]=reinterpret_cast<uintptr_t>(r);
        forwarded[3]=uintptr_t(lod);forwarded[4]=reinterpret_cast<uintptr_t>(m);forwarded[5]=reinterpret_cast<uintptr_t>(w);forwarded[6]=uintptr_t(mode);
        return initResult;
    };
    Check(HookInit(reinterpret_cast<void*>(1),reinterpret_cast<void*>(2),reinterpret_cast<void*>(3),4,reinterpret_cast<void*>(5),reinterpret_cast<void*>(6),7),"native render initializer result forwarded");
    for(unsigned i=0;i<7;++i)Check(forwarded[i]==i+1,"all seven native initializer arguments retained");
    initResult=false;Check(!HookInit(nullptr,nullptr,nullptr,0,nullptr,nullptr,0),"failed native initialization is never repaired or overridden");initResult=true;
    for(unsigned p=0;p<2;++p){
        auto& s=g_players[p].snapshot;
        s.actor=Register(actors[p],int(p*5));s.mesh=Register(meshes[p],int(p*5+1));s.skeleton=Register(skeletons[p],int(p*5+2));
        s.tick=1000;s.present=s.gameplay=true;
        Put(actors[p],0x348,reinterpret_cast<uintptr_t>(controllers[p]));Put(controllers[p],0x350,s.actor.address);
        Put(actors[p],0x10,reinterpret_cast<uintptr_t>(actorClass));Put<unsigned char>(actors[p],0x118,3);Put<unsigned char>(actors[p],0xC18,1);
        Put(actors[p],0x380,s.mesh.address);
        owners[p].index=int(p);owners[p].kind=dg::local_players::PoseKind::Foot;owners[p].localPlayer=1+p;
        owners[p].character=owners[p].pawn=s.actor.address;owners[p].controller=reinterpret_cast<uintptr_t>(controllers[p]);
        Put(meshes[p],0xE8,s.actor.address);Put(meshes[p],0x5A0,s.skeleton.address);Put(meshes[p],0x6C0,reinterpret_cast<uintptr_t>(renderObjects[p]));
        Put(renderObjects[p],0,g_base+0x2DC8930);Put(renderObjects[p],0x40,uintptr_t(0x1234));
        Put(packets[p],0,g_base+0x2DC88A8);Put(packets[p],0x10,Array{reinterpret_cast<uintptr_t>(&current[p]),80,80});
        Put(packets[p],0x20,Array{reinterpret_cast<uintptr_t>(&previous[p]),80,80});
        Put(meshes[p],0x5A8,std::array<int,2>{-1,0});Put(meshes[p],0x5F4,0);
        Put(meshes[p],0x5B0,Array{reinterpret_cast<uintptr_t>(&poses[p]),80,80});
        for(unsigned i=0;i<80;++i){current[p][i]=Translation(i==14?10.0f:float(i),float(p),5);poses[p][i].q[3]=1;poses[p][i].s[0]=poses[p][i].s[1]=poses[p][i].s[2]=1;}
        previous[p]=current[p];poses[p][14].t[0]=10;poses[p][17].t[0]=-10;
        for(unsigned side=0;side<2;++side){
            auto gun=Register(guns[p][side],int(p*5+3+side));
            Put(actors[p],0x1040+side*8,gun.address);Put(guns[p][side],0,g_base+0x2A575C8);
            Put(guns[p][side],0xE8,s.actor.address);Put(guns[p][side],0x108,s.mesh.address);Put<unsigned char>(guns[p][side],0x1CC,0x21);
        }
        const std::array<Matrix,2> anchors{current[p][14],current[p][14]};
        Check(g_players[p].pose.Capture(current[p],anchors),"seed independent animated sample for packet fixture");
        g_players[p].sampleTick=1000;
        Check(!ProcessPacket(reinterpret_cast<uintptr_t>(packets[p]),s.mesh.address,0x1234,0,1000),"both Native do not process or capture render arms");
    }
    const auto nativePose=poses[0];
    std::array<unsigned char,sizeof(actors)> beforeActors{};std::memcpy(beforeActors.data(),actors,sizeof(actors));
    const auto baseline=current[0];auto& s=g_players[0].snapshot;s.lower=true;s.tick=1100;
    dg::runtime::Get().pistolRightModel=3;poses[0][14].t[0]=900;current[0][14]=Translation(20,0,5);
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"render packet receives scoped OFF pose");
    for(unsigned i=0;i<80;++i)if(i!=79&&i!=14)Check(Equal(current[0][i],i>=40&&i<=62?Translation(float(i)+10,0,5):baseline[i]),"right OFF includes aiming shoulder but preserves opposite arm and spine");
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j)Check(current[0][79].m[i][j]==0,"only OFF-side holster collapsed in runtime packet");
    auto expectedPose=nativePose;expectedPose[14].t[0]=900;
    Check(std::memcmp(&poses[0],&expectedPose,sizeof(expectedPose))==0&&std::memcmp(beforeActors.data(),actors,sizeof(actors))==0,"native poses, action bytes, firing actors and sockets never written");
    Check(Equal(current[1][41],Translation(41,1,5)),"P1 render packet cannot alter P2");
    current[0]=baseline;current[0][14]=Translation(20,0,5);previous[0]=baseline;poses[0][17].t[0]=500;
    dg::runtime::Get().pistolRightModel=0;dg::runtime::Get().pistolLeftModel=3;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"left OFF uses the same complete branch solve");
    for(unsigned i=17;i<=39;++i)Check(Equal(current[0][i],Translation(float(i)+10,0,5)),"component pose from another animation update cannot affect the packet-consistent resting arm");
    Check(poses[0][17].t[0]==500,"native aiming clavicle and firing sockets remain untouched");poses[0][17].t[0]=-10;
    dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;
    current[0]=baseline;s.tick=1000;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1400)&&Equal(current[0][41],baseline[41])&&current[0][79].m[0][0]==0,"expired pose keeps permanent holster hiding without reusing animation");
    current[0]=baseline;s.gameplay=false;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1500)&&Equal(current[0][41],baseline[41])&&current[0][79].m[0][0]==0,"focus loss keeps holster hidden and leaves native arms intact");s.gameplay=true;
    // Same already verified pose survives native UI suspension beyond either TTL.
    current[0]=baseline;current[0][41]=Translation(900,3,8);s.suspended=true;s.gameplay=false;s.lower=true;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,9000)&&Equal(current[0][41],Translation(41,0,5)),"native paused packet keeps verified OFF arm without a fresh sample");
    Check(Equal(current[0][17],baseline[17]),"UI pose does not change the equipped opposite arm");
    s.suspended=false;current[0]=baseline;current[0][41]=Translation(900,3,8);
    ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,9001);
    Check(Equal(current[0][41],Translation(900,3,8)),"UI close cannot reuse an expired pose in active gameplay");s.gameplay=true;
    s.tick=1100;Put(items,size_t(s.actor.index)*24+16,s.actor.serial+1);
    Check(!ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"reused actor serial cannot reuse cached pose");
    Put(items,size_t(s.actor.index)*24+16,s.actor.serial);
    Put(packets[0],0x20,Array{reinterpret_cast<uintptr_t>(&previous[0]),1,80});
    Check(!ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"unexpected previous matrix layout rejected");
    Put(packets[0],0x20,Array{reinterpret_cast<uintptr_t>(&previous[0]),80,80});
    unsigned char ability[0x1C0]{},component[0x180]{},registry[0x140]{},weapon[0xF0]{};uintptr_t active[1]{reinterpret_cast<uintptr_t>(ability)},slots[6]{};
    Put(actors[0],0xC70,reinterpret_cast<uintptr_t>(component));Put(component,0xE8,s.actor.address);
    Put<unsigned char>(actors[0],0xD15,0);Put<unsigned char>(actors[0],0xD16,1);
    Check(Ranged(s.actor.address),"native ranged idle permits arm presentation with no active action");
    Put(component,0xF8,Array{reinterpret_cast<uintptr_t>(active),1,1});Put(ability,0x48,s.actor.address);Put(ability,0x1B0,s.actor.address);
    Check(!Ranged(s.actor.address),"dash/chest/unknown ability without pistol weapon stays native");
    Put(actors[0],0xC78,reinterpret_cast<uintptr_t>(registry));Put(registry,0xE8,s.actor.address);slots[2]=reinterpret_cast<uintptr_t>(weapon);
    Put(registry,0x108,Array{reinterpret_cast<uintptr_t>(slots),6,6});Put(ability,0x50,slots[2]);Put(weapon,0xE8,s.actor.address);
    Check(Ranged(s.actor.address),"registered native ammo ability can lower the OFF arm");
    Put(ability,0x48,uintptr_t(0x1234));Check(!Ranged(s.actor.address),"foreign ability cannot qualify as owned shooting");Put(ability,0x48,s.actor.address);
    Put<unsigned char>(actors[0],0xD15,1);Check(!Ranged(s.actor.address),"melee animation remains native");Put<unsigned char>(actors[0],0xD15,0);
    HideAt(0,0,s);Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"hidden flag preserves native visibility and unrelated bits");
    HookHidden(guns[0][0],true,1);Restore(g_hidden[0][0]);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"foreign native hide supersedes our original baseline");
    HookHidden(guns[0][0],false,1);HideAt(0,0,s);HookHidden(guns[0][0],false,1);HideAt(0,0,s);Restore(g_hidden[0][0]);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x21,"return to Native restores latest foreign visibility intent");
    Name(0,"AnimNotify_PlayParticleEffect");Name(1,"VFX_strife_muzzleFlash");Name(2,"VFX_Weapon_Gun_R");Name(3,"VFX_Weapon_Gun_L");Name(4,"VFX_Root_Beam");Name(5,"unrelated_hit_effect");
    Put(klass,0x18,0);Put(notify,0x10,reinterpret_cast<uintptr_t>(klass));Put(effect,0x18,1);Put(notify,0x38,reinterpret_cast<uintptr_t>(effect));
    const auto now=GetTickCount64();s.tick=now;Put(notify,0x84,std::array<int,2>{2,0});
    Check(SkipParticle(notify,meshes[0]),"owned right muzzle suppressed");Put(notify,0x84,std::array<int,2>{3,0});
    Check(!SkipParticle(notify,meshes[0]),"visible left muzzle preserved");Put(notify,0x84,std::array<int,2>{4,0});
    Check(!SkipParticle(notify,meshes[0]),"shared root effect preserved with visible hand");dg::runtime::Get().pistolLeftModel=3;
    Check(SkipParticle(notify,meshes[0]),"shared root effect suppressed only with both OFF");Put(effect,0x18,5);
    Check(!SkipParticle(notify,meshes[0]),"unrelated effect never suppressed");Put(effect,0x18,1);
    Name(6,"AnimNotify_CallAbilityFunction");Put(klass,0x18,6);
    Check(!SkipParticle(notify,meshes[0]),"World Ender lifecycle notification is never a muzzle particle");Put(klass,0x18,0);gameThread=false;
    Check(!SkipParticle(notify,meshes[0]),"notify outside game thread retained");gameThread=true;
    dg::local_players::Context horse{};horse.index=0;horse.kind=dg::local_players::PoseKind::Horse;
    HideAt(0,0,s);OnLivePlayer(horse,9,true);
    Check(!PoseReady(0)&&Get<unsigned char>(guns[0][0],0x1CC)==0x21,"leaving normal Strife restores model and discards pose");
    unsigned char horseActor[0xC10]{},horseMesh[0x800]{},horseGuns[4][0x600]{},pc[0x360]{},strifeClass[0x40]{},horseClass[0x40]{};
    const auto horseId=Register(horseActor,10),horseMeshId=Register(horseMesh,11);
    Name(7,"Strife_Blueprint_C");Name(8,"Despair_Blueprint_C");Put(strifeClass,0x18,7);Put(horseClass,0x18,8);
    Put(actors[0],0x10,reinterpret_cast<uintptr_t>(strifeClass));Put<unsigned char>(actors[0],0xC18,1);
    Put(horseActor,0x10,reinterpret_cast<uintptr_t>(horseClass));Put<unsigned char>(horseActor,0x118,3);
    Put(horseActor,0x348,reinterpret_cast<uintptr_t>(pc));Put(pc,0x350,horseId.address);
    Put(horseActor,0x380,horseMeshId.address);Put(horseMesh,0xE8,horseId.address);
    horse.localPlayer=1;horse.controller=reinterpret_cast<uintptr_t>(pc);horse.character=reinterpret_cast<uintptr_t>(actors[0]);horse.pawn=horseId.address;owners[0]=horse;
    for(unsigned i=0;i<4;++i){
        const auto id=Register(horseGuns[i],12+int(i));Put(horseActor,(i<2?0xBA0:0xBF8)+(i%2)*8,id.address);
        Put(horseGuns[i],0,g_base+0x2A575C8);Put(horseGuns[i],0xE8,horseId.address);Put(horseGuns[i],0x108,horseMeshId.address);
        Put<unsigned char>(horseGuns[i],0x1CC,0x21);
    }
    dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[0],0x1CC)==0x61&&Get<unsigned char>(horseGuns[2],0x1CC)==0x61,"mounted OFF hides both owned right hand and stowed pistol");
    Check(Get<unsigned char>(horseGuns[1],0x1CC)==0x21&&Get<unsigned char>(horseGuns[3],0x1CC)==0x21,"mounted opposite hand and stowed model remain native");
    Check(!PoseReady(0),"mounted model hiding does not reuse foot arm pose or horse animation");
    paused=radial=true;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[0],0x1CC)==0x61,"wheel/pause cannot restore OFF models");
    Restore(g_hidden[0][2]);
    const auto events=hiddenEvents.size();HookHidden(horseGuns[0],false,2);
    Check(hiddenEvents.size()==events+2&&hiddenEvents[events]==std::pair<bool,unsigned char>{false,2}&&
          hiddenEvents[events+1]==std::pair<bool,unsigned char>{true,1}&&Get<unsigned char>(horseGuns[0],0x1CC)==0x61,
          "first native visibility event discovers uncached OFF component and preserves child propagation");
    paused=radial=false;
    HookHidden(horseGuns[0],true,1);dg::runtime::Get().pistolRightModel=0;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[0],0x1CC)==0x61&&Get<unsigned char>(horseGuns[2],0x1CC)==0x21,"Native restores latest native mounted visibility independently per component");
    HookHidden(horseGuns[0],false,1);dg::runtime::Get().pistolLeftModel=3;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[1],0x1CC)==0x61&&Get<unsigned char>(horseGuns[3],0x1CC)==0x61,"left mounted OFF uses left offsets only");
    OnLivePlayer(horse,9,false);Check(Get<unsigned char>(horseGuns[1],0x1CC)==0x61&&Get<unsigned char>(horseGuns[3],0x1CC)==0x61,"focus loss retains mounted OFF appearance");
    const auto n=hiddenCalls;Put(horseActor,0x348,uintptr_t(0));OnLivePlayer(horse,9,true);
    Check(hiddenCalls==n+2&&Get<unsigned char>(horseGuns[1],0x1CC)==0x21,"invalid mounted controller restores only the existing leases");
    // Return to a normal pawn with no cached arm pose: OFF still applies now.
    struct Info {int name,number,parent;};std::array<Info,80> hierarchy{};
    constexpr int parents[ArmBones]={-1,0,1,2,3,4,2,6,7,2,9,10,2,12,13,2,15,16,2,2,1,0};
    for(unsigned side=0;side<2;++side)for(unsigned j=0;j<ArmBones;++j)
        hierarchy[Upper[side]+j].parent=j?int(Upper[side])+parents[j]:int(Clavicle[side]);
    const unsigned anchors[]={14,17,18,20,40,41,43,78,79};
    const char* boneNames[]={"Bone_Strife_Spine2","Bone_Strife_Clavicle_L","Bone_Strife_UpperArm_L","Bone_Strife_Hand_L","Bone_Strife_Clavicle_R","Bone_Strife_UpperArm_R","Bone_Strife_Hand_R","Bone_Strife_Holster_R","Bone_Strife_Holster_L"};
    for(unsigned i=0;i<9;++i){Name(9+i,boneNames[i]);hierarchy[anchors[i]].name=int(9+i);}
    hierarchy[14].parent=13;hierarchy[17].parent=hierarchy[40].parent=14;hierarchy[78].parent=hierarchy[79].parent=1;
    Put(skeletons[0],0x190,Array{reinterpret_cast<uintptr_t>(hierarchy.data()),80,80});
    unsigned char resource[0x380]{};Put<float>(resource,0x37C,100);
    Put(actors[0],0xEA0,reinterpret_cast<uintptr_t>(resource));Put<unsigned char>(actors[0],0x118,3);
    Put(actors[0],0x348,reinterpret_cast<uintptr_t>(pc));Put(pc,0x350,reinterpret_cast<uintptr_t>(actors[0]));Put(actors[0],0x380,reinterpret_cast<uintptr_t>(meshes[0]));
    horse.kind=dg::local_players::PoseKind::Foot;horse.pawn=horse.character;owners[0]=horse;dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;
    OnLivePlayer(horse,10,true);
    Check(!PoseReady(0)&&Get<unsigned char>(guns[0][0],0x1CC)==0x61,"return to foot keeps OFF immediately while waiting for a fresh animation evaluation");
    Check(!g_players[0].pose.ready,"no frozen arm pose survives a transformation");
    auto& footPlayer=g_players[0];Check(footPlayer.pose.Capture(current[0],{current[0][14],current[0][14]}),"seed a verified gameplay sample before UI suspension");
    footPlayer.snapshot.lower=true;footPlayer.sampleTick=1;
    paused=radial=true;OnLivePlayer(horse,10,true);HookHidden(guns[0][0],false,1);
    Check(footPlayer.snapshot.suspended&&footPlayer.snapshot.lower&&footPlayer.pose.ready,"native UI observer preserves the already verified unarmed presentation");
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"normal-foot wheel cannot briefly show the hidden pistol");
    Restore(g_hidden[0][0]);OnMeshAssigned(guns[0][0]);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"mesh refresh reapplies OFF before the next live observer");
    dg::runtime::Get().pistolRightModel=0;OnLivePlayer(horse,10,true);paused=radial=false;
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x21,"Native restores foot visibility while wheel is open");
    // The blades are exact primitive weapon meshes, with separate native intent.
    unsigned char blade[2][0x800]{};Name(18,"Dagger_R");Name(19,"Dagger_L");
    for(unsigned side=0;side<2;++side){
        const auto id=Register(blade[side],20+int(side));
        Put(actors[0],0x1010+side*8,id.address);Put(blade[side],0,g_base+0x2724250);
        Put(blade[side],0x18,18+int(side));Put(blade[side],0xE8,reinterpret_cast<uintptr_t>(actors[0]));
        Put(blade[side],0x108,reinterpret_cast<uintptr_t>(meshes[0]));Put<unsigned char>(blade[side],0x1CC,0x21);
    }
    dg::runtime::Get().strifeBladeVisibility=1;OnLivePlayer(horse,10,true);
    Check(Get<unsigned char>(blade[0],0x1CC)==0x21&&Get<unsigned char>(blade[1],0x1CC)==0x61,"Hide left blade keeps right blade native");
    HookHidden(blade[1],false,2);Check(Get<unsigned char>(blade[1],0x1CC)==0x61,"melee notify cannot re-show the selected hidden blade");
    dg::runtime::Get().strifeBladeVisibility=2;OnLivePlayer(horse,10,true);
    Check(Get<unsigned char>(blade[0],0x1CC)==0x61&&Get<unsigned char>(blade[1],0x1CC)==0x21,"Hide right blade restores latest native left intent");
    dg::runtime::Get().strifeBladeVisibility=3;OnLivePlayer(horse,10,true);
    Check(Get<unsigned char>(blade[0],0x1CC)==0x61&&Get<unsigned char>(blade[1],0x1CC)==0x61,"Hide both blades is independent from pistol choices");
    dg::runtime::Get().strifeBladeVisibility=0;OnLivePlayer(horse,10,true);
    Check(Get<unsigned char>(blade[0],0x1CC)==0x21&&Get<unsigned char>(blade[1],0x1CC)==0x21,"Native restores both blades independently");
    g_ready=false;
}
void AnimationBoundary(){
    using namespace dg::pistol_visual;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,48);
    g_ready=g_animationReady=true;g_players[0]={};g_players[1]={};
    unsigned char anim[0x500]{},blend[0x100]{},skeleton[0x100]{},seq[3][0x100]{},proxy[0x500]{};
    struct AssetSample {uintptr_t sequence;float position[3],rate;};AssetSample assetSamples[3]{};
    auto& p=g_players[0];auto& snapshot=p.snapshot;
    snapshot.actor=Register(actors[0],0);snapshot.mesh=Register(meshes[0],1);snapshot.skeleton=Register(skeletons[0],2);
    snapshot.present=snapshot.gameplay=snapshot.lower=true;snapshot.tick=1000;snapshot.worldTime=1;snapshot.speed=1;
    p.source.anim=Register(anim,24);p.source.blend=Register(blend,25);p.source.skeleton=Register(skeleton,26);
    for(unsigned i=0;i<3;++i){p.source.sequences[i]=Register(seq[i],27+int(i));assetSamples[i].sequence=p.source.sequences[i].address;}
    Put(blend,0xC8,Array{reinterpret_cast<uintptr_t>(assetSamples),3,3});
    p.source.lengths={.8666667f,2.6666667f,1.0666667f};
    std::array<uint16_t,80> required{};
    for(unsigned i=0;i<80;++i){required[i]=uint16_t(i);p.source.rig.parents[i]=i?0:-1;p.source.rig.inverseBind[i]=Translation(0,0,0);}
    const auto proxyAt=reinterpret_cast<uintptr_t>(proxy);Put(anim,0x240,proxyAt);Put(anim,0x318,g_base+0x2A77628);
    Put(proxy,0xA0,p.source.anim.address);Put(proxy,0xB0,p.source.skeleton.address);Put(proxy,0xB8,snapshot.mesh.address);
    Put(proxy,0x450,Array{reinterpret_cast<uintptr_t>(required.data()),80,80});
    PoseContext native{};native.proxy=proxyAt;native.pose.container=proxyAt+0x450;native.pose.transforms.count=80;
    const auto beforeNative=native;
    static std::array<LocalTransform,80> sampled{};
    static float animationPhase=0;static unsigned sampleCalls=0;sampleCalls=0;
    g_initContext=+[](PoseContext* context){context->pose.container=context->proxy+0x450;context->pose.transforms={reinterpret_cast<uintptr_t>(sampled.data()),80,80};};
    g_sample=+[](void*,const NativeArray* cache,CompactPose*,Curve*){
        ++sampleCalls;Check(cache->count>=1&&cache->count<=2,"bounded blend samples");
        const auto* input=reinterpret_cast<const BlendSample*>(cache->data);
        animationPhase=input[0].time;
        for(unsigned i=0;i<80;++i){sampled[i]={};sampled[i].q[3]=1;for(unsigned j=0;j<3;++j)sampled[i].s[j]=1;sampled[i].t[0]=float(i);}
        // Stand-in for the native clip changing its arm transform with time.
        sampled[41].t[1]=animationPhase*10;
    };
    dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;
    Check(SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1000)&&sampleCalls==1,"owned root samples live unarmed animation");
    const auto first=p.pose.relative[0][0];snapshot.tick=1050;snapshot.worldTime=1.05f;
    Check(SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050)&&sampleCalls==2&&
          !Equal(first,p.pose.relative[0][0]),"animation refresh changes arm instead of replaying frozen rest");
    Check(std::memcmp(&native,&beforeNative,sizeof(native))==0,"native output context and gameplay pose not overwritten");
    const auto n=sampleCalls;
    snapshot.suspended=true;Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050)&&sampleCalls==n,"suspended UI reuses private presentation without extra native evaluation");snapshot.suspended=false;
    snapshot.lower=false;Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050)&&sampleCalls==n,"melee and powers do not sample replacement arms");snapshot.lower=true;
    Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x320,&native,1050),"foreign animation root rejected");
    required[20]=19;Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050),"unsupported compact mapping keeps native pose");required[20]=20;
    Put(proxy,0xB8,uintptr_t(1));Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050),"foreign mesh proxy rejected");Put(proxy,0xB8,snapshot.mesh.address);
    native.additive=1;Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050),"additive context rejected");native.additive=0;
    snapshot.tick=700;Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050),"expired snapshot rejects native sampling");snapshot.tick=1050;
    Put(items,size_t(p.source.anim.index)*24+16,p.source.anim.serial+1);
    Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050),"animation address reuse rejected by serial");
    Put(items,size_t(p.source.anim.index)*24+16,p.source.anim.serial);
    dg::runtime::Get().pistolRightModel=0;
    Check(!SampleAnimation(reinterpret_cast<uintptr_t>(anim)+0x318,&native,1050)&&sampleCalls==n,"both Native never call extra pose evaluation");
    static unsigned forwards=0;g_evaluate=+[](void*,PoseContext*){++forwards;};HookEvaluate(nullptr,&native);
    Check(forwards==1,"every original root evaluation is retained");
    g_ready=g_animationReady=false;g_sample=nullptr;g_initContext=nullptr;g_evaluate=nullptr;
}
void Config(){
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"dgp",0,path),"create isolated INI fixture");
    dg::config::Store a;a.SetPath(path);Check(a.Load(),"load old INI fixture");
    Check(a.strifeBladeVisibility==0&&!a.mountedJumpEnabled,"old INI keeps blades native and mounted jump OFF");
    a.pistolRightModel=3;a.pistolLeftModel=1;a.strifeBladeVisibility=2;a.mountedJumpEnabled=true;a.mountedJumpSound=1;a.aimIndicatorHideMode=3;a.menuKey=VK_F10;const auto keys=a.hotkeys;Check(a.SaveNow(),"write OFF selection before disk round trip");
    dg::config::Store b;b.SetPath(path);
    Check(b.Load()&&b.pistolRightModel==3&&b.pistolLeftModel==1&&b.strifeBladeVisibility==2&&b.mountedJumpEnabled&&b.mountedJumpSound==1&&b.aimIndicatorHideMode==3&&b.menuKey==VK_F10&&b.hotkeys==keys,"OFF round trip preserves independent hand and personal bindings");
    Check(dg::runtime::Get().pistolRightModel.load()==3,"OFF is published to the runtime");
    WritePrivateProfileStringW(L"Values",L"PistolRightModel",L"4",path);
    Check(b.Load()&&b.pistolRightModel==0,"unknown future mode falls back to Native");
    WritePrivateProfileStringW(L"Values",L"StrifeBladeVisibility",L"9",path);
    Check(b.Load()&&b.strifeBladeVisibility==0,"invalid blade choice safely restores Native");
    WritePrivateProfileStringW(L"Values",L"MountedJumpSound",L"9",path);
    Check(b.Load()&&b.mountedJumpSound==0&&dg::runtime::Get().mountedJumpSound.load()==0,"unknown sound choice falls back to native samples");
    DeleteFileW(path);
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}
bool CinematicActive(unsigned){return cinematic;}bool RadialActive(unsigned){return radial;}
}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return true;}
bool ResolveActor(void* actor,Context& out){for(const auto& c:owners)if(c.pawn==reinterpret_cast<uintptr_t>(actor)){out=c;return true;}return false;}}
namespace dg::dash_combat {bool PresentationReady(){return presentation;}}
namespace dg::foreign_rampage {bool Evaluate(void*,pistol_visual::PoseContext*){return false;}}
int main(){try{Policy();Runtime();AnimationBoundary();Config();std::cout<<"Pistol visual native boundary checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
