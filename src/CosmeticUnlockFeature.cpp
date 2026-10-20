#include "CosmeticUnlockFeature.h"
#include "CosmeticNativeBoundary.h"
#include "NativeObjectName.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>

namespace dg::cosmetics {
namespace {
// PlayerState execIsSkinUnlocked (7C4440) passes a borrowed row handle to
// 574870(playerState,handle). IsSkinEquipped is deliberately never hooked.
struct RowHandle {uintptr_t table=0;int index=-1,number=0;};
static_assert(sizeof(RowHandle)==16);
using CheckFn=bool(*)(void*,const RowHandle*);
CheckFn g_original=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};std::atomic_uint g_overrides{0};
enum class Group {Unknown,BonusMount,Progression};

Group Classify(const RowHandle* handle){
    native_objects::MemoryReader reader;RowHandle h{};uintptr_t cls=0,package=0;
    if(!reader.Read(reinterpret_cast<uintptr_t>(handle),0,h)||!h.table||h.number||
       !reader.Read(h.table,0x10,cls)||!native_objects::NameIs(g_base,cls,"DataTable")||
       !reader.Read(h.table,0x20,package))return Group::Unknown;
    const auto row=[&]<size_t N>(const char (&name)[N]){return native_objects::NameValueIs(g_base,h.index,h.number,name);};
    if(native_objects::NameIs(g_base,h.table,"RuinSkinTable")&&
       native_objects::NameIs(g_base,package,"/Game/Data/ArtData/Skins/RuinSkinTable")&&row("RuinSkin_Rampage"))return Group::BonusMount;
    if(native_objects::NameIs(g_base,h.table,"MayhemSkinTable")&&
       native_objects::NameIs(g_base,package,"/Game/Data/ArtData/Skins/MayhemSkinTable")&&row("MayhemSkin_Despair"))return Group::BonusMount;
    if(native_objects::NameIs(g_base,h.table,"WarSkinTable")&&
       native_objects::NameIs(g_base,package,"/Game/Data/ArtData/Skins/WarSkinTable")&&
       (row("WarSkin_Abyssal")||row("WarSkin_Silver")||row("WarSkin_Gold")))return Group::Progression;
    if(native_objects::NameIs(g_base,h.table,"StrifeSkinTable")&&
       native_objects::NameIs(g_base,package,"/Game/Data/ArtData/Skins/StrifeSkinTable")&&
       (row("StrifeSkin_Abyssal")||row("StrifeSkin_Silver")||row("StrifeSkin_Gold")))return Group::Progression;
    return Group::Unknown;
}
bool HookCheck(void* playerState,const RowHandle* handle){
    if(!g_original)return false;
    const bool native=g_original(playerState,handle);
    if(native||!g_ready.load(std::memory_order_acquire))return native;
    const auto& rt=runtime::Get();
    if(!rt.bonusMountSkinsEnabled.load()&&!rt.progressionSkinsEnabled.load())return native;
    const Group group=Classify(handle);
    const bool allow=(group==Group::BonusMount&&rt.bonusMountSkinsEnabled.load())||
        (group==Group::Progression&&rt.progressionSkinsEnabled.load());
    if(allow&&g_overrides.fetch_add(1)==0&&g_log)
        g_log("Cosmetics V1.68: exact skin unlock overridden; native selection only, no save flags, rewards, stats or DLC ownership written; EXPERIMENTAL");
    return allow;
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    if(!UnlockBoundaryMatches(g_base)){if(log)log("Cosmetics V1.68 DISABLED: IsSkinUnlocked registration/call signature mismatch");return;}
    const auto initialized=MH_Initialize();if(initialized!=MH_OK&&initialized!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+UnlockRva);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookCheck),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Cosmetics V1.68 READY: verified IsSkinUnlocked boundary; IsSkinEquipped untouched; two bonus mounts / six progression appearances; no grants or ownership writes; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Shutdown(){g_ready.store(false,std::memory_order_release);}
}
