#include "GeyserStaticPose.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "PistolAnimationPolicy.h"
#include <atomic>
#include <cmath>
#include <algorithm>
namespace dg::geyser_static {
namespace {
using Id=native_objects::Identity;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Vector {float x=0,y=0,z=0;};
struct Extract {bool root=true;unsigned char pad[3]{};float time=0;};
using Sample=void(*)(void*,pistol_visual::CompactPose*,pistol_visual::Curve*,const Extract*);
using InitContext=void(*)(pistol_visual::PoseContext*);
Sample g_sample=nullptr;InitContext g_initContext=nullptr;uintptr_t g_base=0;Id g_return{};std::atomic_uint g_mask{0};SRWLOCK g_lock=SRWLOCK_INIT;
foreign_wrath::LogFn g_log=nullptr;std::atomic_uint g_samples{0};
constexpr float ReturnStart=.85f,ReturnSpan=1.15f,ReturnBlend=.22f;
struct Visual {Id actor{},ability{},mesh{},anim{},skeleton{},montage{},movement{};uintptr_t world=0;int input=-1,lastInput=-1,velocity=-1,acceleration=-1;float position=0,weight=0;uint64_t tick=0;};
Visual g_players[2]{};
template<class T>bool Read(uintptr_t p,size_t at,T& out){native_objects::MemoryReader r;return r.Read(p,at,out);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Field(uintptr_t object,const char* name,int& at){uintptr_t cls=0;return Read(object,0x10,cls)&&native_objects::PropertyField(g_base,cls,name,"StructProperty",12,at);}
bool Still(uintptr_t p,int at,float squareLimit){
    Vector v{};return at>=0&&Read(p,size_t(at),v)&&std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z)&&v.x*v.x+v.y*v.y+v.z*v.z<=squareLimit;
}
void Clear(int index){AcquireSRWLockExclusive(&g_lock);g_players[index]={};g_mask.fetch_and(~(1u<<index),std::memory_order_relaxed);ReleaseSRWLockExclusive(&g_lock);}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char sample[]={0x48,0x89,0x74,0x24,0x18,0x48,0x89,0x7C,0x24,0x20,0x55,0x41,0x54,0x41,0x56,0x48};
    constexpr unsigned char context[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x48,0x89,0x7C,0x24,0x18};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x1690BD0),sample,sizeof(sample))&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x1694890),context,sizeof(context))){
        g_sample=reinterpret_cast<Sample>(g_base+0x1690BD0);g_initContext=reinterpret_cast<InitContext>(g_base+0x1694890);
    }
}
void ObserveSequence(uintptr_t p){
    if(!g_sample||!native_tps::OnGameThread()||Alive(g_return))return;
    uintptr_t cls=0,outer=0,skeleton=0;float length=0,rate=0;unsigned char root=255;
    if(Is(p,"Strife_Aether_Channel_End")&&Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Read(p,0x20,outer)&&
       Is(outer,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Aether_Channel_End")&&Read(p,0x38,skeleton)&&Is(skeleton,"Strife_Skeleton")&&
       Read(p,0x90,length)&&std::isfinite(length)&&std::fabs(length-.8333333135f)<.00001f&&Read(p,0x94,rate)&&rate==1&&Read(p,0x1A1,root)&&!root)Capture(p,g_return);
}
bool AssetsReady(){return Alive(g_return);}
bool InvalidateAssets(){if(g_return&&!Alive(g_return)){g_return={};return true;}return false;}
void Begin(const local_players::Context& c,void* ability,void* montage){
    if(!g_sample||!native_tps::OnGameThread()||c.index<0||c.index>1)return;Clear(c.index);Visual v{};uintptr_t mesh=0,anim=0,movement=0,skeleton=0,p=0;unsigned char role=0;
    if(!c.localPlayer||!c.world||!c.controller||c.kind!=local_players::PoseKind::Foot||c.pawn!=c.character||!Read(c.character,0x118,role)||role!=3||
       !Read(c.character,0x348,p)||p!=c.controller||!Read(c.controller,0x350,p)||p!=c.character||!AssetsReady()||!Capture(c.character,v.actor)||!Capture(reinterpret_cast<uintptr_t>(ability),v.ability)||
       !Capture(reinterpret_cast<uintptr_t>(montage),v.montage)||!Read(c.character,0x380,mesh)||!Capture(mesh,v.mesh)||!Read(mesh,0xE8,p)||p!=c.character||!Read(mesh,0x718,anim)||!Capture(anim,v.anim)||
       !Read(anim,0x28,skeleton)||!Capture(skeleton,v.skeleton)||!Read(g_return.address,0x38,skeleton)||skeleton!=v.skeleton.address||
       !Read(c.character,0x388,movement)||!Capture(movement,v.movement)||!Field(c.character,"ControlInputVector",v.input)||!Field(c.character,"LastControlInputVector",v.lastInput)||
       !Field(movement,"Velocity",v.velocity)||!Field(movement,"Acceleration",v.acceleration))return;
    v.world=c.world;AcquireSRWLockExclusive(&g_lock);g_players[c.index]=v;g_mask.fetch_or(1u<<c.index,std::memory_order_relaxed);ReleaseSRWLockExclusive(&g_lock);
}
void Observe(const local_players::Context& c){
    if(!g_sample||!native_tps::OnGameThread()||c.index<0||c.index>1||!(g_mask.load(std::memory_order_relaxed)&(1u<<c.index)))return;
    Visual v{};AcquireSRWLockShared(&g_lock);v=g_players[c.index];ReleaseSRWLockShared(&g_lock);
    uintptr_t p=0,mesh=0,anim=0;unsigned char state=0,mode=255;Array instances{};bool found=false;
    if(c.kind!=local_players::PoseKind::Foot||c.pawn!=c.character||v.actor.address!=c.character||v.world!=c.world||!Alive(v.actor)||!Alive(v.ability)||!Alive(v.mesh)||!Alive(v.anim)||!Alive(v.montage)||!Alive(v.movement)||
       !Read(v.ability.address,0x48,p)||p!=c.character||!Read(v.ability.address,0xD8,state)||(state!=1&&state!=2)||
       !Read(c.character,0x380,mesh)||mesh!=v.mesh.address||!Read(mesh,0xE8,p)||p!=c.character||!Read(mesh,0x718,anim)||anim!=v.anim.address||!Read(v.movement.address,0xE8,p)||p!=c.character||
       !Read(anim,0x78,instances)||instances.count<0||instances.count>instances.capacity||instances.capacity>32||(instances.count&&!instances.data)){Clear(c.index);return;}
    for(int i=0;i<instances.count;++i){uintptr_t instance=0,montage=0;
        if(Read(instances.data,size_t(i)*8,instance)&&Read(instance,0,montage)&&montage==v.montage.address&&Read(instance,0x128,v.position)&&Read(instance,0x148,v.weight)&&
           std::isfinite(v.position)&&v.position>=0&&v.position<=2.367f&&std::isfinite(v.weight)&&v.weight>=0&&v.weight<=1.001f){found=true;break;}
    }
    if(!found){Clear(c.index);return;}
    // Never change the montage/ability or write movement/jump state. A held
    // direction against a wall is movement intent too, even with zero speed.
    if(!Read(v.movement.address,0x1B0,mode)||mode!=1||!Still(c.character,v.input,.000001f)||!Still(c.character,v.lastInput,.000001f)||
       !Still(v.movement.address,v.velocity,1)||!Still(v.movement.address,v.acceleration,1))v.weight=0;
    const float fade=std::clamp((v.position-ReturnStart)/ReturnBlend,0.f,1.f);
    v.weight=std::clamp(v.weight,0.f,1.f)*fade*fade*(3-2*fade);v.tick=GetTickCount64();
    AcquireSRWLockExclusive(&g_lock);g_players[c.index]=v;ReleaseSRWLockExclusive(&g_lock);
}
bool Evaluate(void* node,pistol_visual::PoseContext* output){
    if(!g_sample||!g_initContext||!g_mask.load(std::memory_order_relaxed)||!output)return false;Visual v{};const auto now=GetTickCount64();
    AcquireSRWLockShared(&g_lock);for(const auto& p:g_players)if(p.anim&&reinterpret_cast<uintptr_t>(node)==p.anim.address+0x318){v=p;break;}ReleaseSRWLockShared(&g_lock);
    if(!v.tick||now<v.tick||now-v.tick>100||v.weight<=0||!Alive(v.actor)||!Alive(v.mesh)||!Alive(v.anim)||!Alive(v.skeleton)||!Alive(v.montage)||!AssetsReady())return false;
    uintptr_t proxy=0,p=0,vt=0;Array bones{};pistol_visual::PoseContext context{};float length=0;
    if(!Read(reinterpret_cast<uintptr_t>(node),0,vt)||vt!=g_base+0x2A77628||!Read(reinterpret_cast<uintptr_t>(output),0,context)||context.additive||
       !Read(v.anim.address,0x240,proxy)||context.proxy!=proxy||!Read(proxy,0xA0,p)||p!=v.anim.address||!Read(proxy,0xB8,p)||p!=v.mesh.address||!Read(proxy,0xB0,p)||p!=v.skeleton.address||
       context.pose.container!=proxy+0x450||context.pose.transforms.count!=80||context.pose.transforms.capacity<80||context.pose.transforms.capacity>1024||!context.pose.transforms.data||(context.pose.transforms.data&15u)||
       !Read(proxy,0x450,bones)||!bones.data||bones.count!=80||bones.capacity<80||bones.capacity>1024||!Read(g_return.address,0x38,p)||p!=v.skeleton.address||
       !Read(g_return.address,0x90,length)||!std::isfinite(length)||length<=0||length>1)return false;
    std::array<uint16_t,80> indices{};std::array<pistol_visual::LocalTransform,80> original{};
    if(!Read(bones.data,0,indices)||!Read(context.pose.transforms.data,0,original))return false;for(unsigned i=0;i<80;++i)if(indices[i]!=i)return false;
    // Same native MemStack context as the accepted pistol-arm sampler. Both
    // transforms AND curve buffers are private to this evaluation scope.
    // Never clone an owning TArray header or modify the live curve values.
    pistol_visual::PoseContext sampled{};sampled.proxy=proxy;g_initContext(&sampled);
    if(sampled.pose.container!=context.pose.container||sampled.pose.transforms.count!=80||sampled.pose.transforms.capacity<80||
       sampled.pose.transforms.capacity>1024||!sampled.pose.transforms.data||(sampled.pose.transforms.data&15u)||
       sampled.pose.transforms.data==context.pose.transforms.data||
       (context.curve.values.data&&sampled.curve.values.data==context.curve.values.data))return false;
    const Extract extract{true,{},std::clamp((v.position-ReturnStart)*length/ReturnSpan,0.f,length)};
    g_sample(reinterpret_cast<void*>(g_return.address),&sampled.pose,&sampled.curve,&extract);
    std::array<pistol_visual::LocalTransform,80> returned{};
    if(sampled.pose.transforms.count!=80||!Read(sampled.pose.transforms.data,0,returned))return false;
    auto* pose=reinterpret_cast<pistol_visual::LocalTransform*>(context.pose.transforms.data);
    for(unsigned i=1;i<80;++i){const auto& a=original[i];auto b=returned[i];float dot=0,norm=0;
        for(unsigned j=0;j<4;++j)dot+=a.q[j]*b.q[j];const float sign=dot<0?-1.f:1.f;
        for(unsigned j=0;j<4;++j){b.q[j]=a.q[j]*(1-v.weight)+sign*b.q[j]*v.weight;norm+=b.q[j]*b.q[j];}
        if(norm>1.e-12f&&std::isfinite(norm)){const float inv=1/std::sqrt(norm);for(auto& q:b.q)q*=inv;}else std::memcpy(b.q,a.q,sizeof(b.q));
        for(unsigned j=0;j<3;++j){b.t[j]=a.t[j]+(b.t[j]-a.t[j])*v.weight;b.s[j]=a.s[j]+(b.s[j]-a.s[j])*v.weight;}
        pose[i]=b;
    }
    if(g_log&&g_samples.fetch_add(1)==0)g_log("Geyser V1.119 static return: 1.15s clip (+0.50s), smooth 0.22s blend; live curves, root and gameplay untouched; EXPERIMENTAL");return true;
}
void Shutdown(){Clear(0);Clear(1);g_sample=nullptr;g_initContext=nullptr;}
}
