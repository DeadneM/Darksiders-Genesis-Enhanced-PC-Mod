#include "CosmeticUnlockFeature.h"
#include "CosmeticNativeBoundary.h"
#include "NativeObjectName.h"
#include "RuntimeSettings.h"
#include "CosmeticSelectionList.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>
#include <intrin.h>

namespace dg::cosmetics {
namespace {
// PlayerState execIsSkinUnlocked (7C4440) passes a borrowed row handle to
// 574870(playerState,handle). IsSkinEquipped is deliberately never hooked.
using CheckFn=bool(*)(void*,const RowHandle*);
CheckFn g_original=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};std::atomic_uint g_overrides{0};
using FilterFn=void(*)(void*,void*);
using SetSkinFn=void(*)(void*,const RowHandle*);
FilterFn g_filter=nullptr;SetSkinFn g_setSkin=nullptr;
std::atomic_bool g_selectionReady{false};std::atomic_uint g_kept{0};
thread_local bool g_filtering=false;
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
unsigned char BonusType(const RowHandle& row){
    if(Classify(&row)!=Group::BonusMount)return 0;
    // Native EMayhemSkinType: MAYHEM_BP=8 (Despair), RUIN_BP=6 (Rampage).
    return native_objects::NameValueIs(g_base,row.index,row.number,"MayhemSkin_Despair")?8:6;
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
template<size_t N> bool InstanceNamed(uintptr_t object,const char (&expected)[N]){
    native_objects::MemoryReader reader;uintptr_t cls=0;
    if(!reader.Read(object,0x10,cls))return false;
    for(unsigned i=0;cls&&i<12;++i){
        if(native_objects::NameIs(g_base,cls,expected))return true;
        if(!reader.Read(cls,0x40,cls))return false;
    }
    return false;
}
void FilterSelection(void* save,void* instance,uintptr_t caller){
    if(!g_filter)return;
    // This synchronous native load boundary already resolves these rows and
    // edits this private SaveGame array. Keep the same calling thread/allocator;
    // no cached UObject, actor service, load request or disk write is involved.
    const bool enabled=g_selectionReady.load(std::memory_order_acquire)&&!g_filtering&&
        caller==SkinFilterCaller&&
        InstanceNamed(reinterpret_cast<uintptr_t>(save),"MayhemSaveGame")&&
        InstanceNamed(reinterpret_cast<uintptr_t>(instance),"MayhemGameInstance");
    if(!enabled){g_filter(save,instance);return;}
    native_objects::MemoryReader reader;
    const auto address=reinterpret_cast<uintptr_t>(save)+0x240;
    const auto before=ReadSkinList(reader,address);
    std::array<SkinEntry,2> selected{};unsigned count=0;
    if(before.valid)for(int i=0;i<before.count;++i){
        const auto& entry=before.entries[size_t(i)];
        const auto type=BonusType(entry.row);
        if(!type||entry.type!=type)continue;
        unsigned sameType=0;
        for(int j=0;j<before.count;++j)if(before.entries[size_t(j)].type==entry.type)++sameType;
        // Ambiguous/corrupt type lists must never invent a winning choice.
        if(sameType!=1||count==selected.size()){count=0;break;}
        selected[count++]=entry;
    }
    g_filtering=true;
    g_filter(save,instance); // All native DLC filtering still runs exactly once.
    if(g_setSkin)for(unsigned i=0;i<count;++i){
        const auto after=ReadSkinList(reader,address);const auto& entry=selected[i];
        if(!after.valid||after.count>=32||ContainsType(after,entry.type))continue;
        // Restore only a choice actually removed by this invocation, through
        // SetSkin's ordinary row/type resolution and native array allocation.
        g_setSkin(reinterpret_cast<void*>(address),&entry.row);
        const auto applied=ReadSkinList(reader,address);
        bool kept=false;
        if(applied.valid)for(int j=0;j<applied.count;++j)
            kept|=applied.entries[size_t(j)].row==entry.row&&applied.entries[size_t(j)].type==entry.type;
        if(kept){const unsigned n=++g_kept;if(g_log&&n<=12)
            g_log("Cosmetics V1.72 SELECTED_BONUS_KEPT during native load filter; existing choice only; DLC ownership and disk untouched by this callback; EXPERIMENTAL");}
    }
    g_filtering=false;
}
void HookFilter(void* save,void* instance){
    FilterSelection(save,instance,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
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
    if(SkinFilterBoundaryMatches(g_base)){
        void* filter=reinterpret_cast<void*>(g_base+SkinFilterRva);
        if(MH_CreateHook(filter,reinterpret_cast<void*>(&HookFilter),reinterpret_cast<void**>(&g_filter))==MH_OK){
            g_setSkin=reinterpret_cast<SetSkinFn>(g_base+SetSkinRva);
            if(MH_EnableHook(filter)==MH_OK)g_selectionReady.store(true,std::memory_order_release);
            else {MH_RemoveHook(filter);g_filter=nullptr;g_setSkin=nullptr;}
        }
    }
    if(log)log(g_selectionReady.load()?"Cosmetics V1.72 READY: preserve only selected Despair/Rampage during the verified native DLC load filter; menu availability toggle never unequips a saved choice; EXPERIMENTAL":
        "Cosmetics V1.72 selection retention unavailable; native load filter kept");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
bool SelectionRetentionReady(){return g_selectionReady.load(std::memory_order_acquire);}
void Shutdown(){g_selectionReady.store(false,std::memory_order_release);g_ready.store(false,std::memory_order_release);}
}
