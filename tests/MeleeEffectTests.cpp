#include "../src/MeleeEffectFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

namespace {
using namespace dg::melee_effect;
static_assert(sizeof(Plan)==128,"private per-notify plan remains compact without implicit alignment padding");
unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},asset[0x220]{},bones[44*12]{};
unsigned char notify[0x90]{},sequence[0x80]{},particle[0x80]{},package[0x80]{},cls[3][0x80]{},entries[20][100]{};
uintptr_t names[20]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},vt[0x400/8]{};
dg::local_players::Context owner{};bool gameThread=true,local=true;unsigned nativeNotifies=0,spawns=0,locations=0;
Transform sent{},source{};void* sentWorld=nullptr;void* sentAsset=nullptr;bool sentAuto=false;unsigned char sentPool=0;
bool attachCall=false;unsigned roots=0;Transform rootTransform{};
dg::native_bone::Point attachedPosition{},attachedRotation{11,22,33},attachedScale{2,3,4},sentPosition{},sentScale{};
const dg::native_bone::Point* sentRotation=nullptr;dg::native_bone::Name sentSocket{};int sentType=-1;
template<class T> void Put(void* p,size_t o,T value){std::memcpy(static_cast<unsigned char*>(p)+o,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* object,unsigned index,const char* text){
    Put(object,0x18,int(index));Put(object,0x1C,0);std::memset(entries[index],0,100);strcpy_s(reinterpret_cast<char*>(entries[index]+12),88,text);
}
dg::native_bone::Point* Location(void*,dg::native_bone::Point* out,dg::native_bone::Name name){
    ++locations;*out=name.index==9?dg::native_bone::Point{10,20,30}:dg::native_bone::Point{100,200,300};return out;
}
void* Spawn(void* world,void* fx,const Transform* transform,bool autoDestroy,unsigned char pool){
    ++spawns;sent=*transform;sentWorld=world;sentAsset=fx;sentAuto=autoDestroy;sentPool=pool;return reinterpret_cast<void*>(0xBEEF);
}
Transform* Root(void* m,Transform* out,dg::native_bone::Name socket,int space){
    Check(m==mesh&&socket.index==11&&!socket.number&&!space,"native world root transform contract");++roots;*out=rootTransform;return out;
}
void* Attach(void* fx,void* m,dg::native_bone::Name socket,const dg::native_bone::Point* location,
             const dg::native_bone::Point* rotation,const dg::native_bone::Point* scale,int type,bool autoDestroy,unsigned char pool){
    ++spawns;sentAsset=fx;sentWorld=m;sentSocket=socket;sentPosition=*location;sentRotation=rotation;sentScale=*scale;sentType=type;sentAuto=autoDestroy;sentPool=pool;
    return reinterpret_cast<void*>(0xCAFE);
}
void Notify(void* n,void* m,void* a){
    ++nativeNotifies;Check(n==notify&&m==mesh&&a==sequence,"original notify receives all three original arguments");
    if(attachCall)Check(HookAttach(particle,mesh,{11,0},&attachedPosition,&attachedRotation,&attachedScale,0,false,7)==reinterpret_cast<void*>(0xCAFE),"attached spawn return preserved");
    else Check(HookSpawn(reinterpret_cast<void*>(owner.world),particle,&source,false,7)==reinterpret_cast<void*>(0xBEEF),"spawn return preserved");
}
void Fresh(){
    nativeNotifies=spawns=locations=0;gameThread=local=true;g_ready=true;g_log=nullptr;g_spawn=Spawn;g_location=Location;g_plan=nullptr;
    attachCall=false;roots=0;g_attach=Attach;g_root=Root;
    owner={};owner.index=0;owner.localPlayer=0x2000;owner.world=0x3000;owner.controller=reinterpret_cast<uintptr_t>(controller);
    owner.character=owner.pawn=reinterpret_cast<uintptr_t>(actor);
    Put(actor,0x10,reinterpret_cast<uintptr_t>(cls[0]));Label(cls[0],0,"Strife_Blueprint_C");
    Put<unsigned char>(actor,0x118,3);Put<unsigned char>(actor,0xC18,1);Put(actor,0x348,owner.controller);Put(controller,0x350,owner.pawn);
    Put(actor,0x380,reinterpret_cast<uintptr_t>(mesh));Put(mesh,0xE8,owner.pawn);Put(mesh,0,reinterpret_cast<uintptr_t>(vt));vt[0x3F8/8]=g_base+0x17A2380;
    vt[0x3F0/8]=g_base+0x17A2590;
    Put(mesh,0x5A0,reinterpret_cast<uintptr_t>(asset));Put(asset,0x190,dg::native_bone::Array{reinterpret_cast<uintptr_t>(bones),44,44});
    Put(bones,43*12,dg::native_bone::Name{9,0});Put(bones,20*12,dg::native_bone::Name{10,0});
    Label(cls[1],1,"AnimNotify_PlayParticleEffect");Put(notify,0x10,reinterpret_cast<uintptr_t>(cls[1]));Put(notify,0x20,reinterpret_cast<uintptr_t>(sequence));
    Put(notify,0x38,reinterpret_cast<uintptr_t>(particle));Put(notify,0x40,dg::native_bone::Point{-30,-60,100});Put<unsigned char>(notify,0x80,0);
    Put(notify,0x84,dg::native_bone::Name{11,0});attachedPosition={-30,-60,100};attachedScale={2,3,4};
    Label(cls[2],2,"ParticleSystem");Put(particle,0x10,reinterpret_cast<uintptr_t>(cls[2]));Label(particle,3,"VFX_strife_meleeSlice");
    Put(particle,0x20,reinterpret_cast<uintptr_t>(package));Label(package,4,"/Game/Particles/VFX_strife_meleeSlice");Label(sequence,5,"Strife_Atk_Melee_01");
    source={};source.rotation[3]=1;source.position[0]=1;source.position[1]=2;source.position[2]=3;source.position[3]=77;
    source.scale[0]=2;source.scale[1]=3;source.scale[2]=4;source.scale[3]=88;dg::runtime::Get().strifeBladeVisibility=2;
    rootTransform={};rootTransform.rotation[2]=rootTransform.rotation[3]=std::sqrt(.5f);
    rootTransform.position[0]=1;rootTransform.position[1]=2;rootTransform.position[2]=3;
    rootTransform.scale[0]=2;rootTransform.scale[1]=3;rootTransform.scale[2]=4;
}
void Invoke(){const auto before=source;Particle(notify,mesh,sequence,Notify);Check(nativeNotifies==1&&spawns==1&&!g_plan,"one native notify/spawn with scope restored");
    Check(!std::memcmp(&source,&before,sizeof(source)),"caller transform remains unchanged");
    Check(sentWorld==reinterpret_cast<void*>(owner.world)&&sentAsset==particle&&!sentAuto&&sentPool==7,"all five spawn ABI arguments preserved");}
void NativeOnly(const char* why){Invoke();Check(!std::memcmp(&source,&sent,sizeof(source)),why);}
void InvokeAttached(){
    attachCall=true;const auto before=attachedPosition,beforeScale=attachedScale;Put<unsigned char>(notify,0x80,1);
    Particle(notify,mesh,sequence,Notify);
    Check(nativeNotifies==1&&spawns==1&&!g_plan,"attached path calls one original notify and emitter");
    Check(!std::memcmp(&before,&attachedPosition,sizeof(before))&&!std::memcmp(&beforeScale,&attachedScale,sizeof(beforeScale)),"attached caller vectors remain unchanged");
    Check(sentAsset==particle&&sentWorld==mesh&&sentSocket.index==11&&!sentSocket.number&&sentRotation==&attachedRotation&&sentType==0&&!sentAuto&&sentPool==7,"all nine attached ABI arguments and root orientation retained");
}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<20;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    strcpy_s(reinterpret_cast<char*>(entries[9]+12),88,"Bone_Strife_Hand_R");strcpy_s(reinterpret_cast<char*>(entries[10]+12),88,"Bone_Strife_Hand_L");
    strcpy_s(reinterpret_cast<char*>(entries[11]+12),88,"Bone_Strife_Root");
    Fresh();InvokeAttached();Check(roots==1&&Near(sentPosition.x,9)&&Near(sentPosition.y,-3)&&Near(sentPosition.z,6.75f)&&Near(sentScale.x,.8f),"attached right arc starts at hand under translated rotated nonuniform root");
    Fresh();dg::runtime::Get().strifeBladeVisibility=1;attachedPosition={0,-100,100};Put(notify,0x40,attachedPosition);owner.index=1;InvokeAttached();
    Check(Near(sentPosition.x,99)&&Near(sentPosition.y,-33)&&Near(sentPosition.z,74.25f),"attached left arc uses P2 owned left hand");
    Fresh();dg::runtime::Get().strifeBladeVisibility=0;InvokeAttached();Check(Near(sentPosition.x,-30)&&Near(sentScale.x,2)&&!roots,"visible blade attached arc remains native");
    for(int choice:{1,2,3}){Fresh();dg::runtime::Get().strifeBladeVisibility=choice;
        Label(sequence,5,"Strife_Atk_Melee_Kick_Launch_Level1_Montage");Put(notify,0x40,dg::native_bone::Point{100,0,160});
        Label(particle,3,"VFX_strife_meleeSlice_strong");Label(package,4,"/Game/Particles/VFX_strife_meleeSlice_strong");Invoke();
        Check(sent.scale[0]==.8f&&sent.position[0]==(choice==1?100:10),"secondary common launcher arc follows the selected bare hand or nearest hand when both blades are OFF");
    }
    for(const char* animation:{"Strife_Atk_WingClip_F","Strife_Atk_WingClip_B"})for(int choice:{1,2,3}){
        Fresh();Label(sequence,5,animation);unsigned char animationPackage[0x80]{};
        char path[160]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_Strife/Animations/%s",animation);Label(animationPackage,12,path);Put(sequence,0x20,reinterpret_cast<uintptr_t>(animationPackage));
        dg::runtime::Get().strifeBladeVisibility=choice;Put(notify,0x40,dg::native_bone::Point{0,60,100});
        source.position[0]=100;source.position[1]=200;source.position[2]=300;Invoke();
        Check(sent.scale[0]==(choice==2?2.f:.8f),"each spinning arc follows its closest actual hand; visible opposite blade is preserved");
        nativeNotifies=spawns=0;source.position[0]=10;source.position[1]=20;source.position[2]=30;Invoke();
        Check(sent.scale[0]==(choice==1?2.f:.8f),"mirrored spinning arc independently respects right OFF and both OFF");
        nativeNotifies=spawns=0;Label(animationPackage,12,"/Game/Other");Invoke();Check(sent.scale[0]==2,"same-name spinning asset outside original package stays native");
    }
    Fresh();vt[0x3F0/8]=0;InvokeAttached();Check(Near(sentPosition.x,-30)&&!roots,"unexpected root virtual forwards without native query");
    Fresh();rootTransform.scale[1]=0;InvokeAttached();Check(Near(sentPosition.x,-30)&&Near(sentScale.x,2),"singular root does not change an attached effect");
    Fresh();rootTransform.rotation[3]=2;InvokeAttached();Check(Near(sentPosition.x,-30),"invalid root quaternion stays native");
    Fresh();Put(notify,0x84,dg::native_bone::Name{10,0});InvokeAttached();Check(Near(sentPosition.x,-30)&&!roots,"non-root source socket excluded");
    Fresh();attachedPosition.x=123;InvokeAttached();Check(Near(sentPosition.x,123),"mismatched nested attached call excluded");
    Fresh();Invoke();Check(sent.position[0]==10&&sent.position[1]==20&&sent.position[2]==30&&sent.scale[0]==.8f,"right hidden blade moves its compact arc to right hand");
    Check(sent.position[3]==77&&sent.scale[3]==88&&sent.rotation[3]==1,"transform padding and rotation retained");
    Fresh();dg::runtime::Get().strifeBladeVisibility=1;NativeOnly("visible right blade arc is untouched");
    Fresh();Put(notify,0x40,dg::native_bone::Point{0,-100,100});dg::runtime::Get().strifeBladeVisibility=1;Invoke();Check(sent.position[0]==100,"left authored arc selects left hand only");
    Fresh();Label(sequence,5,"Strife_Atk_Melee_03");Put(notify,0x40,dg::native_bone::Point{0,-100,100});NativeOnly("shared arc is preserved while one blade remains visible");
    Fresh();Label(sequence,5,"Strife_Atk_Melee_03");Put(notify,0x40,dg::native_bone::Point{0,-100,100});dg::runtime::Get().strifeBladeVisibility=3;Invoke();Check(sent.position[0]==10,"both bare hands compact shared arc at nearest hand");
    Fresh();dg::runtime::Get().strifeBladeVisibility=0;NativeOnly("Native setting forwards without bone queries");Check(!locations,"no bone calls with blades visible");
    Fresh();gameThread=false;NativeOnly("off-thread callback stays native");
    Fresh();local=false;NativeOnly("unknown ownership stays native");
    Fresh();owner.index=1;Invoke();Check(sent.position[0]==10,"P2 uses the same owned-mesh contract");
    Fresh();owner.kind=dg::local_players::PoseKind::Horse;NativeOnly("mounted effects remain native");
    Fresh();owner.kind=dg::local_players::PoseKind::Ultimate;NativeOnly("ultimate effects remain native");
    Fresh();Put<unsigned char>(actor,0x118,2);NativeOnly("remote role rejected");
    Fresh();Put(controller,0x350,uintptr_t(0));NativeOnly("stale controller ownership rejected");
    Fresh();Put<unsigned char>(notify,0x80,1);NativeOnly("attached context cannot affect detached emitter");
    Fresh();Label(sequence,5,"OtherAttack");NativeOnly("unknown animation excluded");
    Fresh();Label(sequence,5,"Strife_Atk_Melee_03_b");Put(notify,0x40,dg::native_bone::Point{0,-200,100});dg::runtime::Get().strifeBladeVisibility=3;Invoke();Check(sent.position[0]==10,"authored third-combo variant is covered");
    Fresh();Label(package,4,"/Game/Other");NativeOnly("same named effect in another package excluded");
    Fresh();Put(notify,0x40,dg::native_bone::Point{0,0,0});NativeOnly("unknown authored offset excluded");
    Fresh();source.position[0]=std::numeric_limits<float>::quiet_NaN();NativeOnly("nonfinite transform forwarded unmodified");
    Fresh();source.rotation[3]=2;NativeOnly("invalid quaternion forwarded unmodified");
    Fresh();vt[0x3F8/8]=0;NativeOnly("unexpected native mesh virtual excluded");Check(!locations,"unverified socket function never called");
    Fresh();Put(bones,43*12,dg::native_bone::Name{10,0});NativeOnly("renamed or incompatible right bone excluded");
    Fresh();Shutdown();NativeOnly("shutdown immediately retains native notify and spawn");
    Check(Classify(2,{50,-180,100})==Side::Right&&Classify(4,{0,-70,100})==Side::Shared&&Classify(5,{0,-150,90})==Side::Shared,"ground and air authored contracts classified");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=owner;return local;}}
int main(){try{Run();std::cout<<"Owned bare-blade particle scope, per-hand selection and native spawn ABI passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
