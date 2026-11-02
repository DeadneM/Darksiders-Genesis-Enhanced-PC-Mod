#include "../src/ShieldVisualFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
namespace {
using namespace dg::shield_visual;
struct Fixture {
 unsigned char actor[2][0x1100]{},mesh[2][0x800]{},controller[2][0x400]{},component[2][0x200]{},status[2][0x100]{},particles[34][0x800]{};
 unsigned char classes[10][0x100]{},properties[2][0x100]{},source[0x100]{},target[0x100]{},package[0x100]{},world[0x100]{},otherWorld[0x100]{};
 uintptr_t statuses[2][1]{},parents[1]{},names[64]{},nameChunk[1]{},objectChunk[1]{};unsigned char entries[64][512]{},items[96*24]{};
} f;
bool thread=true;unsigned visibleCalls=0,objectIndex=0;dg::local_players::Context context[2]{};
template<class T>void Put(void* p,size_t at,const T& value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool value,const char* text){if(!value)throw std::runtime_error(text);}
void Label(void* object,int index,const char* name){Put(object,0x18,index);strcpy_s(reinterpret_cast<char*>(f.entries[index]+12),500,name);}
Id Live(void* object){const auto i=objectIndex++;Check(i<96,"fixture object bound");Put(object,0xC,int(i));Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(object));Put(f.items,size_t(i)*24+16,int(i+100));return {reinterpret_cast<uintptr_t>(object),int(i),int(i+100)};}
void Visible(void* p,bool show,unsigned char propagation){Check(propagation==1,"retained visibility ABI");unsigned char flags=0;std::memcpy(&flags,static_cast<unsigned char*>(p)+0x1CC,1);flags=show?static_cast<unsigned char>(flags|0x20):static_cast<unsigned char>(flags&~0x20);Put(p,0x1CC,flags);++visibleCalls;}
void Tick(int player=0){g_players[unsigned(player)].next=0;g_poll=0;OnLivePlayer(context[player]);}
void Fresh(std::vector<unsigned char>& image){
 thread=true;Shutdown();f={};g_players={};g_effects={};g_count=0;g_poll=0;g_binds=g_ends=0;visibleCalls=objectIndex=0;g_base=reinterpret_cast<uintptr_t>(image.data());
 const unsigned char stop[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x49,0x78,0x48,0x85,0xC9};const unsigned char damage[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x50,0x48,0x8B,0x79,0x68,0x48,0x8B};
 std::memcpy(image.data()+0x69FB90,stop,sizeof(stop));std::memcpy(image.data()+0x6D0860,damage,sizeof(damage));
 for(int i=0;i<64;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i]);f.nameChunk[0]=reinterpret_cast<uintptr_t>(f.names);f.objectChunk[0]=reinterpret_cast<uintptr_t>(f.items);
 Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(f.nameChunk));Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(f.objectChunk));Put(image.data(),0x3959694,96);
 const char* kinds[]={"ActorClass","MeshClass","StatusComponent","MayhemStatusEffect_DamageShield","ParticleSystemComponent","ParticleSystem","ObjectProperty","ArrayProperty","OtherStatus","World"};
 for(int i=0;i<10;++i){Label(f.classes[i],i+1,kinds[i]);Live(f.classes[i]);}
 f.parents[0]=reinterpret_cast<uintptr_t>(f.classes[3])+0x30;Put(f.classes[3],0x30,reinterpret_cast<uintptr_t>(f.parents));Put(f.classes[3],0x38,0);Put(image.data(),0x3812750,reinterpret_cast<uintptr_t>(f.classes[3]));
 Label(f.properties[0],11,"m_pStatusEffectComponent");Put(f.properties[0],0x10,reinterpret_cast<uintptr_t>(f.classes[6]));Put(f.properties[0],0x34,8);Put(f.properties[0],0x44,0x990);Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.properties[0]));
 Label(f.properties[1],12,"m_statusEffects");Put(f.properties[1],0x10,reinterpret_cast<uintptr_t>(f.classes[7]));Put(f.properties[1],0x34,16);Put(f.properties[1],0x44,0x130);Put(f.classes[2],0x48,reinterpret_cast<uintptr_t>(f.properties[1]));
 Label(f.source,13,"P_CreatureCore_ShieldDemon");Label(f.package,14,"/Game/VFX/Particles/Characters/Generic/P_CreatureCore_ShieldDemon");Label(f.target,15,"P_Borrower_Shield");Label(f.properties[0],11,"m_pStatusEffectComponent");strcpy_s(reinterpret_cast<char*>(f.entries[16]+12),500,"VFX_Aura");
 Put(f.source,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.source,0x20,reinterpret_cast<uintptr_t>(f.package));Put(f.target,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Live(f.source);Live(f.target);Live(f.package);
 Put(f.world,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Put(f.otherWorld,0x10,reinterpret_cast<uintptr_t>(f.classes[9]));Live(f.world);Live(f.otherWorld);
 for(int i=0;i<2;++i){
  Put(f.actor[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor[i],0x118,3);Put(f.actor[i],0x380,reinterpret_cast<uintptr_t>(f.mesh[i]));Put(f.actor[i],0x990,reinterpret_cast<uintptr_t>(f.component[i]));Put(f.controller[i],0x350,reinterpret_cast<uintptr_t>(f.actor[i]));Put(f.mesh[i],0xE8,reinterpret_cast<uintptr_t>(f.actor[i]));
  Put(f.component[i],0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.component[i],0xE8,reinterpret_cast<uintptr_t>(f.actor[i]));f.statuses[i][0]=reinterpret_cast<uintptr_t>(f.status[i]);Put(f.component[i],0x130,Array{reinterpret_cast<uintptr_t>(f.statuses[i]),1,1});
  Put(f.status[i],0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Put(f.status[i],0x58,reinterpret_cast<uintptr_t>(f.source));Put(f.status[i],0x68,reinterpret_cast<uintptr_t>(f.actor[i]));Put(f.status[i],0x78,reinterpret_cast<uintptr_t>(f.particles[i]));Put(f.status[i],0xAC,12.f);
  Live(f.actor[i]);Live(f.mesh[i]);Live(f.controller[i]);Live(f.component[i]);Live(f.status[i]);context[i]={};context[i].index=i;context[i].localPlayer=1;context[i].controller=reinterpret_cast<uintptr_t>(f.controller[i]);context[i].character=context[i].pawn=reinterpret_cast<uintptr_t>(f.actor[i]);context[i].world=reinterpret_cast<uintptr_t>(f.world);
 }
 for(unsigned i=0;i<34;++i){auto& particle=f.particles[i];Put(particle,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(particle,0x578,reinterpret_cast<uintptr_t>(f.target));Put(particle,0x108,reinterpret_cast<uintptr_t>(f.mesh[i==1?1:0]));Put<unsigned char>(particle,0x1CC,0x20);Live(particle);}
 dg::runtime::Snapshot s{};s.effectAppearances[unsigned(dg::effect_appearance::Family::ShieldDemonCore)]=4;dg::runtime::Publish(s);Initialize(g_base,Visible,nullptr);Check(CanTrack(),"verified native lifecycle enabled");
}
void Created(int particle=0){Track(f.particles[particle],f.source,f.target,f.world);}
bool Shown(int particle=0){unsigned char flags=0;std::memcpy(&flags,f.particles[particle]+0x1CC,1);return (flags&0x20)!=0;}
void Run(){
 std::vector<unsigned char> image(0x3960000);Fresh(image);Tick();Created();const auto before=std::vector<unsigned char>(f.status[0],f.status[0]+sizeof(f.status[0]));Tick();Check(g_count==1&&g_effects[0].status&&Shown()&&!visibleCalls,"owned native reference binds without changing protection or visibility");
 Check(!std::memcmp(before.data(),f.status[0],before.size()),"native status bytes are never edited");
 Put(f.status[0],0xAC,999.f);Tick();Check(Shown()&&!visibleCalls,"visual end follows native reference, never a guessed absorption threshold");
 Put(f.status[0],0x78,uintptr_t(0));Tick();Check(!Shown()&&visibleCalls==1,"native shield end hides the lingering replacement");Tick();Check(visibleCalls==1,"ended shield is not toggled every observation");
 Put(f.status[0],0x78,reinterpret_cast<uintptr_t>(f.particles[0]));Tick();Check(Shown()&&visibleCalls==2,"native recharge reusing the reference restores our visibility flag");
 Put(f.status[0],0x78,uintptr_t(0));Tick();ReleaseReused(f.particles[0]);Check(Shown()&&!g_count,"pool reuse clears the old shield flag before a new Hidden policy");Visible(f.particles[0],false,1);Tick();Check(!Shown(),"newly hidden Stinger/other native effect is never unhidden by an old shield record");
 Fresh(image);Tick();Created();Put(f.status[0],0x68,reinterpret_cast<uintptr_t>(f.actor[1]));Tick();Check(!g_effects[0].status&&Shown()&&!visibleCalls,"another recipient cannot bind P1 shield");
 Fresh(image);Tick();Created();Put(f.status[0],0x10,reinterpret_cast<uintptr_t>(f.classes[8]));Tick();Check(!g_effects[0].status&&!visibleCalls,"unrelated status never controls the shield");
 Fresh(image);Tick();Created();g_effects[0].born-=101;Put(f.status[0],0x78,uintptr_t(0));Tick();Check(g_effects[0].status&&!Shown(),"one-hit depletion before first observation uses the owned attachment and unique native status");
 Fresh(image);Tick();Created();Tick();Put(f.particles[0],0x578,reinterpret_cast<uintptr_t>(f.source));Put<unsigned char>(f.particles[0],0x1CC,0);Tick();Check(!g_count&&!visibleCalls&&!Shown(),"native template reuse wins without restoring an unowned flag");
 Fresh(image);Created(1);Tick(0);Check(!g_effects[0].status,"P2 shield cannot bind P1 status");Tick(1);Check(g_effects[0].actor.address==reinterpret_cast<uintptr_t>(f.actor[1]),"P2 binds its own status");Put(f.status[1],0x78,uintptr_t(0));Tick(1);Check(!Shown(1)&&Shown(0),"P2 end never hides P1 effect");
 Fresh(image);Tick();Created();Tick();Put(f.status[0],0x78,uintptr_t(0));dg::runtime::Publish(dg::runtime::Snapshot{});Tick();Check(!Shown(),"outstanding replacements are cleaned after selecting Native");
 Fresh(image);Tick();dg::native_bone::Point input{2,3,4},output{};Check(AdjustAttached(f.source,f.mesh[0],{16,0},&input,output)&&std::abs(output.x-1.2f)<.001f&&std::abs(output.y-1.8f)<.001f&&std::abs(output.z-2.4f)<.001f,"Borrower reduced to 60 percent without cumulative scaling");
 Check(!AdjustAttached(f.target,f.mesh[0],{16,0},&input,output)&&!AdjustAttached(f.source,f.mesh[0],{0,0},&input,output),"other templates and sockets retain native attachment scale");
 g_players[0].tick=GetTickCount64()-501;Check(!AdjustAttached(f.source,f.mesh[0],{16,0},&input,output),"stale ownership cannot scale effects");
 Fresh(image);Tick();input.x=std::numeric_limits<float>::quiet_NaN();Check(!AdjustAttached(f.source,f.mesh[0],{16,0},&input,output),"nonfinite scale fails open");thread=false;Created();Check(!g_count,"worker thread cannot register a visual mutation");thread=true;
 Fresh(image);for(int i=0;i<32;++i)Created(i);Check(!CanTrack(),"full bounded tracking rejects new substitutions");
 Fresh(image);Put<unsigned char>(image.data(),0x69FB90,0);Initialize(g_base,Visible,nullptr);Check(!CanTrack(),"unverified native lifecycle keeps original appearance");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Owned shield lifecycle, reference cleanup, pool reuse, P2 isolation and attachment scale passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
