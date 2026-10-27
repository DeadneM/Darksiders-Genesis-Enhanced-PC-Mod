#include "../src/EffectAppearanceFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::effect_appearance;
constexpr unsigned N=unsigned(Assets.size()),Items=N+4,Names=2*N+8;
struct Memory {unsigned char bytes[0x100]{};};
struct Fixture {std::array<Memory,N> assets{},packages{};Memory classes[3]{},worlds[2]{};unsigned char component[0x800]{};std::array<unsigned char,Items*24> items{};std::array<std::array<unsigned char,1024>,Names> entries{};std::array<uintptr_t,Names> names{};uintptr_t nameChunks[1]{},objectChunks[1]{};} f;
dg::local_players::Context owner{};bool thread=true,failCreate=false;unsigned calls=0,visibleCalls=0;void* sentAsset=nullptr;void* sentWorld=nullptr;void* sentOuter=nullptr;bool sentAuto=false;unsigned char sentPool=0;
template<class T>void Put(void* p,size_t at,const T& value){std::memcpy(static_cast<unsigned char*>(p)+at,&value,sizeof(value));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,unsigned i,const char* text){Put(p,0x18,int(i));strcpy_s(reinterpret_cast<char*>(f.entries[i].data()+12),1012,text);}
Id Live(void* p,unsigned i){Put(p,0xC,int(i));Put(f.items.data(),size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(f.items.data(),size_t(i)*24+16,int(i+100));return {reinterpret_cast<uintptr_t>(p),int(i),int(i+100)};}
void* CreateNative(void* asset,void* world,void* outer,bool autoDestroy,unsigned char pool){++calls;sentAsset=asset;sentWorld=world;sentOuter=outer;sentAuto=autoDestroy;sentPool=pool;if(failCreate)return nullptr;Put(f.component,0x578,reinterpret_cast<uintptr_t>(asset));return f.component;}
void Visibility(void* component,bool show,unsigned char propagation){Check(component==f.component&&propagation==1,"only the returned native PSC visibility changes");++visibleCalls;unsigned char value=0;std::memcpy(&value,f.component+0x1CC,1);value=show?static_cast<unsigned char>(value|0x20):static_cast<unsigned char>(value&~0x20);Put(f.component,0x1CC,value);}
void Fresh(){
 std::memset(&f,0,sizeof(f));owner={};thread=true;failCreate=false;calls=visibleCalls=0;g_assets={};g_particleClass={};g_worlds={};g_hidden={};g_hiddenCount=0;g_changes=0;g_available.reset();g_cursor=g_count=0;g_complete=false;g_generation=0;g_scanTick=g_checkTick=g_restoreTick=0;g_ready=true;g_log=nullptr;g_create=CreateNative;g_visible=Visibility;for(auto& mask:g_loaded)mask=0;
 dg::runtime::Publish(dg::runtime::Snapshot{});
 for(unsigned i=0;i<Names;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i].data());f.nameChunks[0]=reinterpret_cast<uintptr_t>(f.names.data());f.objectChunks[0]=reinterpret_cast<uintptr_t>(f.items.data());
 Put(reinterpret_cast<void*>(g_base),0x3955198,reinterpret_cast<uintptr_t>(f.nameChunks));Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(f.objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,int(Items));
 Label(f.classes[0].bytes,1,"ParticleSystem");Label(f.classes[1].bytes,2,"World");Label(f.classes[2].bytes,3,"ParticleSystemComponent");Live(f.classes[0].bytes,N+2);
 for(unsigned i=0;i<N;++i){auto* a=f.assets[i].bytes;Label(a,4+i,Assets[i].name);Label(f.packages[i].bytes,4+N+i,Assets[i].package);Put(a,0x10,reinterpret_cast<uintptr_t>(f.classes[0].bytes));Put(a,0x20,reinterpret_cast<uintptr_t>(f.packages[i].bytes));Live(a,i);}
 for(unsigned i=0;i<2;++i){Put(f.worlds[i].bytes,0x10,reinterpret_cast<uintptr_t>(f.classes[1].bytes));Live(f.worlds[i].bytes,N+i);}
 Put(f.component,0x10,reinterpret_cast<uintptr_t>(f.classes[2].bytes));Put<unsigned char>(f.component,0x1CC,0x20);Live(f.component,N+3);
 owner.index=0;owner.localPlayer=1;owner.controller=2;owner.pawn=3;owner.world=reinterpret_cast<uintptr_t>(f.worlds[0].bytes);
}
void Select(unsigned family,int choice){dg::runtime::Snapshot s{};s.effectAppearances[family]=choice;dg::runtime::Publish(s);OnLivePlayer(owner);}
unsigned Find(const char* name){for(unsigned i=0;i<N;++i)if(!Compare(Assets[i].name,name))return i;throw std::runtime_error("missing fixture asset name");}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Fresh();
 Check(HookCreate(f.assets[0].bytes,f.worlds[0].bytes,f.packages[0].bytes,false,7)==f.component&&calls==1&&sentAsset==f.assets[0].bytes&&sentWorld==f.worlds[0].bytes&&sentOuter==f.packages[0].bytes&&!sentAuto&&sentPool==7,"all Native forwards the exact factory ABI and return");
 OnLivePlayer(owner);Check(g_cursor==0&&g_particleClass.address==0,"all Native performs no discovery");Select(0,1);Check(g_cursor==int(Items)&&g_available.all(),"bounded 16-record discovery accepts all checked package/name/class assets");
 const auto assets=f.assets,packages=f.packages;const auto items=f.items;
 unsigned checked=0;
 for(unsigned source=0;source<N;++source){const auto family=Roles[Assets[source].role].family;
  for(int choice=0;choice<Hidden(family);++choice){dg::runtime::Snapshot s{};s.effectAppearances[family]=choice;dg::runtime::Publish(s);g_worlds[0].tick=GetTickCount64();const auto before=calls;HookCreate(f.assets[source].bytes,f.worlds[0].bytes,f.packages[2].bytes,true,3);const int target=Target(Assets[source].role,choice);
   Check(calls==before+1&&sentAsset==(target>=0?f.assets[unsigned(target)].bytes:f.assets[source].bytes)&&sentWorld==f.worlds[0].bytes&&sentOuter==f.packages[2].bytes&&sentAuto&&sentPool==3,"every role/preset changes only the intended template; one native call, original ABI");++checked;
  }
 }
 Check(!std::memcmp(assets.data(),f.assets.data(),sizeof(f.assets))&&!std::memcmp(packages.data(),f.packages.data(),sizeof(f.packages))&&items==f.items,"no shared asset, package or object-table mutation");
 // Persisted V1.115 Caltrops choice numbers have exactly their old targets.
 const unsigned glyph=Find("P_Strife_Caltrop_Glyph_Init_1B"),blast=Find("P_Strife_Caltrop_Trigger_1B");
 for(int choice=1;choice<=4;++choice){Check(Target(Assets[glyph].role,choice)==int(Find(choice==1?"P_Strife_Caltrop_Glyph_Init_1B":"P_Strife_Caltrop_Glyph_Init_Frost_01")),"Caltrops glyph numbering preserved");char expected[96]{};if(choice==1)strcpy_s(expected,"P_Strife_Caltrop_Trigger_1B");else sprintf_s(expected,"P_Strife_Caltrop_Trigger_Frost_Lvl%d",choice-1);Check(Target(Assets[blast].role,choice)==int(Find(expected)),"Caltrops explosion numbering preserved");}
 Select(0,2);void* unknown=reinterpret_cast<void*>(0x1234);HookCreate(unknown,f.worlds[0].bytes,nullptr,true,0);Check(sentAsset==unknown,"unknown assets pass through without dereferencing them");
 HookCreate(f.assets[glyph].bytes,f.worlds[1].bytes,nullptr,true,0);Check(sentAsset==f.assets[glyph].bytes,"unpublished worlds retain native visuals");
 thread=false;HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Check(sentAsset==f.assets[glyph].bytes,"worker thread never substitutes");thread=true;
 g_worlds[0].tick=GetTickCount64()-501;HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Check(sentAsset==f.assets[glyph].bytes,"stale world fails open");OnLivePlayer(owner);
 const auto frost=Find("P_Strife_Caltrop_Glyph_Init_Frost_01");Put(f.items.data(),size_t(frost)*24+16,999);HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Check(sentAsset==f.assets[glyph].bytes,"expired selected asset preserves native");
 Fresh();Select(0,Hidden(0));const auto before=calls;HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,5);Check(calls==before+1&&sentAsset==f.assets[glyph].bytes&&visibleCalls==1&&g_hiddenCount==1,"Hidden still creates the original native component with identical asset and lifetime");
 Select(0,0);g_restoreTick=0;OnLivePlayer(owner);Check(visibleCalls==2&&!g_hiddenCount,"return to Native restores only the visibility changed by this module");
 Select(0,Hidden(0));HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);dg::runtime::Publish(dg::runtime::Snapshot{});const auto oldCalls=visibleCalls;HookCreate(unknown,f.worlds[0].bytes,nullptr,true,0);Check(visibleCalls==oldCalls+1&&!g_hiddenCount,"native pool reuse cannot carry over the previous mod-hidden flag");
 Fresh();Select(0,Hidden(0));failCreate=true;HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Check(!g_hiddenCount&&!visibleCalls,"failed factory produces no visibility side effect");
 Fresh();Select(0,Hidden(0));Put<unsigned char>(f.component,0x1CC,0);HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Select(0,0);g_restoreTick=0;OnLivePlayer(owner);Check(!visibleCalls,"already native-hidden components are never made visible");
 Fresh();dg::runtime::Snapshot invalid{};for(auto& x:invalid.effectAppearances)x=999;dg::runtime::Publish(invalid);Check(!dg::runtime::Get().effectAppearanceMask.load(),"all unsupported config choices normalize to Native");
 Select(0,Hidden(0));HookCreate(f.assets[glyph].bytes,f.worlds[0].bytes,nullptr,true,0);Shutdown();Check(!g_hiddenCount&&visibleCalls==2,"game-thread shutdown restores owned visibility");
 std::cout<<FamilyCount<<" families, "<<N<<" assets, "<<checked<<" preset cases; Native ABI, expiry, pool reuse and visibility restoration passed\n";
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
