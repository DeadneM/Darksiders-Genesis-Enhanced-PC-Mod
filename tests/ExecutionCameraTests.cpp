#include "../src/ExecutionCameraFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

namespace {
using namespace dg::execution_camera;
void Check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
void Policy(){
    Follow ground;Check(ground.Update(1,2,3,4,500,480,false,1000)==0,"ordinary pose establishes baseline");
    for(uint64_t t=1016;t<2000;t+=16)Check(ground.Update(1,2,3,4,500,500,true,t)==0,"captured floor finisher remains within dead zone");
    Follow high;high.Update(1,2,3,4,500,480,false,1000);
    float rise=0;for(uint64_t t=1016;t<1500;t+=16)rise=high.Update(1,2,3,4,500,780,true,t);
    Check(rise>250&&rise<261,"animated body rise follows after the 40-unit dead zone");
    float before=rise;rise=high.Update(1,2,3,4,500,480,false,1512);Check(rise<before&&rise>0,"ordinary framing restored smoothly after finisher");
    Check(high.Update(1,22,3,4,500,800,true,1528)==0&&!high.sampled,"character replacement cannot reuse baseline");
    Follow missing;Check(!missing.Update(1,2,3,4,500,900,true,1000)&&!missing.sampled,"first sighting during finisher fails open without ordinary baseline");
    Follow p1,p2;p1.Update(1,2,3,4,500,480,false,1000);p2.Update(11,12,13,14,100,80,false,1000);
    Check(p1.Update(1,2,3,4,500,800,true,1016)>0&&p2.Update(11,12,13,14,100,80,false,1016)==0,"local players retain independent follow states");
    Check(p1.Update(1,2,33,4,500,800,true,1032)==0,"world replacement clears follow");
    p1.Update(1,2,3,4,500,480,false,1048,1);Check(p1.Update(1,2,3,4,500,800,true,1064,2)==0&&!p1.sampled,"new published generation cannot reuse an earlier body's baseline");
    p2.Update(11,12,13,14,100,500,true,1032);Check(p2.Update(11,12,13,14,100,500,true,1500)==0,"long gap discards old animated baseline");
    Check(p2.Update(11,12,13,14,100,500,true,1400)==0,"clock reversal clears follow");
    p2.Update(11,12,13,14,100,80,false,1416);Check(p2.Update(11,12,13,14,100,3000,true,1432)==0&&!p2.sampled,"implausible body displacement rejected");
    Check(!p2.Update(11,12,13,14,100,std::numeric_limits<float>::infinity(),true,1448),"invalid animation point clears state");
}
unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},asset[0x220]{},bones[24]{},component[0x110]{},ability[0x200]{};
unsigned char classes[4][0x80]{},package[0x80]{},entries[10][100]{};
uintptr_t active[1]{},names[10]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},vt[0x400/8]{};
bool gameThread=true;float pelvisZ=480;unsigned locationCalls=0;
template<class T>void Put(void* p,size_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
void Label(void* object,unsigned index,const char* text){Put(object,0x18,int(index));Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),88,text);}
dg::native_bone::Point* Location(void*,dg::native_bone::Point* point,dg::native_bone::Name){++locationCalls;*point={0,0,pelvisZ};return point;}
void Feature(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<10;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Label(classes[0],0,"Strife_Blueprint_C");Label(classes[1],1,"MayhemAbility_Strife_Nephilim_Finisher_C");Label(classes[2],2,"MayhemPlayerFinisherAbility");
    Label(package,3,"/Script/DarksidersGenesis");Label(classes[3],4,"UnrelatedAbility");
    Put(classes[1],0x40,reinterpret_cast<uintptr_t>(classes[2]));Put(classes[2],0x20,reinterpret_cast<uintptr_t>(package));
    Put(actor,0x10,reinterpret_cast<uintptr_t>(classes[0]));Put<unsigned char>(actor,0xC18,1);Put<unsigned char>(actor,0x118,3);
    Put(actor,0x348,reinterpret_cast<uintptr_t>(controller));Put(controller,0x350,reinterpret_cast<uintptr_t>(actor));Put(actor,0x380,reinterpret_cast<uintptr_t>(mesh));
    Put(actor,0xC70,reinterpret_cast<uintptr_t>(component));Put(component,0xE8,reinterpret_cast<uintptr_t>(actor));
    active[0]=reinterpret_cast<uintptr_t>(ability);Put(component,0xF8,dg::native_bone::Array{reinterpret_cast<uintptr_t>(active),0,1});
    Put(ability,0x10,reinterpret_cast<uintptr_t>(classes[1]));Put(ability,0x48,reinterpret_cast<uintptr_t>(actor));Put(ability,0x1B0,reinterpret_cast<uintptr_t>(actor));
    Put(mesh,0xE8,reinterpret_cast<uintptr_t>(actor));Put(mesh,0,reinterpret_cast<uintptr_t>(vt));vt[0x3F8/8]=g_base+0x17A2380;
    Put(mesh,0x5A0,reinterpret_cast<uintptr_t>(asset));Put(asset,0x190,dg::native_bone::Array{reinterpret_cast<uintptr_t>(bones),2,2});
    Put(bones,12,dg::native_bone::Name{5,0});strcpy_s(reinterpret_cast<char*>(entries[5]+12),88,"Bone_Strife_Pelvis");
    dg::local_players::Context c{};c.index=0;c.localPlayer=123;c.world=456;c.controller=reinterpret_cast<uintptr_t>(controller);c.character=c.pawn=reinterpret_cast<uintptr_t>(actor);
    g_ready=true;g_location=Location;g_log=nullptr;g_players[0]={};g_players[1]={};
    Check(Offset(c,500)==0&&g_players[0].sampled,"owned local baseline read through exact native bone contract");
    Put(component,0x100,1);Check(Offset(c,500)==0&&g_players[0].active,"native finisher ancestry recognized without raising ground framing");
    Put(ability,0x1B0,uintptr_t(0));Offset(c,500);Check(!g_players[0].active,"ability from another character excluded");
    Put(ability,0x1B0,reinterpret_cast<uintptr_t>(actor));Put(ability,0x10,reinterpret_cast<uintptr_t>(classes[3]));Offset(c,500);Check(!g_players[0].active,"ordinary ability excluded");
    Put(ability,0x10,reinterpret_cast<uintptr_t>(classes[1]));Label(package,3,"/Game/Other");Offset(c,500);Check(!g_players[0].active,"same finisher type name in a foreign package excluded");
    Label(package,3,"/Script/DarksidersGenesis");c.index=1;Offset(c,500);Check(g_players[1].active,"P2 owns its own finisher state");
    c.kind=dg::local_players::PoseKind::Horse;Check(!Offset(c,500)&&!g_players[1].sampled,"horse restores native framing");c.kind=dg::local_players::PoseKind::Foot;
    gameThread=false;const auto calls=locationCalls;Check(!Offset(c,500)&&locationCalls==calls,"no bone function off the native game thread");gameThread=true;
    Put(controller,0x350,uintptr_t(0));Check(!Offset(c,500)&&!g_players[1].sampled,"controller replacement clears baseline");
    Shutdown();Check(!Offset(c,500),"shutdown uses original camera geometry");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {Slot PublishedSlot(unsigned){Slot s{};s.localPlayer=123;s.character=reinterpret_cast<uintptr_t>(actor);s.generation=1;return s;}}
int main(){try{Policy();Feature();std::cout<<"Ground/high finisher policy, native bone ownership and P1/P2 isolation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
