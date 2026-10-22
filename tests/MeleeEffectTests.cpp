#include "../src/MeleeEffectFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

namespace {
using namespace dg::melee_effect;
unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},asset[0x220]{},bones[44*12]{};
unsigned char notify[0x90]{},sequence[0x80]{},particle[0x80]{},package[0x80]{},cls[3][0x80]{},entries[20][100]{};
uintptr_t names[20]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},vt[0x400/8]{};
dg::local_players::Context owner{};bool gameThread=true,local=true;unsigned nativeNotifies=0,spawns=0,locations=0;
Transform sent{},source{};void* sentWorld=nullptr;void* sentAsset=nullptr;bool sentAuto=false;unsigned char sentPool=0;
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
void Notify(void* n,void* m,void* a){
    ++nativeNotifies;Check(n==notify&&m==mesh&&a==sequence,"original notify receives all three original arguments");
    Check(HookSpawn(reinterpret_cast<void*>(owner.world),particle,&source,false,7)==reinterpret_cast<void*>(0xBEEF),"spawn return preserved");
}
void Fresh(){
    nativeNotifies=spawns=locations=0;gameThread=local=true;g_ready=true;g_log=nullptr;g_spawn=Spawn;g_location=Location;g_plan=nullptr;
    owner={};owner.index=0;owner.localPlayer=0x2000;owner.world=0x3000;owner.controller=reinterpret_cast<uintptr_t>(controller);
    owner.character=owner.pawn=reinterpret_cast<uintptr_t>(actor);
    Put(actor,0x10,reinterpret_cast<uintptr_t>(cls[0]));Label(cls[0],0,"Strife_Blueprint_C");
    Put<unsigned char>(actor,0x118,3);Put<unsigned char>(actor,0xC18,1);Put(actor,0x348,owner.controller);Put(controller,0x350,owner.pawn);
    Put(actor,0x380,reinterpret_cast<uintptr_t>(mesh));Put(mesh,0xE8,owner.pawn);Put(mesh,0,reinterpret_cast<uintptr_t>(vt));vt[0x3F8/8]=g_base+0x17A2380;
    Put(mesh,0x5A0,reinterpret_cast<uintptr_t>(asset));Put(asset,0x190,dg::native_bone::Array{reinterpret_cast<uintptr_t>(bones),44,44});
    Put(bones,43*12,dg::native_bone::Name{9,0});Put(bones,20*12,dg::native_bone::Name{10,0});
    Label(cls[1],1,"AnimNotify_PlayParticleEffect");Put(notify,0x10,reinterpret_cast<uintptr_t>(cls[1]));Put(notify,0x20,reinterpret_cast<uintptr_t>(sequence));
    Put(notify,0x38,reinterpret_cast<uintptr_t>(particle));Put(notify,0x40,dg::native_bone::Point{-30,-60,100});Put<unsigned char>(notify,0x80,0);
    Label(cls[2],2,"ParticleSystem");Put(particle,0x10,reinterpret_cast<uintptr_t>(cls[2]));Label(particle,3,"VFX_strife_meleeSlice");
    Put(particle,0x20,reinterpret_cast<uintptr_t>(package));Label(package,4,"/Game/Particles/VFX_strife_meleeSlice");Label(sequence,5,"Strife_Atk_Melee_01");
    source={};source.rotation[3]=1;source.position[0]=1;source.position[1]=2;source.position[2]=3;source.position[3]=77;
    source.scale[0]=2;source.scale[1]=3;source.scale[2]=4;source.scale[3]=88;dg::runtime::Get().strifeBladeVisibility=2;
}
void Invoke(){const auto before=source;Particle(notify,mesh,sequence,Notify);Check(nativeNotifies==1&&spawns==1&&!g_plan,"one native notify/spawn with scope restored");
    Check(!std::memcmp(&source,&before,sizeof(source)),"caller transform remains unchanged");
    Check(sentWorld==reinterpret_cast<void*>(owner.world)&&sentAsset==particle&&!sentAuto&&sentPool==7,"all five spawn ABI arguments preserved");}
void NativeOnly(const char* why){Invoke();Check(!std::memcmp(&source,&sent,sizeof(source)),why);}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<20;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    strcpy_s(reinterpret_cast<char*>(entries[9]+12),88,"Bone_Strife_Hand_R");strcpy_s(reinterpret_cast<char*>(entries[10]+12),88,"Bone_Strife_Hand_L");
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
    Fresh();Put<unsigned char>(notify,0x80,1);NativeOnly("attached effect excluded");
    Fresh();Label(sequence,5,"OtherAttack");NativeOnly("unknown animation excluded");
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
