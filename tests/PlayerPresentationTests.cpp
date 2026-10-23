#include "../src/PlayerPresentationFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::presentation;
unsigned char actor[0x1100]{},pc[0x600]{},localPlayer[0x300]{},manager[0xB0]{},basic[0xD00]{},augmented[0xD00]{},parent[0xD00]{},table[0x80]{},classes[5][0x80]{},nameEntries[12][120]{};
uintptr_t names[12]{},nameChunks[1]{reinterpret_cast<uintptr_t>(names)},armVT[0x9B0/8]{},parentVT[3]{},objectChunks[1]{};
unsigned char items[2*24]{};dg::local_players::Context owner{};
bool focused=true,gameThread=true,localOwned=true,paused=false,cinematic=false,radial=false,parentAllowed=true;
unsigned char nativeCursor=8;unsigned refreshes=0,nativeQueries=0;
template<class T> void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
void Check(bool ok,const char* text){if(!ok)throw std::runtime_error(text);}
void Label(void* object,int index,const char* text){Put(object,0x18,index);Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(nameEntries[index]+12),108,text);}
bool Focus(){return focused;}
bool DisplayNative(void* p){++nativeQueries;return p==augmented;}
bool ParentNative(void*){return parentAllowed;}
unsigned char CursorNative(void*){return nativeCursor;}
void RefreshNative(void* p){++refreshes;HookDisplay(p);}
void Fresh(){
    focused=gameThread=localOwned=parentAllowed=true;paused=cinematic=radial=false;nativeCursor=8;refreshes=nativeQueries=0;
    g_ready=true;g_display=DisplayNative;g_parent=ParentNative;g_refresh=RefreshNative;g_cursor=CursorNative;g_focus=Focus;g_log=nullptr;g_seen[0]={};g_seen[1]={};
    owner={};owner.index=0;owner.localPlayer=reinterpret_cast<uintptr_t>(localPlayer);owner.controller=reinterpret_cast<uintptr_t>(pc);
    owner.character=owner.pawn=reinterpret_cast<uintptr_t>(actor);owner.world=0x1234;
    Put(actor,0x10,reinterpret_cast<uintptr_t>(classes[0]));Put<unsigned char>(actor,0x118,3);Put<unsigned char>(actor,0xC18,2);
    Put(actor,0x380,reinterpret_cast<uintptr_t>(parent));Put(actor,0xFB0,reinterpret_cast<uintptr_t>(basic));Put(actor,0xFB8,reinterpret_cast<uintptr_t>(augmented));
    Put(pc,0x350,owner.pawn);Put(pc,0x398,owner.localPlayer);Put(localPlayer,0x288,reinterpret_cast<uintptr_t>(manager));
    Put(manager,0x10,reinterpret_cast<uintptr_t>(classes[4]));Put(manager,0x20,owner.localPlayer);Put(manager,0x88,Array{});Put(manager,0x98,Array{});
    for(auto* component:{basic,augmented}){
        Put(component,0x10,reinterpret_cast<uintptr_t>(classes[1]));Put(component,0xE8,owner.character);Put(component,0,reinterpret_cast<uintptr_t>(armVT));
        Put(component,0xC20,reinterpret_cast<uintptr_t>(table));Put(component,0xC28,7);Put(component,0xC2C,0);
        Put<unsigned char>(component,0xBC8,1);Put(component,0xC08,0);Put(component,0xC0C,7);Put(component,0xC10,reinterpret_cast<uintptr_t>(parent)+0xBC0);
    }
    Put<unsigned char>(basic,0xC30,1);Put<unsigned char>(augmented,0xC30,0);
    armVT[0x9A0/8]=g_base+0x7283A0;parentVT[2]=g_base+0x705560;Put(parent,0xBC0,reinterpret_cast<uintptr_t>(parentVT));
    Put(table,0x10,reinterpret_cast<uintptr_t>(classes[2]));
    Put(actor,0xC,1);Put(parent,0xC,0);Put(items,0,reinterpret_cast<uintptr_t>(parent));Put(items,16,7);
    Put(items,24,reinterpret_cast<uintptr_t>(actor));Put(items,24+16,8);
    auto& rt=dg::runtime::Get();rt.warArmAppearance=0;rt.mouseReticleHideMode=0;rt.thirdPersonEnabled=true;
    rt.tpsTemporarilySuspended=false;rt.tpsCutsceneSuspended=false;rt.tpsViewGameplay=true;rt.tpsGameplayViewTick=GetTickCount64();
}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    for(unsigned i=0;i<12;++i)names[i]=reinterpret_cast<uintptr_t>(nameEntries[i]);
    objectChunks[0]=reinterpret_cast<uintptr_t>(items);Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(image.data(),0x3959694,2);
    Label(classes[0],0,"War_Blueprint_C");Label(classes[1],1,"AltPlayerSkinSkeletalMeshComponent");Label(classes[2],2,"DataTable");
    Label(table,3,"MayhemWarEquipmentItems");Label(classes[4],4,"MayhemUIManager_Blueprint_C");Label(basic,5,"DefaultGauntletMesh");Label(augmented,6,"TremorGauntletMesh");
    strcpy_s(reinterpret_cast<char*>(nameEntries[7]+12),108,"TremorGauntlet");
    Fresh();Check(!HookDisplay(basic)&&HookDisplay(augmented),"Native arm selection unchanged");
    auto& rt=dg::runtime::Get();rt.warArmAppearance=1;
    const auto beforeBasic=std::vector<unsigned char>(basic,basic+sizeof(basic));
    Check(HookDisplay(basic)&&!HookDisplay(augmented),"Basic selects the original arm despite native equipment unlock");
    Check(!std::memcmp(basic,beforeBasic.data(),sizeof(basic)),"selection does not write the unlock cache or component");
    rt.warArmAppearance=2;Check(!HookDisplay(basic)&&HookDisplay(augmented),"Augmented selects the upgraded mesh");
    rt.warArmAppearance=1;parentAllowed=false;Check(!HookDisplay(basic)&&HookDisplay(augmented),"native armor child policy retained when override cannot apply");
    Fresh();rt.warArmAppearance=1;owner.index=1;Check(HookDisplay(basic),"owned P2 arm supported");
    Fresh();rt.warArmAppearance=1;localOwned=false;Check(!HookDisplay(basic),"remote or unknown owner stays native");
    Fresh();rt.warArmAppearance=1;Put(actor,0xFB0,reinterpret_cast<uintptr_t>(augmented));Check(!HookDisplay(basic),"mismatched reciprocal component excluded");
    Fresh();rt.warArmAppearance=1;OnLivePlayer(owner);Check(refreshes==2,"one native refresh for both components");OnLivePlayer(owner);Check(refreshes==2,"no repeated refresh in steady gameplay");
    rt.warArmAppearance=0;OnLivePlayer(owner);Check(refreshes==4,"Native restores through the same skin refresh");
    Fresh();for(int mode=0;mode<=3;++mode){rt.mouseReticleHideMode=mode;Check(HookCursor(pc)==(mode==1||mode==3?0:8),"TPS mode matrix");}
    rt.thirdPersonEnabled=false;for(int mode=0;mode<=3;++mode){rt.mouseReticleHideMode=mode;Check(HookCursor(pc)==(mode==2||mode==3?0:8),"normal view mode matrix");}
    Fresh();rt.mouseReticleHideMode=3;Put(manager,0x88,Array{0x1000,1,1});Check(HookCursor(pc)==8,"native page cursor retained even if crosshair is still selected");
    Fresh();rt.mouseReticleHideMode=3;Put(manager,0x98,Array{0x1000,1,1});Check(HookCursor(pc)==8,"closing page transition retains cursor");
    Fresh();rt.mouseReticleHideMode=3;nativeCursor=1;Check(HookCursor(pc)==1,"native menu arrow retained");
    nativeCursor=0;Check(!HookCursor(pc),"native hidden cursor retained");
    Fresh();rt.mouseReticleHideMode=3;paused=true;Check(HookCursor(pc)==8,"pause retains cursor");paused=false;radial=true;Check(HookCursor(pc)==8,"wheels retain cursor");
    radial=false;cinematic=true;Check(HookCursor(pc)==8,"cinematic cursor remains native");cinematic=false;focused=false;Check(HookCursor(pc)==8,"Alt-Tab and overlay focus remain native");
    Fresh();rt.mouseReticleHideMode=3;gameThread=false;Check(HookCursor(pc)==8,"unverified thread retains cursor");
    Fresh();rt.mouseReticleHideMode=3;rt.tpsGameplayViewTick=0;Check(HookCursor(pc)==8,"unknown TPS view fails open");
    Fresh();rt.mouseReticleHideMode=3;owner.index=1;Check(HookCursor(pc)==8,"P2 controller cursor is not changed");
    Fresh();rt.mouseReticleHideMode=3;Shutdown();Check(HookCursor(pc)==8,"shutdown restores native query output");
}
void Configuration(){
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    Check(GetTempPathW(MAX_PATH,directory)&&GetTempFileNameW(directory,L"DGP",0,path),"temporary profile created");
    dg::config::Store a;a.SetPath(path);a.Load();
    Check(a.warArmAppearance==0&&a.mouseReticleHideMode==0,"old profiles retain native appearance and cursor");
    a.warArmAppearance=2;a.mouseReticleHideMode=3;Check(a.SaveNow(),"presentation settings saved");
    dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.warArmAppearance==2&&b.mouseReticleHideMode==3,"presentation settings round trip");
    Check(dg::runtime::Get().warArmAppearance==2&&dg::runtime::Get().mouseReticleHideMode==3,"runtime publication");
    WritePrivateProfileStringW(L"Values",L"WarArmAppearance",L"99",path);
    WritePrivateProfileStringW(L"Values",L"MouseReticleHideMode",L"-1",path);b.Load();
    Check(b.warArmAppearance==0&&b.mouseReticleHideMode==0,"corrupt profile values restore Native");
    b.ResetDefaults(false);Check(b.warArmAppearance==0&&b.mouseReticleHideMode==0,"Native reset");
    DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return cinematic;}bool RadialActive(unsigned){return radial;}}
namespace dg::local_players {bool ResolveActor(void* actor,Context& out){out=owner;return localOwned&&reinterpret_cast<uintptr_t>(actor)==owner.pawn;}}
int main(){try{Run();Configuration();std::cout<<"Native War arm choice, cursor policies and configuration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
