#include "../src/LeviathanVisualFeature.cpp"
#include "ConfigStore.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
unsigned char actor[0x500]{},root[0x200]{},mesh[0x200]{},nativeEffect[0x850]{},custom[0x850]{},foreign[0x850]{};
unsigned char particleClass[0x40]{},componentClass[0x40]{},assets[3][0x40]{},packages[3][0x40]{},items[32*24]{};
uintptr_t chunks[1]{reinterpret_cast<uintptr_t>(items)},children[3]{};
unsigned char names[16][256]{};uintptr_t entries[16]{},nameChunks[1]{reinterpret_cast<uintptr_t>(entries)};
unsigned nameCount=0,spawns=0,destroys=0,visibleCalls=0;bool paused=false,rejectSpawn=false;
template<class T> void Put(void* p,size_t at,T value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
template<class T> T Get(void* p,size_t at){T v{};std::memcpy(&v,static_cast<unsigned char*>(p)+at,sizeof(v));return v;}
void Check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
dg::native_objects::Identity Register(void* p,int i){
    Put(p,0xC,i);Put(items,size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(items,size_t(i)*24+16,i+1);
    return {reinterpret_cast<uintptr_t>(p),i,i+1};
}
void Name(void* p,const char* s){
    const auto i=nameCount++;Check(i<16,"bounded names fixture");entries[i]=reinterpret_cast<uintptr_t>(names[i]);
    std::memcpy(names[i]+12,s,std::strlen(s)+1);Put(p,0x18,int(i));Put(p,0x1C,0);
}
void Visible(void* component,bool value,unsigned char propagation){
    Check(propagation==1,"native visibility uses dirty-only propagation");++visibleCalls;
    auto f=Get<unsigned char>(component,0x1CC);Put<unsigned char>(component,0x1CC,value?f|0x20:f&~0x20);
}
void Destroy(void* owner,void* component){Check(owner==actor&&component==custom,"only our owned particle is destroyed");++destroys;}
void* Spawn(void* asset,void* parent,uint64_t socket,const dg::leviathan_visual::Vector* location,
    const dg::leviathan_visual::Vector* rotation,const dg::leviathan_visual::Vector* scale,unsigned char attach,bool autoDestroy,unsigned char pool){
    Check(parent==root&&socket==0&&location->x==0&&location->y==0&&location->z==0&&rotation->y==0&&scale->x==1&&scale->z==1&&attach==0&&autoDestroy&&pool==0,"native root attachment without an invented height offset");
    ++spawns;if(rejectSpawn)return nullptr;Put(custom,0x578,asset);return custom;
}
void Run(){
    using namespace dg::leviathan_visual;
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x3959680,reinterpret_cast<uintptr_t>(chunks));Put(image.data(),0x3959694,32);
    Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(nameChunks));
    Name(particleClass,"ParticleSystem");Name(componentClass,"ParticleSystemComponent");
    const char* labels[]={"P_Dagon_CreatureCore_Status_Lvl1","P_Dagon_CreatureCore_Status_Lvl2","P_Dagon_CreatureCore_Status_Lvl3"};
    const char* paths[]={"/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl1","/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl2","/Game/VFX/Particles/Characters/Dagon/P_Dagon_CreatureCore_Status_Lvl3"};
    for(int i=0;i<3;++i){Name(assets[i],labels[i]);Name(packages[i],paths[i]);Put(assets[i],0x10,reinterpret_cast<uintptr_t>(particleClass));Put(assets[i],0x20,reinterpret_cast<uintptr_t>(packages[i]));g_templates[i]=Register(assets[i],i+1);Check(Template(reinterpret_cast<uintptr_t>(assets[i]))==i,"exact particle asset and package identified");}
    Register(actor,4);Register(nativeEffect,5);Register(custom,6);Register(foreign,7);
    Put<unsigned char>(actor,0x118,3);Put(actor,0x380,reinterpret_cast<uintptr_t>(mesh));Put(actor,0x158,reinterpret_cast<uintptr_t>(root));
    Put(mesh,0xE8,reinterpret_cast<uintptr_t>(actor));Put(root,0xE8,reinterpret_cast<uintptr_t>(actor));Put(root,0x118,reinterpret_cast<uintptr_t>(children));Put<int>(root,0x120,3);Put<int>(root,0x124,3);
    children[0]=reinterpret_cast<uintptr_t>(nativeEffect);children[1]=reinterpret_cast<uintptr_t>(custom);children[2]=reinterpret_cast<uintptr_t>(foreign);
    for(auto* p:{nativeEffect,custom,foreign}){
        Put(p,0x10,reinterpret_cast<uintptr_t>(componentClass));Put(p,0xE8,reinterpret_cast<uintptr_t>(actor));Put<unsigned char>(p,0x1CC,0x25);
        Put(p,0x108,reinterpret_cast<uintptr_t>(root));Put(p,0x578,reinterpret_cast<uintptr_t>(assets[0]));
    }
    Put(foreign,0xE8,uintptr_t(1));
    g_ready=true;g_spawn=Spawn;g_destroy=Destroy;g_visible=Visible;
    dg::local_players::Context owner{};owner.index=0;owner.pawn=owner.character=reinterpret_cast<uintptr_t>(actor);
    owner.kind=dg::local_players::PoseKind::Foot;auto& rt=dg::runtime::Get();
    rt.leviathanVisualEnabled=false;rt.leviathanVisualMode=0;OnLivePlayer(owner);
    Check(!spawns&&!visibleCalls,"default disabled preserves all native visuals");
    Put(root,0xE8,uintptr_t(1));rt.leviathanVisualEnabled=true;rt.leviathanVisualMode=1;OnLivePlayer(owner);
    Check(!spawns&&!visibleCalls,"a root with foreign ownership cannot receive or hide particles");
    Put(root,0xE8,reinterpret_cast<uintptr_t>(actor));rt.leviathanVisualMode=0;
    rt.leviathanVisualEnabled=true;OnLivePlayer(owner);
    Check(!spawns&&Get<unsigned char>(nativeEffect,0x1CC)==5&&Get<unsigned char>(foreign,0x1CC)==0x25,"OFF hides exact local lightning only, preserving other visibility bits and foreign owners");
    Put(nativeEffect,0x578,uintptr_t(0x1234));g_players[0].tick=0;OnLivePlayer(owner);
    Check(Get<unsigned char>(nativeEffect,0x1CC)==0x25,"native retargeting releases our visibility instead of leaving an unrelated visual hidden");
    Put(nativeEffect,0x578,reinterpret_cast<uintptr_t>(assets[0]));
    rt.leviathanVisualEnabled=false;OnLivePlayer(owner);
    Check(Get<unsigned char>(nativeEffect,0x1CC)==0x25&&!destroys,"disabled restores the borrowed visibility without destroying native effects");
    rt.leviathanVisualEnabled=true;rt.leviathanVisualMode=2;OnLivePlayer(owner);
    Check(spawns==1&&Get<uintptr_t>(custom,0x578)==reinterpret_cast<uintptr_t>(assets[1]),"effect 2 creates a visual independently of the core/status system");
    g_players[0].tick=0;OnLivePlayer(owner);Check(spawns==1,"stable selection never repeatedly spawns");
    rt.leviathanVisualMode=3;OnLivePlayer(owner);Check(spawns==2&&destroys==1,"selection changes replace only the custom visual");
    rt.leviathanVisualEnabled=false;OnLivePlayer(owner);Check(destroys==2&&Get<unsigned char>(nativeEffect,0x1CC)==0x25,"disabled destroys custom visual and restores native effect");
    rt.leviathanVisualEnabled=true;rt.leviathanVisualMode=999;OnLivePlayer(owner);Check(spawns==2,"invalid selection fails open");
    rt.leviathanVisualMode=1;paused=true;OnLivePlayer(owner);Check(spawns==2,"pause never spawns visuals");paused=false;
    owner.index=1;OnLivePlayer(owner);Check(spawns==3,"P2 has an independent owned particle slot");
    Put(items,6*24+16,999);rt.leviathanVisualEnabled=false;OnLivePlayer(owner);Check(destroys==2,"stale component identity cannot destroy a reused object");
    owner.index=0;rt.leviathanVisualEnabled=true;rt.leviathanVisualMode=1;rejectSpawn=true;
    for(int attempt=0;attempt<5;++attempt){g_players[0].tick=0;OnLivePlayer(owner);}
    Check(spawns==6&&Get<unsigned char>(nativeEffect,0x1CC)==0x25,"failed native spawns stop after three attempts and preserve the native visual");
    rt.leviathanVisualEnabled=false;OnLivePlayer(owner);
    wchar_t dir[MAX_PATH]{},path[MAX_PATH]{};Check(GetTempPathW(MAX_PATH,dir)&&GetTempFileNameW(dir,L"DGL",0,path),"temporary INI");
    dg::config::Store a;a.SetPath(path);Check(a.Load()&&!a.leviathanVisualEnabled&&a.leviathanVisualMode==0,"old profile defaults to native appearance");
    a.leviathanVisualEnabled=true;a.leviathanVisualMode=3;a.menuKey=VK_F10;Check(a.SaveNow(),"visual settings save");
    dg::config::Store b;b.SetPath(path);Check(b.Load()&&b.leviathanVisualEnabled&&b.leviathanVisualMode==3&&b.menuKey==VK_F10&&rt.leviathanVisualMode==3,"visual selection, runtime and personal binding survive save/load");
    b.ResetDefaults(false);Check(!b.leviathanVisualEnabled&&b.leviathanVisualMode==0,"reset preserves native behavior");DeleteFileW(path);
}
}
namespace dg::native_tps {bool OnGameThread(){return true;}bool IsWorldPaused(void*){return paused;}bool CinematicActive(unsigned){return false;}}
int main(){try{Run();std::cout<<"Leviathan visual ownership, spawn ABI, restoration and configuration passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
