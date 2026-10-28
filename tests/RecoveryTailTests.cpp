#include "../src/RecoveryTailFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::recovery_tail;
unsigned char actors[2][0x1100]{},pcs[2][0x400]{},components[2][0x180]{},moves[2][0x250]{},resources[2][0x400]{};
unsigned char abilities[2][0x250]{},helpers[2][0x40]{},montages[2][0xA0]{},packages[4][0x40]{},classes[8][0x40]{},items[8*24]{};
unsigned char names[17][256]{};uintptr_t entries[17]{},nameChunks[1]{},chunks[1]{reinterpret_cast<uintptr_t>(items)},lists[2][1]{};
dg::local_players::Context owners[2]{};bool thread=true,paused=false,radial=false,cinema=false,playing[2]{true,true},local=true;
uint64_t now=1000;unsigned opens=0,closes=0,interrupts=0;bool argsOK=true;
constexpr uint64_t Other=(uint64_t(1)<<7)|(uint64_t(1)<<40);
template<class T>void Put(void* p,size_t at,T v){std::memcpy(static_cast<unsigned char*>(p)+at,&v,sizeof(v));}
template<class T>T At(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool ok,const char* msg){if(!ok)throw std::runtime_error(msg);}
void Window(void* p,unsigned char cause,bool yes){argsOK=argsOK&&cause==1;auto bits=At<uint64_t>(p,0x120);Put(p,0x120,yes?bits|MoveBit:bits&~MoveBit);yes?++opens:++closes;}
void Interrupt(void* p,unsigned char cause,bool yes){argsOK=argsOK&&cause==1&&!yes;Put(p,0xB0,At<uint64_t>(p,0xB0)&~MoveBit);++interrupts;}
bool PlayingNative(void* p){return p==helpers[0]?playing[0]:p==helpers[1]?playing[1]:false;}
void Fresh(){
 g_pending={};thread=local=true;paused=radial=cinema=false;playing[0]=playing[1]=true;now=1000;opens=closes=interrupts=0;argsOK=true;
 g_ready=true;g_window=Window;g_interrupt=Interrupt;g_playing=PlayingNative;g_log=nullptr;
 for(unsigned i=0;i<4;++i)Put(packages[i],0x18,int(i+8));
 auto& rt=dg::runtime::Get();rt.recoveryDashTailEnabled=false;rt.recoveryChestTailEnabled=false;rt.recoveryTailDelaySeconds=0;
 std::memset(items,0,sizeof(items));
 for(unsigned i=0;i<2;++i){
  std::memset(actors[i],0,sizeof(actors[i]));std::memset(abilities[i],0,sizeof(abilities[i]));std::memset(moves[i],0,sizeof(moves[i]));
  auto& c=owners[i];c={};c.index=int(i);c.character=c.pawn=reinterpret_cast<uintptr_t>(actors[i]);c.controller=reinterpret_cast<uintptr_t>(pcs[i]);c.localPlayer=100+i;c.world=300;
  Put(actors[i],0x348,c.controller);Put(pcs[i],0x350,c.character);Put<unsigned char>(actors[i],0x118,3);Put<unsigned char>(actors[i],0xC18,static_cast<unsigned char>(i+1));
  Put(actors[i],0xEA0,reinterpret_cast<uintptr_t>(resources[i]));Put(resources[i],0x37C,100.f);
  Put(actors[i],0xC70,reinterpret_cast<uintptr_t>(components[i]));Put(components[i],0xE8,c.character);lists[i][0]=reinterpret_cast<uintptr_t>(abilities[i]);Put(components[i],0xF8,reinterpret_cast<uintptr_t>(lists[i]));Put(components[i],0x100,1);Put(components[i],0x104,1);
  Put(actors[i],0x388,reinterpret_cast<uintptr_t>(moves[i]));Put(moves[i],0xE8,c.character);Put<unsigned char>(moves[i],0x1B0,1);
  Put(abilities[i],0x10,reinterpret_cast<uintptr_t>(classes[i+1]));Put(abilities[i],0x48,c.character);Put(abilities[i],0x1B0,c.character);Put<unsigned char>(abilities[i],0xD8,2);
  Put(abilities[i],0x120,Other);Put(abilities[i],0xB0,Other);Put(abilities[i],0x1D8,reinterpret_cast<uintptr_t>(helpers[i]));
  Put(helpers[i],0x10,reinterpret_cast<uintptr_t>(classes[4]));Put(helpers[i],0x28,reinterpret_cast<uintptr_t>(montages[i]));
  Put(montages[i],0x10,reinterpret_cast<uintptr_t>(classes[5]));Put(montages[i],0x18,int(6+i));Put(montages[i],0x20,reinterpret_cast<uintptr_t>(packages[i+2]));Put(montages[i],0x94,1.f);
  Put(classes[i+1],0x18,int(i+1));Put(classes[i+1],0x10,reinterpret_cast<uintptr_t>(classes[3]));Put(classes[i+1],0x20,reinterpret_cast<uintptr_t>(packages[i]));
  for(unsigned k=0;k<4;++k){void* p=k==0?static_cast<void*>(actors[i]):k==1?static_cast<void*>(abilities[i]):k==2?static_cast<void*>(helpers[i]):static_cast<void*>(montages[i]);const unsigned id=i*4+k;Put(p,0xC,int(id));Put(items,id*24,reinterpret_cast<uintptr_t>(p));Put(items,id*24+16,int(100+id));}
 }
}
void Tick(unsigned i=0,bool focus=true){Service(owners[i],7+i,focus,now);}
void Stop(unsigned i=0){Tick(i);playing[i]=false;Tick(i);now+=30;Tick(i);}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,8);
 const char* labels[]={"None","MayhemAbility_Strife_ChestInteract_C","MayhemAbility_War_ChestInteract_C","BlueprintGeneratedClass","MayhemMontagePlayer","AnimMontage","Strife_IA_Chest_A_Open_Montage","War_IA_Chest_A_Open_Montage",
  "/Game/Blueprints/Abilities/Interact/MayhemAbility_Strife_ChestInteract","/Game/Blueprints/Abilities/Interact/MayhemAbility_War_ChestInteract",
  "/Game/Characters/Heroes/Hero_Strife/Animations/Strife_IA_Chest_A_Open_Montage","/Game/Characters/Heroes/Hero_War/Animation/War_IA_Chest_A_Open_Montage",
  "MayhemAbility_Strife_Ground_Dash_Level3_Blueprint_C","/Game/Blueprints/Abilities/Dash/MayhemAbility_Strife_Ground_Dash_Level3_Blueprint","Strife_Dash_Level3_Montage","/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Dash_Level3_Montage","Unrelated_C"};
 for(unsigned i=0;i<17;++i){strcpy_s(reinterpret_cast<char*>(names[i]+12),244,labels[i]);entries[i]=reinterpret_cast<uintptr_t>(names[i]);}
 nameChunks[0]=reinterpret_cast<uintptr_t>(entries);Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
 for(unsigned i=3;i<6;++i)Put(classes[i],0x18,int(i));for(unsigned i=0;i<4;++i)Put(packages[i],0x18,int(i+8));
 Fresh();Stop();Check(!opens,"both experiments OFF preserve native behavior");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Tick();now+=500;Tick();Check(!opens,"active montage never shortened");playing[0]=false;Tick();now+=29;Tick();Check(!opens,"stopped confirmation retained");++now;Tick();Check(opens==1&&argsOK,"one native MOVE request after observed completed animation");
 Check(abilities[0][0xD8]==2&&At<uint64_t>(abilities[0],0xB0)==Other&&At<uint64_t>(abilities[0],0x120)==(Other|MoveBit),"no forced finish or unrelated action-bit write");Tick();Check(opens==1,"no repeated request");
 dg::runtime::Get().recoveryChestTailEnabled=false;Tick();Check(closes==1&&interrupts==1&&At<uint64_t>(abilities[0],0x120)==Other,"OFF restores only claimed window");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;playing[0]=false;Tick();now+=1000;Tick();Check(!opens,"false before initial playback is not treated as completion");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;dg::runtime::Get().recoveryTailDelaySeconds=.2f;Tick();playing[0]=false;Tick();now+=229;Tick();Check(!opens,"user grace delay respected");++now;Tick();Check(opens==1,"grace delay plus confirmation expires");
 for(int bad=0;bad<8;++bad){Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Tick();if(bad==0)paused=true;if(bad==1)cinema=true;if(bad==2)radial=true;if(bad==3)thread=false;if(bad==4)owners[0].kind=dg::local_players::PoseKind::Horse;if(bad==5)Put<unsigned char>(abilities[0],0x148,1);if(bad==6)Put(abilities[0],0x48,owners[1].character);if(bad==7)Put(montages[0],0x20,reinterpret_cast<uintptr_t>(packages[3]));playing[0]=false;Tick();now+=500;Tick();Check(!opens,"menus/thread/transform/finishing/owner/package contracts reject");}
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Put(abilities[0],0x120,Other|MoveBit);Stop();dg::runtime::Get().recoveryChestTailEnabled=false;Tick();Check(!opens&&!closes,"native existing window never claimed");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Stop();owners[0].world=999;Tick();Check(!closes&&!interrupts,"world replacement never calls old objects");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Tick();playing[0]=false;Tick();Put(items,24+16,999);now+=500;Tick();Check(!opens,"stale/recycled ability cannot inherit stopped observation");
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Tick();Tick(1);playing[0]=playing[1]=false;Tick();Tick(1);now+=30;Tick();Tick(1);Check(opens==2,"P2 has independent playback/completion tracking");
 Fresh();dg::runtime::Get().recoveryDashTailEnabled=true;Put(classes[1],0x18,12);Put(packages[0],0x18,13);Put(montages[0],0x18,14);Put(abilities[0],0x200,reinterpret_cast<uintptr_t>(montages[0]));Put<unsigned char>(moves[0],0x1B0,6);Put<unsigned char>(moves[0],0x1B1,5);Put(abilities[0],0x224,.1f);Put(abilities[0],0x228,.5f);Tick();playing[0]=false;Tick();now+=500;Tick();Check(!opens,"forced dash time must be exhausted");Put<unsigned char>(moves[0],0x1B0,1);Put<unsigned char>(moves[0],0x1B1,0);Tick();Check(!opens,"Walking alone cannot bypass an unfinished dash timer");Put(abilities[0],0x224,0.f);Tick();now+=30;Tick();Check(opens==1,"ended authored dash can request MOVE without editing timer or velocity");
 wchar_t folder[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,folder)&&GetTempFileNameW(folder,L"DGR",0,path),"temporary config");
 dg::config::Store c;c.SetPath(path);Check(c.Load()&&!c.recoveryDashTailEnabled&&!c.recoveryChestTailEnabled&&!c.heavyLandingCustomTimingEnabled,"old profile opts in to none of new experiments");
 c.recoveryChestTailEnabled=true;c.recoveryDashTailEnabled=true;c.recoveryTailDelaySeconds=.25f;c.heavyLandingCustomTimingEnabled=true;c.heavyLandingDelaySeconds=.22f;c.heavyLandingGroundSeconds=.08f;c.sheathKeyboardKey='K';Check(c.SaveNow(),"experimental profile saves");
 dg::config::Store loaded;loaded.SetPath(path);Check(loaded.Load()&&loaded.recoveryChestTailEnabled&&loaded.recoveryDashTailEnabled&&loaded.recoveryTailDelaySeconds==.25f&&loaded.heavyLandingCustomTimingEnabled&&loaded.heavyLandingDelaySeconds==.22f&&loaded.heavyLandingGroundSeconds==.08f&&loaded.sheathKeyboardKey=='K',"six keys and existing shortcut roundtrip");
 Check(dg::runtime::Get().recoveryChestTailEnabled.load()&&dg::runtime::Get().recoveryDashTailEnabled.load()&&dg::runtime::Get().recoveryTailDelaySeconds.load()==.25f&&dg::runtime::Get().heavyLandingCustomTimingEnabled.load()&&dg::runtime::Get().heavyLandingDelaySeconds.load()==.22f,"loaded experiment settings reach the game-thread runtime");
 loaded.ResetDefaults(false);Check(!loaded.recoveryChestTailEnabled&&!loaded.recoveryDashTailEnabled&&!loaded.heavyLandingCustomTimingEnabled,"reset disables experiments");WritePrivateProfileStringW(nullptr,nullptr,nullptr,path);DeleteFileW(path);
 Fresh();dg::runtime::Get().recoveryChestTailEnabled=true;Stop();Shutdown();Check(!Ready()&&!closes,"shutdown does not call native APIs off thread");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {
bool Matches(unsigned i,uintptr_t actor,uint32_t generation){return local&&i<2&&actor==owners[i].character&&generation==7+i;}
bool ResolveActor(void* p,Context& c){for(const auto& x:owners)if(local&&x.character==reinterpret_cast<uintptr_t>(p)){c=x;return true;}return false;}
}
int main(){try{Run();std::cout<<"Recovery tail ownership, native window, montage/motion gates, restoration and settings passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
