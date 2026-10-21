#include "../src/PistolVisualFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <limits>
namespace {
using dg::pistol_visual::Matrix;using dg::pistol_visual::Matrices;using dg::pistol_visual::Transform;
unsigned char actors[2][0x1100]{},meshes[2][0x800]{},skeletons[2][0x400]{},renderObjects[2][0x80]{};
unsigned char packets[2][0x110]{},guns[2][2][0x600]{},items[24*24]{},names[24][128]{},notify[0x100]{},klass[0x40]{},effect[0x40]{};
uintptr_t nameEntries[24]{},nameChunks[1]{reinterpret_cast<uintptr_t>(nameEntries)};void* chunks[1]{items};
Matrices current[2]{},previous[2]{};std::array<Transform,80> poses[2]{};
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
    Transform t{};t.q[3]=1;t.s[0]=t.s[1]=t.s[2]=1;t.t[0]=12;t.t[1]=-5;
    Matrix m{},inverse{};Check(ToMatrix(t,m)&&Inverse(m,inverse),"native FTransform conversion/inverse");
    Check(Equal(Multiply(m,inverse),Translation(0,0,0)),"row-vector inverse cancels translation");
    t.q[2]=.70710678f;t.q[3]=.70710678f;t.s[0]=2;t.s[1]=3;t.s[2]=4;
    Check(ToMatrix(t,m)&&Inverse(m,inverse)&&Equal(Multiply(m,inverse),Translation(0,0,0)),"rotation and nonuniform scale invert");
    Matrix singular{};Check(!Inverse(singular,inverse),"singular matrix rejected");
    t.q[3]=std::numeric_limits<float>::quiet_NaN();Check(!ToMatrix(t,m),"invalid native transform rejected");
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
    const auto last=output;clav[0]=Translation(25,7,0);output=source;old=source;
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
}
void Runtime(){
    using namespace dg::pistol_visual;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,24);
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
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
        s.tick=1000;s.idleSince=100;s.capture=s.gameplay=true;
        Put(meshes[p],0xE8,s.actor.address);Put(meshes[p],0x5A0,s.skeleton.address);Put(meshes[p],0x6C0,reinterpret_cast<uintptr_t>(renderObjects[p]));
        Put(renderObjects[p],0,g_base+0x2DC8930);Put(renderObjects[p],0x40,uintptr_t(0x1234));
        Put(packets[p],0,g_base+0x2DC88A8);Put(packets[p],0x10,Array{reinterpret_cast<uintptr_t>(&current[p]),80,80});
        Put(packets[p],0x20,Array{reinterpret_cast<uintptr_t>(&previous[p]),80,80});
        Put(meshes[p],0x5A8,std::array<int,2>{-1,0});Put(meshes[p],0x5F4,0);
        Put(meshes[p],0x5B0,Array{reinterpret_cast<uintptr_t>(&poses[p]),80,80});
        for(unsigned i=0;i<80;++i){current[p][i]=Translation(float(i),float(p),5);poses[p][i].q[3]=1;poses[p][i].s[0]=poses[p][i].s[1]=poses[p][i].s[2]=1;}
        previous[p]=current[p];poses[p][14].t[0]=10;poses[p][17].t[0]=-10;
        for(unsigned side=0;side<2;++side){
            auto gun=Register(guns[p][side],int(p*5+3+side));
            Put(actors[p],0x1040+side*8,gun.address);Put(guns[p][side],0,g_base+0x2A575C8);
            Put(guns[p][side],0xE8,s.actor.address);Put(guns[p][side],0x108,s.mesh.address);Put<unsigned char>(guns[p][side],0x1CC,0x21);
        }
        Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[p]),s.mesh.address,0x1234,0,1000)&&g_poseReady[p],"each owned render packet captures a separate sheath pose");
    }
    const auto nativePose=poses[0];
    std::array<unsigned char,sizeof(actors)> beforeActors{};std::memcpy(beforeActors.data(),actors,sizeof(actors));
    const auto baseline=current[0];auto& s=g_players[0].snapshot;s.capture=false;s.lower=true;s.tick=1100;
    dg::runtime::Get().pistolRightModel=3;poses[0][14].t[0]=20;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"render packet receives scoped OFF pose");
    for(unsigned i=0;i<80;++i)if(i!=79)Check(Equal(current[0][i],i>=40&&i<=62?Translation(float(i)+10,0,5):baseline[i]),"right OFF includes aiming shoulder but preserves opposite arm and spine");
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j)Check(current[0][79].m[i][j]==0,"only OFF-side holster collapsed in runtime packet");
    auto expectedPose=nativePose;expectedPose[14].t[0]=20;
    Check(std::memcmp(&poses[0],&expectedPose,sizeof(expectedPose))==0&&std::memcmp(beforeActors.data(),actors,sizeof(actors))==0,"native poses, action bytes, firing actors and sockets never written");
    Check(Equal(current[1][41],Translation(41,1,5)),"P1 render packet cannot alter P2");
    current[0]=baseline;previous[0]=baseline;poses[0][17].t[0]=500;
    dg::runtime::Get().pistolRightModel=0;dg::runtime::Get().pistolLeftModel=3;
    Check(ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1100),"left OFF uses the same complete branch solve");
    for(unsigned i=17;i<=39;++i)Check(Equal(current[0][i],Translation(float(i)+10,0,5)),"animated left aiming clavicle cannot pull the resting arm off its spine anchor");
    Check(poses[0][17].t[0]==500,"native aiming clavicle and firing sockets remain untouched");poses[0][17].t[0]=-10;
    dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;
    const auto result=current[0];s.tick=1000;
    Check(!ProcessPacket(reinterpret_cast<uintptr_t>(packets[0]),s.mesh.address,0x1234,0,1400)&&std::memcmp(&result,&current[0],sizeof(result))==0,"expired owner fails open without render write");
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
    Hide(0,0,s);Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"hidden flag preserves native visibility and unrelated bits");
    HookHidden(guns[0][0],true,1);Restore(g_hidden[0][0]);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"foreign native hide supersedes our original baseline");
    HookHidden(guns[0][0],false,1);Hide(0,0,s);HookHidden(guns[0][0],false,1);Hide(0,0,s);Restore(g_hidden[0][0]);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x21,"return to Native restores latest foreign visibility intent");
    Name(0,"AnimNotify_PlayParticleEffect");Name(1,"VFX_strife_muzzleFlash");Name(2,"VFX_Weapon_Gun_R");Name(3,"VFX_Weapon_Gun_L");Name(4,"VFX_Root_Beam");Name(5,"unrelated_hit_effect");
    Put(klass,0x18,0);Put(notify,0x10,reinterpret_cast<uintptr_t>(klass));Put(effect,0x18,1);Put(notify,0x38,reinterpret_cast<uintptr_t>(effect));
    const auto now=GetTickCount64();s.tick=now;g_players[0].renderTick=now;Put(notify,0x84,std::array<int,2>{2,0});
    Check(SkipParticle(notify,meshes[0]),"owned right muzzle suppressed");Put(notify,0x84,std::array<int,2>{3,0});
    Check(!SkipParticle(notify,meshes[0]),"visible left muzzle preserved");Put(notify,0x84,std::array<int,2>{4,0});
    Check(!SkipParticle(notify,meshes[0]),"shared root effect preserved with visible hand");dg::runtime::Get().pistolLeftModel=3;
    Check(SkipParticle(notify,meshes[0]),"shared root effect suppressed only with both OFF");Put(effect,0x18,5);
    Check(!SkipParticle(notify,meshes[0]),"unrelated effect never suppressed");Put(effect,0x18,1);
    Name(6,"AnimNotify_CallAbilityFunction");Put(klass,0x18,6);
    Check(!SkipParticle(notify,meshes[0]),"World Ender lifecycle notification is never a muzzle particle");Put(klass,0x18,0);gameThread=false;
    Check(!SkipParticle(notify,meshes[0]),"notify outside game thread retained");gameThread=true;
    dg::local_players::Context horse{};horse.index=0;horse.kind=dg::local_players::PoseKind::Horse;
    Hide(0,0,s);OnLivePlayer(horse,9,true);
    Check(!PoseReady(0)&&Get<unsigned char>(guns[0][0],0x1CC)==0x21,"leaving normal Strife restores model and discards pose");
    unsigned char horseActor[0xC10]{},horseMesh[0x800]{},horseGuns[4][0x600]{},pc[0x360]{},strifeClass[0x40]{},horseClass[0x40]{};
    const auto horseId=Register(horseActor,10),horseMeshId=Register(horseMesh,11);
    Name(7,"Strife_Blueprint_C");Name(8,"Despair_Blueprint_C");Put(strifeClass,0x18,7);Put(horseClass,0x18,8);
    Put(actors[0],0x10,reinterpret_cast<uintptr_t>(strifeClass));Put<unsigned char>(actors[0],0xC18,1);
    Put(horseActor,0x10,reinterpret_cast<uintptr_t>(horseClass));Put<unsigned char>(horseActor,0x118,3);
    Put(horseActor,0x348,reinterpret_cast<uintptr_t>(pc));Put(pc,0x350,horseId.address);
    Put(horseActor,0x380,horseMeshId.address);Put(horseMesh,0xE8,horseId.address);
    horse.localPlayer=1;horse.controller=reinterpret_cast<uintptr_t>(pc);horse.character=reinterpret_cast<uintptr_t>(actors[0]);horse.pawn=horseId.address;
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
    const auto events=hiddenEvents.size();HookHidden(horseGuns[0],false,2);
    Check(hiddenEvents.size()==events+2&&hiddenEvents[events]==std::pair<bool,unsigned char>{false,2}&&
          hiddenEvents[events+1]==std::pair<bool,unsigned char>{true,1}&&Get<unsigned char>(horseGuns[0],0x1CC)==0x61,
          "native visibility and child propagation forwarded before exact OFF component is retained in same call");
    paused=radial=false;
    HookHidden(horseGuns[0],true,1);dg::runtime::Get().pistolRightModel=0;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[0],0x1CC)==0x61&&Get<unsigned char>(horseGuns[2],0x1CC)==0x21,"Native restores latest native mounted visibility independently per component");
    HookHidden(horseGuns[0],false,1);dg::runtime::Get().pistolLeftModel=3;OnLivePlayer(horse,9,true);
    Check(Get<unsigned char>(horseGuns[1],0x1CC)==0x61&&Get<unsigned char>(horseGuns[3],0x1CC)==0x61,"left mounted OFF uses left offsets only");
    OnLivePlayer(horse,9,false);Check(Get<unsigned char>(horseGuns[1],0x1CC)==0x21&&Get<unsigned char>(horseGuns[3],0x1CC)==0x21,"focus loss restores mounted native visibility");
    const auto n=hiddenCalls;Put(horseActor,0x348,uintptr_t(0));OnLivePlayer(horse,9,true);
    Check(hiddenCalls==n,"foreign or stale mounted controller cannot hide a component");
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
    horse.kind=dg::local_players::PoseKind::Foot;horse.pawn=horse.character;dg::runtime::Get().pistolRightModel=3;dg::runtime::Get().pistolLeftModel=0;
    OnLivePlayer(horse,10,true);
    Check(!PoseReady(0)&&Get<unsigned char>(guns[0][0],0x1CC)==0x61,"return to normal form keeps OFF before pose capture is ready");
    paused=radial=true;OnLivePlayer(horse,10,true);HookHidden(guns[0][0],false,1);
    Check(Get<unsigned char>(guns[0][0],0x1CC)==0x61,"normal-foot wheel cannot briefly show the hidden pistol");
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
void Config(){
    wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"dgp",0,path),"create isolated INI fixture");
    dg::config::Store a;a.SetPath(path);Check(a.Load(),"load old INI fixture");
    Check(a.strifeBladeVisibility==0&&!a.mountedJumpEnabled,"old INI keeps blades native and mounted jump OFF");
    a.pistolRightModel=3;a.pistolLeftModel=1;a.strifeBladeVisibility=2;a.mountedJumpEnabled=true;a.aimIndicatorHideMode=3;a.menuKey=VK_F10;const auto keys=a.hotkeys;Check(a.SaveNow(),"write OFF selection before disk round trip");
    dg::config::Store b;b.SetPath(path);
    Check(b.Load()&&b.pistolRightModel==3&&b.pistolLeftModel==1&&b.strifeBladeVisibility==2&&b.mountedJumpEnabled&&b.aimIndicatorHideMode==3&&b.menuKey==VK_F10&&b.hotkeys==keys,"OFF round trip preserves independent hand and personal bindings");
    Check(dg::runtime::Get().pistolRightModel.load()==3,"OFF is published to the runtime");
    WritePrivateProfileStringW(L"Values",L"PistolRightModel",L"4",path);
    Check(b.Load()&&b.pistolRightModel==0,"unknown future mode falls back to Native");
    WritePrivateProfileStringW(L"Values",L"StrifeBladeVisibility",L"9",path);
    Check(b.Load()&&b.strifeBladeVisibility==0,"invalid blade choice safely restores Native");
    DeleteFileW(path);
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}
bool CinematicActive(unsigned){return cinematic;}bool RadialActive(unsigned){return radial;}
}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return true;}}
namespace dg::dash_combat {bool PresentationReady(){return presentation;}}
int main(){try{Policy();Runtime();Config();std::cout<<"Pistol visual native boundary checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
