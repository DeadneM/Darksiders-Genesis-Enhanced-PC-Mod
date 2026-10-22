#include "../src/ForeignWrathFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::foreign_wrath;
struct Fixture {
    unsigned char actor[0x1100]{},controller[0x400]{},mesh[0x800]{},anim[0x4D00]{},blend[0xE0]{},samples[72]{},sequence[0x100]{},skeleton[0x80]{};
    unsigned char stone[0x120]{},cdo[0x240]{},ability[0x240]{},nativeMontage[0x180]{},replacement[0x180]{},instance[0x50]{};
    unsigned char classes[6][0x80]{},stonePackage[0x80]{},montagePackage[0x80]{},items[640*24]{};
} f;
unsigned char entries[24][160]{};uintptr_t names[24]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
dg::local_players::Context owner{};bool gameThread=true,local=true,createOK=true,canPlay=true,nested=false,throwResolve=false;
unsigned getterCalls=0,resolveCalls=0,playCalls=0,finishCalls=0,createCalls=0;unsigned char finishCause=255;
void* playedAbility=nullptr;void* playedMontage=nullptr;float playedRate=0;Name playedSection{};
struct Creation {void* sequence=nullptr;Name slot{};float in=0,out=0,rate=0;int loops=0;float trigger=0,start=0;} creation;
template<class T>void Put(void* p,size_t o,T value){std::memcpy(static_cast<unsigned char*>(p)+o,&value,sizeof(value));}
void Check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
void Label(void* object,unsigned index,const char* text){Put(object,0x18,int(index));Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),148,text);}
void Register(unsigned index,void* object,int serial){
    Put(object,8,uint32_t(0));Put(object,0xC,int(index));Put(f.items,index*24,reinterpret_cast<uintptr_t>(object));Put(f.items,index*24+8,uint32_t(0));Put(f.items,index*24+16,serial);
}
uintptr_t* GetterNative(void*,uintptr_t* result,Name){++getterCalls;if(result)*result=0xCAFE;return result;}
uintptr_t* ResolveNative(void* actor,uintptr_t* result,Name id,bool* unlocked,bool* affordable){
    ++resolveCalls;HookGetter(actor,result,id);*unlocked=id.number==2;*affordable=false;
    if(nested){
        uintptr_t other=0;HookGetter(actor,&other,{id.index,id.number+1});Check(other==0xCAFE,"unrelated ID stays native inside foreign scope");
        HookGetter(f.controller,&other,id);Check(other==0xCAFE,"other actor stays native inside foreign scope");
    }
    if(throwResolve)throw std::runtime_error("mock native error");
    return result;
}
void PlayNative(void* ability,void* montage,float rate,Name section){
    ++playCalls;playedAbility=ability;playedMontage=montage;playedRate=rate;playedSection=section;
    Put(ability,0x1D8,reinterpret_cast<uintptr_t>(f.instance));
}
void* CreateNative(void* seq,Name slot,float in,float out,float rate,int loops,float trigger,float start){
    ++createCalls;creation={seq,slot,in,out,rate,loops,trigger,start};return createOK?f.replacement:nullptr;
}
void FinishNative(void*,unsigned char cause){++finishCalls;finishCause=cause;}
bool PlayingNative(void* instance){Check(instance==f.instance,"native montage instance is checked");return canPlay;}
void Fresh(){
    f={};owner={};gameThread=local=createOK=canPlay=true;nested=throwResolve=false;
    getterCalls=resolveCalls=playCalls=finishCalls=createCalls=0;finishCause=255;creation={};
    g_ready=true;g_assetsReady=true;g_requested=false;g_override=nullptr;g_getter=GetterNative;g_play=PlayNative;g_create=CreateNative;g_finish=FinishNative;g_playing=PlayingNative;g_log=nullptr;
    g_cursor=g_count=0;g_complete=false;g_scanTick=0;g_world=0;
    owner.index=0;owner.localPlayer=123;owner.world=456;owner.controller=reinterpret_cast<uintptr_t>(f.controller);owner.character=owner.pawn=reinterpret_cast<uintptr_t>(f.actor);
    const char* labels[]={"Strife_Blueprint_C","StrifeAnim_Blueprint_C","BlendSpace1D","AnimSequence","AnimMontage","BlueprintGeneratedClass"};
    for(unsigned i=0;i<6;++i)Label(f.classes[i],i+1,labels[i]);
    Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));
    Put(f.actor,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put<unsigned char>(f.actor,0x118,3);Put<unsigned char>(f.actor,0xC18,1);
    Put(f.actor,0x348,owner.controller);Put(f.controller,0x350,owner.pawn);Put(f.actor,0x380,reinterpret_cast<uintptr_t>(f.mesh));Put(f.mesh,0xE8,owner.pawn);
    Put(f.mesh,0x718,reinterpret_cast<uintptr_t>(f.anim));Put(f.anim,0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Put(f.anim,0x4C38,owner.pawn);
    Put(f.anim,0x49C8,g_base+0x2A76860);Put(f.anim,0x4A10,reinterpret_cast<uintptr_t>(f.blend));Put(f.blend,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Label(f.blend,7,"Strife_Locomotion");
    Put(f.blend,0xC8,Array{reinterpret_cast<uintptr_t>(f.samples),3,3});Put(f.samples,24,reinterpret_cast<uintptr_t>(f.sequence));Put(f.samples,44,1.0f);
    Put(f.sequence,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));Label(f.sequence,8,"Strife_Idle");Put(f.sequence,0x38,reinterpret_cast<uintptr_t>(f.skeleton));
    Put(f.sequence,0x90,2.666667f);Put(f.sequence,0x94,1.0f);Put(f.anim,0x28,reinterpret_cast<uintptr_t>(f.skeleton));
    strcpy_s(reinterpret_cast<char*>(entries[9]+12),148,"TPS");Put(f.anim,0x3740,Name{9,0});
    Put(f.stone,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Label(f.stone,10,"MayhemAbility_StoneSkin_Blueprint_C");Put(f.stone,0x20,reinterpret_cast<uintptr_t>(f.stonePackage));
    Label(f.stonePackage,11,"/Game/Blueprints/Abilities/Stoneskin/MayhemAbility_StoneSkin_Blueprint");Put(f.stone,0xF8,reinterpret_cast<uintptr_t>(f.cdo));Put(f.cdo,0x10,reinterpret_cast<uintptr_t>(f.stone));
    Put(f.ability,0x10,reinterpret_cast<uintptr_t>(f.stone));Put(f.ability,0x48,owner.pawn);Put(f.ability,0x1B0,owner.pawn);Put<unsigned char>(f.ability,0xD8,1);
    Put(f.nativeMontage,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Label(f.nativeMontage,12,"War_Atk_Stoneskin_Montage");Put(f.nativeMontage,0x20,reinterpret_cast<uintptr_t>(f.montagePackage));
    Label(f.montagePackage,13,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Montage");Put(f.nativeMontage,0x90,1.833333f);Put(f.nativeMontage,0x94,1.0f);
    Put(f.replacement,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));Put(f.replacement,0x38,reinterpret_cast<uintptr_t>(f.skeleton));Put(f.replacement,0x90,2.666667f);
    objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,640);
    Register(1,f.stone,41);g_stone={reinterpret_cast<uintptr_t>(f.stone),1,41};
    for(auto& choice:dg::runtime::Get().wrathSlots[0])choice=0;
}
void Cast(){HookPlay(f.ability,f.nativeMontage,1.0f,{0,0});}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    for(unsigned i=0;i<24;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Fresh();uintptr_t klass=0;const auto actorBefore=std::to_array(f.actor);
    Check(Prepare(f.actor,klass)&&klass==reinterpret_cast<uintptr_t>(f.stone),"ready native class and own compatible animation enable foreign slot");
    Check(!std::memcmp(f.actor,actorBefore.data(),actorBefore.size()),"preparation writes no actor, input or resource bytes");
    uintptr_t result=0;bool unlocked=false,affordable=true;nested=true;
    auto* out=Resolve(ResolveNative,f.actor,&result,{18,2},&unlocked,&affordable,klass);
    Check(out==&result&&result==klass&&unlocked&&!affordable&&resolveCalls==1&&getterCalls==2&&!g_override,"native resolver keeps return, slot-unlock and cost results with exact scoped class override");
    HookGetter(f.actor,&result,{18,2});Check(result==0xCAFE,"class lookup outside scope remains native");
    throwResolve=true;try{Resolve(ResolveNative,f.actor,&result,{18,2},&unlocked,&affordable,klass);}catch(const std::runtime_error&){}
    Check(!g_override,"scope is restored if native resolver throws");
    Fresh();Cast();Check(playCalls==1&&createCalls==1&&!finishCalls&&playedAbility==f.ability&&playedMontage==f.replacement,"one compatible montage goes through original native ability player");
    Check(std::abs(2.666667f/playedRate-1.833333f)<.00001f&&playedSection==Name{0,0},"cast duration and section preserved");
    Check(creation.sequence==f.sequence&&creation.slot==Name{9,0}&&creation.in==.1f&&creation.out==.1f&&creation.rate==1&&creation.loops==1&&creation.trigger==-1&&creation.start==0,"all eight native factory arguments match proved ABI");
    Check(!std::memcmp(f.actor,actorBefore.data(),actorBefore.size()),"casting adapter never writes pawn movement or gameplay bytes");
    Fresh();Put(f.nativeMontage,0x94,2.0f);Cast();Check(std::abs(2.666667f/playedRate-.9166665f)<.00001f,"native source montage rate scale is preserved in effective cast duration");
    Fresh();owner.index=1;owner.localPlayer=124;Cast();Check(playCalls==1&&playedMontage==f.replacement,"same exact owner contract supports P2 without borrowing P1");
    Fresh();Label(f.nativeMontage,12,"War_Atk_Stoneskin_Molten_Montage");Label(f.montagePackage,13,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin_Molten_Montage");Cast();Check(playCalls==1&&!finishCalls,"native Molten variant gets its own compatible duration");
    for(unsigned failure=0;failure<7;++failure){
        Fresh();
        if(failure==0)createOK=false;
        if(failure==1)canPlay=false;
        if(failure==2)Put(f.replacement,0x38,uintptr_t(0));
        if(failure==3)Put(f.anim,0x28,uintptr_t(0));
        if(failure==4)Label(f.montagePackage,13,"/Game/Other");
        if(failure==5)owner.kind=dg::local_players::PoseKind::Horse;
        if(failure==6)Put(f.nativeMontage,0x90,0.0f);
        Cast();Check(finishCalls==1&&finishCause==0,"unavailable source/factory/playback ends only exact owned cast through native normal finish");
        Check(!playCalls||playedMontage==f.replacement,"foreign War skeleton is never forwarded on an identified local Strife cast");
    }
    Fresh();Put<unsigned char>(f.ability,8,0x10);Cast();Check(playCalls==1&&playedMontage==f.nativeMontage&&!finishCalls&&!createCalls,"CDO never adapted or finished");
    Fresh();Label(f.classes[0],1,"War_Blueprint_C");Cast();Check(playedMontage==f.nativeMontage&&!createCalls&&!finishCalls,"War's own Stoneskin is fully native");
    Fresh();local=false;Cast();Check(playedMontage==f.nativeMontage&&!createCalls&&!finishCalls,"unknown or nonlocal owner fully native");
    Fresh();Put<unsigned char>(f.actor,0x118,2);Cast();Check(playedMontage==f.nativeMontage&&!createCalls&&!finishCalls,"remote role is never adapted or finished");
    Fresh();gameThread=false;Cast();Check(playedMontage==f.nativeMontage&&!createCalls&&!finishCalls,"other threads pass all native arguments unchanged");
    Fresh();Put(f.items,1*24+16,42);Check(!Prepare(f.actor,klass),"expired class serial cannot resolve another foreign cast");
    Fresh();g_assetsReady=false;Check(!Prepare(f.actor,klass),"not-yet-discovered class leaves slot unavailable");
    Fresh();g_stone={};g_assetsReady=false;std::memset(f.items,0,sizeof(f.items));Register(598,f.stone,41);g_requested=true;
    Observe(owner);Check(g_cursor==512&&!g_assetsReady,"one native observation scans at most 512 table entries");
    g_scanTick=0;Observe(owner);Check(g_assetsReady&&g_stone.index==598,"next bounded slice discovers exact loaded class");
    const int cursor=g_cursor;g_scanTick=0;Observe(owner);Check(g_cursor==cursor,"loaded class is reused without another global scan");
    Fresh();Shutdown();Cast();Check(playedMontage==f.nativeMontage&&!createCalls&&!finishCalls&&Status()==0,"shutdown restores complete native forwarding");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
namespace dg::local_players {bool ResolveActor(void*,Context& c){c=owner;return local;}}
int main(){try{Run();std::cout<<"Scoped foreign class, original unlock/cost, eight-argument montage factory and native finish isolation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
