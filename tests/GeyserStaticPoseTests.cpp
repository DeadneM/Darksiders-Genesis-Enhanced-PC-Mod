#include "../src/GeyserStaticPose.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::geyser_static;
struct Fixture {unsigned char actor[0x600]{},ability[0x200]{},mesh[0x800]{},anim[0x900]{},skeleton[0x200]{},montage[0x200]{},movement[0x300]{},sequence[0x200]{},instance[0x160]{},proxy[0x600]{},items[16*24]{};uintptr_t instanceList[1]{};uint16_t bones[80]{};std::array<dg::pistol_visual::LocalTransform,80> pose{};} f;
uintptr_t chunks[1]{};dg::local_players::Context owner{};unsigned calls=0;bool thread=true;dg::pistol_visual::Curve* liveCurve=nullptr;
template<class T>void Put(void* p,size_t at,const T& v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
Id Live(void* p,int i){Put(p,0xC,i);Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(i)*24+16,i+100);return {reinterpret_cast<uintptr_t>(p),i,i+100};}
void SampleNative(void* sequence,dg::pistol_visual::CompactPose*,dg::pistol_visual::Curve* curve,const Extract* e){Check(sequence==f.sequence&&e->root&&std::fabs(e->time-.4f)<.0001f&&curve!=liveCurve,"pure return extraction uses a private curve header and native animation time");++calls;f.pose[0].t[0]=999;f.pose[1].t[0]=20;}
dg::pistol_visual::PoseContext Setup(){
 f={};calls=0;thread=true;g_sample=SampleNative;g_mask=1;owner={};owner.index=0;owner.world=1;owner.localPlayer=2;owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);owner.controller=3;
 Visual v{};v.actor=Live(f.actor,0);v.ability=Live(f.ability,1);v.mesh=Live(f.mesh,2);v.anim=Live(f.anim,3);v.skeleton=Live(f.skeleton,4);v.montage=Live(f.montage,5);v.movement=Live(f.movement,6);g_return=Live(f.sequence,7);v.world=1;v.input=0x500;v.lastInput=0x50C;v.velocity=0xD8;v.acceleration=0x188;g_players[0]=v;g_players[1]={};
 chunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(reinterpret_cast<void*>(g_base),0x3959694,16);
 Put(f.actor,0x380,v.mesh.address);Put(f.mesh,0xE8,v.actor.address);Put(f.mesh,0x718,v.anim.address);Put(f.movement,0xE8,v.actor.address);Put<unsigned char>(f.movement,0x1B0,1);Put(f.ability,0x48,v.actor.address);Put<unsigned char>(f.ability,0xD8,2);
 f.instanceList[0]=reinterpret_cast<uintptr_t>(f.instance);Put(f.anim,0x78,Array{reinterpret_cast<uintptr_t>(f.instanceList),1,1});Put(f.instance,0,v.montage.address);Put(f.instance,0x128,1.4f);Put(f.instance,0x148,1.f);
 Put(f.anim,0x318,g_base+0x2A77628);Put(f.anim,0x240,reinterpret_cast<uintptr_t>(f.proxy));Put(f.proxy,0xA0,v.anim.address);Put(f.proxy,0xB8,v.mesh.address);Put(f.proxy,0xB0,v.skeleton.address);Put(f.sequence,0x38,v.skeleton.address);Put(f.sequence,0x90,.8333333135f);
 for(unsigned i=0;i<80;++i){f.bones[i]=uint16_t(i);f.pose[i].q[3]=1;f.pose[i].s[0]=f.pose[i].s[1]=f.pose[i].s[2]=1;}f.pose[0].t[0]=13;f.pose[1].t[0]=10;
 Put(f.proxy,0x450,Array{reinterpret_cast<uintptr_t>(f.bones),80,80});dg::pistol_visual::PoseContext out{};out.proxy=reinterpret_cast<uintptr_t>(f.proxy);out.pose.container=out.proxy+0x450;out.pose.transforms={reinterpret_cast<uintptr_t>(f.pose.data()),80,80};return out;
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());auto out=Setup();liveCurve=&out.curve;
 const auto actor=std::vector<unsigned char>(std::begin(f.actor),std::end(f.actor)),movement=std::vector<unsigned char>(std::begin(f.movement),std::end(f.movement)),ability=std::vector<unsigned char>(std::begin(f.ability),std::end(f.ability));
 Observe(owner);Check(Evaluate(f.anim+0x318,&out)&&calls==1&&f.pose[0].t[0]==13&&f.pose[1].t[0]==20,"only the stationary rendered pose changes, preserving root");
 Check(!std::memcmp(actor.data(),f.actor,sizeof(f.actor))&&!std::memcmp(movement.data(),f.movement,sizeof(f.movement))&&!std::memcmp(ability.data(),f.ability,sizeof(f.ability)),"movement, jump/action flags, ability memory and vectors remain byte-identical");
 for(int mode=0;mode<6;++mode){out=Setup();liveCurve=&out.curve;if(mode==0)Put(f.movement,0xD8,Vector{10,0,0});if(mode==1)Put(f.movement,0x188,Vector{10,0,0});if(mode==2)Put(f.actor,0x500,Vector{1,0,0});if(mode==3)Put(f.actor,0x50C,Vector{1,0,0});if(mode==4)Put<unsigned char>(f.movement,0x1B0,3);if(mode==5)Put(f.instance,0x128,.8f);const auto before=f.pose;Observe(owner);Check(!Evaluate(f.anim+0x318,&out)&&!calls&&!std::memcmp(before.data(),f.pose.data(),sizeof(f.pose)),"movement/intent against wall/jump/start pose all retain native output");}
 out=Setup();liveCurve=&out.curve;Observe(owner);out.curve.values.count=1;Check(!Evaluate(f.anim+0x318,&out)&&!calls,"unknown curve-bearing context is untouched");
 out=Setup();liveCurve=&out.curve;Put<unsigned char>(f.ability,0xD8,0);Observe(owner);Check(!Evaluate(f.anim+0x318,&out)&&!g_mask,"finished ability cannot retain the stationary pose");
 out=Setup();liveCurve=&out.curve;owner.kind=dg::local_players::PoseKind::Horse;Observe(owner);Check(!g_mask&&!calls,"transformation clears only render observation");
 out=Setup();liveCurve=&out.curve;Observe(owner);Put(f.items,7*24+16,999);Check(!Evaluate(f.anim+0x318,&out)&&!calls,"expired return asset cannot be sampled");Shutdown();
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Stationary-only Geyser render, moving/jump passthrough and zero gameplay writes passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
