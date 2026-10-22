#include "../src/MountedJumpFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
unsigned char horse[0xC10]{},normal[0x20]{},move[0x780]{},pc[0x460]{},input[0x400]{},settings[0x100]{};
unsigned char classes[4][0x100]{},nameData[13][128]{},elements[2][0xF0]{};
unsigned char audio[3][0x40]{},packages[3][0x40]{},audioItems[3*24]{},root[0x1B0]{};
uintptr_t audioChunks[1]{reinterpret_cast<uintptr_t>(audioItems)};std::vector<uintptr_t> played;
uintptr_t names[13]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
dg::mounted_jump::Mapping mappings[3]{};dg::local_players::Context owner{};
bool thread=true,paused=false,radial=false,cinema=false,matched=true;unsigned jumps=0,queries=0;
template<class T> void Put(void* p,size_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
void Check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
int* Find(void* map,int* result,const void* key){
    Check(map==input+0x260,"native key lookup uses this player's own key-state map");++queries;
    const auto* name=static_cast<const dg::mounted_jump::Name*>(key);*result=-1;
    for(unsigned i=0;i<2;++i){dg::mounted_jump::Name candidate{};std::memcpy(&candidate,elements[i],8);if(candidate.index==name->index&&candidate.number==name->number)*result=int(i);}
    return result;
}
bool Jump(void* p,bool replay){Check(p==move&&!replay,"native horse DoJump contract");++jumps;Put<unsigned char>(move,0x1B0,3);return true;}
void Tick(bool focused=true,uint32_t gen=7){dg::mounted_jump::g_players[owner.index].tick=0;dg::mounted_jump::OnLivePlayer(owner,gen,focused);}
void Down(unsigned i,bool value){Put<unsigned char>(elements[i],0x34,value?1:0);}
void Ground(){Put<unsigned char>(move,0x1B0,1);}
void Press(unsigned i=0){Down(i,false);Tick();Down(i,true);Tick();}
void Run(){
    using namespace dg::mounted_jump;
    std::vector<unsigned char> image(0x3B00000);g_base=reinterpret_cast<uintptr_t>(image.data());
    const char* labels[]={"InputSettings","PlayerInput","Despair_Blueprint_C","Ruin_Blueprint_C","Jump","OtherAction"};
    for(unsigned i=0;i<6;++i){names[i]=reinterpret_cast<uintptr_t>(nameData[i]);strcpy_s(reinterpret_cast<char*>(nameData[i]+12),sizeof(nameData[i])-12,labels[i]);}
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<3;++i)Put(classes[i],0x18,int(i));
    Put(image.data(),0x3A5E1C0,reinterpret_cast<uintptr_t>(classes[0]));Put(classes[0],0xF8,reinterpret_cast<uintptr_t>(settings));
    mappings[0].action={4,0};mappings[0].key={6,0};mappings[1].action={5,0};mappings[1].key={7,0};mappings[2].action={4,0};mappings[2].key={8,0};
    Put(settings,0x80,Array{reinterpret_cast<uintptr_t>(mappings),3,3});
    Put(horse,0x10,reinterpret_cast<uintptr_t>(classes[2]));Put<unsigned char>(horse,0x118,3);
    Put(horse,0x348,reinterpret_cast<uintptr_t>(pc));Put(pc,0x350,reinterpret_cast<uintptr_t>(horse));Put(horse,0x388,reinterpret_cast<uintptr_t>(move));
    Put(move,0xE8,reinterpret_cast<uintptr_t>(horse));Put(move,0,g_base+0x270ABF0);Ground();Put(image.data(),0x270ABF0+0x600,g_base+0x667510);
    Put(pc,0x448,reinterpret_cast<uintptr_t>(input));Put(input,0x10,reinterpret_cast<uintptr_t>(classes[1]));Put(input,0x20,reinterpret_cast<uintptr_t>(pc));
    Put(input,0x260,Array{reinterpret_cast<uintptr_t>(elements),2,2});Put(elements[0],0,Name{6,0});Put(elements[1],0,Name{8,0});
    owner.index=0;owner.localPlayer=1;owner.kind=dg::local_players::PoseKind::Horse;
    owner.pawn=reinterpret_cast<uintptr_t>(horse);owner.character=reinterpret_cast<uintptr_t>(normal);owner.controller=reinterpret_cast<uintptr_t>(pc);
    g_ready=true;g_find=Find;g_jump=Jump;auto& option=dg::runtime::Get().mountedJumpEnabled;
    option=false;Tick();Check(!queries&&!jumps,"OFF performs no input lookup or jump");
    option=true;Down(0,true);Tick();Check(!jumps,"enabling while held requires release");
    Press();Check(jumps==1,"native Jump binding gives one native horse jump");Ground();Tick();Check(jumps==1,"holding cannot repeat on landing");
    Down(0,false);Tick();Ground();Press(1);Check(jumps==2,"second Jump mapping works without any imposed key code");
    Down(0,false);Down(1,false);Tick();Put<unsigned char>(move,0x1B0,3);Press();Check(jumps==2,"airborne horse cannot air-jump");
    Ground();Down(0,false);Down(1,false);Tick();mappings[0].key={9,0};Put(elements[0],0,Name{9,0});Down(0,true);Tick();Check(jumps==2,"rebinding while new key held does not trigger a jump");
    Press();Check(jumps==3,"fresh native binding is read after rebind without Alt-Tab or restart");
    Ground();paused=true;Press();paused=false;Tick();Check(jumps==3,"pause exit with held Jump cannot fire");Press();Check(jumps==4,"release after pause rearms the action");
    Ground();radial=true;Press();radial=false;Tick();Check(jumps==4,"wheel exit cannot trigger a held jump");
    cinema=true;Press();cinema=false;Tick();Check(jumps==4,"cinematic exit cannot trigger a held jump");
    Press();Check(jumps==5,"normal gameplay resumes after release");Ground();Press();Tick(false);Tick();Check(jumps==6,"focus restoration cannot add another held jump");
    Ground();Put(input,0x20,uintptr_t(0));Press();Check(jumps==6,"foreign player input cannot control this mount");Put(input,0x20,reinterpret_cast<uintptr_t>(pc));
    owner.index=1;Put(classes[2],0x18,3);Down(0,false);Down(1,false);Tick();Press(1);Check(jumps==7,"War's mount and P2 use the same independent action path");
    Ground();owner.kind=dg::local_players::PoseKind::Foot;Press();Check(jumps==7,"validated foot jump path remains untouched");
    owner.kind=dg::local_players::PoseKind::Horse;matched=false;Press();Check(jumps==7,"stale generation/owner cannot jump");matched=true;
    // Loaded native SoundCue contracts and per-leap audio lifecycle.
    const char* audioNames[]={"SoundCue","char_ruin_jump","char_ruin_jump_land","char_ruin_snort_low_vo",
        "/Game/Audio/Sfx/Character/Ruin/char_ruin_jump","/Game/Audio/Sfx/Character/Ruin/char_ruin_jump_land",
        "/Game/Audio/Sfx/Character/Ruin/char_ruin_snort_low_vo"};
    for(unsigned i=0;i<7;++i){names[i+6]=reinterpret_cast<uintptr_t>(nameData[i+6]);strcpy_s(reinterpret_cast<char*>(nameData[i+6]+12),sizeof(nameData[i+6])-12,audioNames[i]);}
    Put(classes[3],0x18,6);Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(audioChunks));Put(image.data(),0x3959694,3);
    for(unsigned i=0;i<3;++i){
        const auto address=reinterpret_cast<uintptr_t>(audio[i]);Put(audioItems,i*24,address);Put(audioItems,i*24+16,int(i+10));
        Put(audio[i],0xC,int(i));Put(audio[i],0x10,reinterpret_cast<uintptr_t>(classes[3]));Put(audio[i],0x18,int(i+7));
        Put(audio[i],0x20,reinterpret_cast<uintptr_t>(packages[i]));Put(packages[i],0x18,int(i+10));
    }
    Put(horse,0x158,reinterpret_cast<uintptr_t>(root));Put(root,0xE8,reinterpret_cast<uintptr_t>(horse));Put(root,0x1A0,Vector{10,20,30});
    g_sound=+[](void* context,void* cue,const Vector* p,const Vector* r,float volume,float pitch,float start,void* attenuation,void* concurrency,void* source){
        Check(context==horse&&source==horse&&p->x==10&&p->y==20&&p->z==30&&r->x==0&&r->y==0&&r->z==0,"native audio follows the owned horse origin");
        Check((volume==1.0f||volume==0.9f)&&pitch==1&&start==0&&!attenuation&&!concurrency,"native cue attenuation, Sfx and random samples remain active");
        played.push_back(reinterpret_cast<uintptr_t>(cue));
    };
    auto& sound=dg::runtime::Get().mountedJumpSound;sound=0;Ground();Down(0,false);Down(1,false);Tick();Press();
    Check(SoundAssetsReady()==7&&played.size()==1&&played[0]==reinterpret_cast<uintptr_t>(audio[0]),"one native varied jump sound on success");
    Tick();Check(played.size()==1,"held/airborne action never repeats audio");Ground();Tick();Tick();
    Check(played.size()==2&&played[1]==reinterpret_cast<uintptr_t>(audio[1]),"one native landing sound after this leap reaches ground");
    sound=1;Press();Check(played.size()==3&&played.back()==reinterpret_cast<uintptr_t>(audio[2]),"alternative snort uses its native random cue");
    Tick(false);Ground();Tick();Check(played.size()==3,"focus loss cancels any pending landing sound");
    sound=2;Press();Ground();Tick();Check(played.size()==3,"Off keeps jump working with no added sound");
    sound=0;Put(audioItems,16,99);Put(audio[0],0x10,reinterpret_cast<uintptr_t>(classes[0]));Press();Check(played.size()==3,"stale sound serial cannot play a recycled object");
    g_ready=false;
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinema;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {bool Matches(unsigned,uintptr_t,uint32_t){return matched;}}
int main(){try{Run();std::cout<<"Mounted native Jump binding checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
