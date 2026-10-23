#include "PlayerPresentationFeature.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>

namespace dg::presentation {
namespace {
using Display=bool(*)(void*);
using Refresh=void(*)(void*);
// GetMouseCursor returns a 32-bit enum, although CurrentMouseCursor is a byte.
// A byte-return detour leaves the other EAX bits undefined for Slate's index.
using Cursor=std::int32_t(*)(void*);
Display g_display=nullptr,g_parent=nullptr;Refresh g_refresh=nullptr;Cursor g_cursor=nullptr;
uintptr_t g_base=0;FocusFn g_focus=nullptr;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
struct Seen {native_objects::Identity actor{};int mode=0;};Seen g_seen[2]{};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
bool Arm(uintptr_t component,uintptr_t actor,unsigned side){
    uintptr_t cls=0,owner=0,vt=0,entry=0,table=0;struct Name{int index,number;} row{};
    unsigned char inverse=0;
    return Read(component,0x10,cls)&&Is(cls,"AltPlayerSkinSkeletalMeshComponent")&&Read(component,0xE8,owner)&&owner==actor&&
        (side?Is(component,"TremorGauntletMesh"):Is(component,"DefaultGauntletMesh"))&&
        Read(component,0,vt)&&Read(vt,0x9A0,entry)&&entry==g_base+0x7283A0&&
        Read(component,0xC20,table)&&Read(table,0x10,cls)&&Is(cls,"DataTable")&&Is(table,"MayhemWarEquipmentItems")&&
        Read(component,0xC28,row)&&native_objects::NameValueIs(g_base,row.index,row.number,"TremorGauntlet")&&
        Read(component,0xC30,inverse)&&inverse==(side?0:1);
}
bool OwnedWar(uintptr_t actor,local_players::Context& c){
    uintptr_t cls=0,pawn=0;unsigned char role=0,type=0;
    return native_tps::OnGameThread()&&Read(actor,0x10,cls)&&Is(cls,"War_Blueprint_C")&&
        local_players::ResolveActor(reinterpret_cast<void*>(actor),c)&&c.index>=0&&c.index<2&&c.localPlayer&&
        c.kind==local_players::PoseKind::Foot&&c.character==actor&&c.pawn==actor&&
        Read(c.controller,0x350,pawn)&&pawn==actor&&Read(actor,0x118,role)&&role==3&&Read(actor,0xC18,type)&&type==2;
}
bool ParentAllows(uintptr_t component,uintptr_t actor,bool& allowed){
    unsigned char check=0;uintptr_t iface=0,parent=0,vt=0,entry=0,main=0;struct Weak{int index,serial;} weak{};
    if(!Read(component,0xBC8,check)||!Read(component,0xC08,weak)||!Read(component,0xC10,iface))return false;
    if(!check||!iface||weak.index<0||weak.serial<=0){allowed=true;return true;}
    int serial=0;uint32_t flags=0;native_objects::MemoryReader reader;
    if(!native_objects::Item(reader,g_base,weak.index,parent,serial,flags))return false;
    if(serial!=weak.serial){allowed=true;return true;}
    // The exact same live parent policy used by 7283A0. Armor skins that
    // disallow child meshes retain that decision; no unlock cache is written.
    if(iface!=parent+0xBC0||!Read(actor,0x380,main)||main!=parent||
       !Read(iface,0,vt)||!Read(vt,0x10,entry)||entry!=g_base+0x705560)return false;
    allowed=g_parent(reinterpret_cast<void*>(iface));return true;
}
bool HookDisplay(void* live){
    const bool native=g_display?g_display(live):false;const int mode=runtime::Get().warArmAppearance.load();
    if(!g_ready||mode<1||mode>2||!native_tps::OnGameThread())return native;
    const auto component=reinterpret_cast<uintptr_t>(live);uintptr_t actor=0,basic=0,augmented=0;
    local_players::Context c{};bool parent=true;
    if(!Read(component,0xE8,actor)||!OwnedWar(actor,c)||!Read(actor,0xFB0,basic)||!Read(actor,0xFB8,augmented)||
       (component!=basic&&component!=augmented)||!Arm(basic,actor,0)||!Arm(augmented,actor,1)||
       !ParentAllows(component,actor,parent)||!parent)return native;
    return mode==1?component==basic:component==augmented;
}
bool Empty(uintptr_t manager,size_t offset){
    Array a{};return Read(manager,offset,a)&&a.count==0&&a.capacity>=0&&a.capacity<=64;
}
std::int32_t HookCursor(void* live){
    const std::int32_t native=g_cursor?g_cursor(live):0;const int mode=runtime::Get().mouseReticleHideMode.load();
    if(!g_ready||mode<1||mode>3||native!=8||!g_focus||!g_focus()||!native_tps::OnGameThread())return native;
    const auto pc=reinterpret_cast<uintptr_t>(live);uintptr_t pawn=0,local=0,manager=0,cls=0,outer=0;
    local_players::Context c{};
    if(!Read(pc,0x350,pawn)||!local_players::ResolveActor(reinterpret_cast<void*>(pawn),c)||c.index!=0||c.controller!=pc||
       !Read(pc,0x398,local)||local!=c.localPlayer||!Read(local,0x288,manager)||!Read(manager,0x10,cls)||
       !Is(cls,"MayhemUIManager_Blueprint_C")||!Read(manager,0x20,outer)||outer!=local||
       !Empty(manager,0x88)||!Empty(manager,0x98)||native_tps::IsWorldPaused(reinterpret_cast<void*>(pawn))||
       native_tps::CinematicActive(0)||native_tps::RadialActive(0))return native;
    const auto& rt=runtime::Get();
    const bool tps=rt.thirdPersonEnabled.load()&&!rt.tpsTemporarilySuspended.load()&&!rt.tpsCutsceneSuspended.load();
    if(tps){const auto tick=rt.tpsGameplayViewTick.load(),now=GetTickCount64();
        if(!rt.tpsViewGameplay.load()||!tick||now<tick||now-tick>300)return native;}
    if(mode==3||(mode==1&&tps)||(mode==2&&!tps))return 0;
    return native;
}
}
void Initialize(LogFn log,bool exact,FocusFn focus){
    g_log=log;g_focus=focus;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char display[]={0x40,0x53,0x48,0x83,0xEC,0x20};
    constexpr unsigned char refresh[]={0x40,0x57,0x48,0x83,0xEC,0x60};
    constexpr unsigned char parent[]={0x40,0x57,0x48,0x83,0xEC,0x40};
    constexpr unsigned char cursor[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0x01};
    constexpr unsigned char cursorResult[]={0x0F,0xB6,0x83,0x51,0x05,0x00,0x00}; // movzx eax, byte [rbx+551]
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x7283A0),display,sizeof(display))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x7152C0),refresh,sizeof(refresh))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x705560),parent,sizeof(parent))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x1ACAA40),cursor,sizeof(cursor))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x1ACAA56),cursorResult,sizeof(cursorResult)))return;
    auto* arm=reinterpret_cast<void*>(g_base+0x7283A0);auto* mouse=reinterpret_cast<void*>(g_base+0x1ACAA40);
    if(MH_CreateHook(arm,reinterpret_cast<void*>(&HookDisplay),reinterpret_cast<void**>(&g_display))!=MH_OK)return;
    if(MH_CreateHook(mouse,reinterpret_cast<void*>(&HookCursor),reinterpret_cast<void**>(&g_cursor))!=MH_OK){MH_RemoveHook(arm);g_display=nullptr;return;}
    if(MH_EnableHook(arm)!=MH_OK||MH_EnableHook(mouse)!=MH_OK){MH_DisableHook(arm);MH_DisableHook(mouse);MH_RemoveHook(arm);MH_RemoveHook(mouse);return;}
    g_parent=reinterpret_cast<Display>(g_base+0x705560);g_refresh=reinterpret_cast<Refresh>(g_base+0x7152C0);g_ready=true;
    if(log)log("Presentation V1.87 READY: 32-bit native cursor enum ABI verified; native War arm selection and menus/input/unlocks retained; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& seen=g_seen[c.index];const int mode=runtime::Get().warArmAppearance.load();
    if(mode==0&&seen.mode==0)return;
    local_players::Context owner{};uintptr_t basic=0,augmented=0;native_objects::Identity identity{};
    if(!OwnedWar(c.character,owner)||!Read(c.character,0xFB0,basic)||!Read(c.character,0xFB8,augmented)||
       !Arm(basic,c.character,0)||!Arm(augmented,c.character,1)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),identity)||
       (identity==seen.actor&&mode==seen.mode))return;
    g_refresh(reinterpret_cast<void*>(basic));g_refresh(reinterpret_cast<void*>(augmented));seen={identity,mode};
    if(g_log)g_log("Presentation V1.86 War arm: native component skin refresh; equipment and save untouched; EXPERIMENTAL");
}
bool Ready(){return g_ready.load();}
void Shutdown(){g_ready=false;}
}
