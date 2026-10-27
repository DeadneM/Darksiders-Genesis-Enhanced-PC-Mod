#include "../src/CaltropsAppearanceFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::caltrops_appearance;
struct Fixture {unsigned char assets[6][0x100]{},packages[6][0x100]{},classes[2][0x80]{},worlds[2][0x100]{},items[16*24]{},entries[32][256]{};uintptr_t names[32]{},nameChunks[1]{},objectChunks[1]{};} f;
dg::local_players::Context c{};bool thread=true;unsigned calls=0;void* sentAsset=nullptr;void* sentWorld=nullptr;void* sentOuter=nullptr;bool sentAuto=false;unsigned char sentPool=0;
template<class T>void Put(void* p,size_t at,const T& x){std::memcpy(static_cast<unsigned char*>(p)+at,&x,sizeof(x));}
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
void Label(void* p,int i,const char* text){Put(p,0x18,i);strcpy_s(reinterpret_cast<char*>(f.entries[i]+12),244,text);}
Id Live(void* p,int i){Put(p,0xC,i);Put(f.items,size_t(i)*24,reinterpret_cast<uintptr_t>(p));Put(f.items,size_t(i)*24+16,100+i);return {reinterpret_cast<uintptr_t>(p),i,100+i};}
void* CreateNative(void* asset,void* world,void* outer,bool autoDestroy,unsigned char pooling){++calls;sentAsset=asset;sentWorld=world;sentOuter=outer;sentAuto=autoDestroy;sentPool=pooling;return reinterpret_cast<void*>(0xBEEF);}
void Fresh(){
 f={};c={};thread=true;calls=0;g_assets={};g_worlds={};g_selection=g_cursor=g_count=0;g_complete=false;g_scanTick=0;g_loaded=0;g_changes=0;g_log=nullptr;g_ready=true;g_create=CreateNative;dg::runtime::Get().caltropsAppearance=0;
 for(int i=0;i<32;++i)f.names[i]=reinterpret_cast<uintptr_t>(f.entries[i]);f.nameChunks[0]=reinterpret_cast<uintptr_t>(f.names);f.objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);
 Put(reinterpret_cast<void*>(g_base),0x3955198,reinterpret_cast<uintptr_t>(f.nameChunks));Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(f.objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,8);
 Label(f.classes[0],1,"ParticleSystem");Label(f.classes[1],2,"World");
 for(unsigned i=0;i<6;++i){Label(f.assets[i],3+int(i),Assets[i].name);Label(f.packages[i],9+int(i),Assets[i].package);Put(f.assets[i],0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Put(f.assets[i],0x20,reinterpret_cast<uintptr_t>(f.packages[i]));Live(f.assets[i],int(i));}
 for(int i=0;i<2;++i){Put(f.worlds[i],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));Live(f.worlds[i],6+i);}
 c.index=0;c.localPlayer=1;c.controller=2;c.pawn=3;c.world=reinterpret_cast<uintptr_t>(f.worlds[0]);
}
void Run(){
 std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Fresh();
 Check(HookCreate(f.assets[0],f.worlds[0],f.packages[0],false,7)==reinterpret_cast<void*>(0xBEEF)&&sentAsset==f.assets[0]&&sentWorld==f.worlds[0]&&sentOuter==f.packages[0]&&!sentAuto&&sentPool==7,"Native fast path preserves all five arguments and return, including pooling");
 OnLivePlayer(c);Check(g_cursor==0&&!LoadedMask(),"Native does not start a loaded-object scan");
 dg::runtime::Get().caltropsAppearance=2;OnLivePlayer(c);Check(LoadedMask()==30&&g_cursor==8,"bounded real name/package discovery supplies the four choices");
 const auto memory=std::vector<unsigned char>(reinterpret_cast<unsigned char*>(&f),reinterpret_cast<unsigned char*>(&f)+sizeof(f));
 for(int choice=1;choice<=4;++choice)for(int source=0;source<6;++source){dg::runtime::Get().caltropsAppearance=choice;const auto before=calls;auto* result=HookCreate(f.assets[source],f.worlds[0],f.packages[2],true,3);Check(result==reinterpret_cast<void*>(0xBEEF)&&calls==before+1&&sentAsset==f.assets[Target(choice,source)]&&sentWorld==f.worlds[0]&&sentOuter==f.packages[2]&&sentAuto&&sentPool==3,"one unchanged native factory call, selected glyph/explosion, same world/outer/auto/pool");}
 Check(!std::memcmp(memory.data(),&f,sizeof(f)),"assets, worlds, object table and source data remain byte-identical");
 auto* unknown=reinterpret_cast<void*>(0x1234);HookCreate(unknown,f.worlds[0],nullptr,true,0);Check(sentAsset==unknown,"all unrelated emitters pass through without reading the unknown asset");
 HookCreate(f.assets[2],f.worlds[1],nullptr,true,0);Check(sentAsset==f.assets[2],"unpublished world cannot inherit the cosmetic override");
 c.index=1;c.world=reinterpret_cast<uintptr_t>(f.worlds[1]);OnLivePlayer(c);HookCreate(f.assets[2],f.worlds[1],nullptr,true,0);Check(sentAsset==f.assets[5],"second local world is independently scoped");
 thread=false;HookCreate(f.assets[2],f.worlds[1],nullptr,true,0);Check(sentAsset==f.assets[2],"worker-thread emitter is untouched");thread=true;
 g_worlds[1].tick=GetTickCount64()-501;HookCreate(f.assets[2],f.worlds[1],nullptr,true,0);Check(sentAsset==f.assets[2],"expired local-world snapshot fails open");
 Fresh();dg::runtime::Get().caltropsAppearance=4;OnLivePlayer(c);Put(f.items,5*24+16,999);HookCreate(f.assets[2],f.worlds[0],nullptr,true,0);Check(sentAsset==f.assets[2],"expired replacement asset retains native particles");
 Fresh();dg::runtime::Get().caltropsAppearance=2;Put(f.assets[1],0x20,reinterpret_cast<uintptr_t>(f.packages[0]));OnLivePlayer(c);Check(!(LoadedMask()&4),"same name with wrong package is not an accepted glyph");HookCreate(f.assets[0],f.worlds[0],nullptr,true,0);Check(sentAsset==f.assets[0],"missing selected glyph keeps its original");
 Fresh();dg::runtime::Snapshot snapshot{};snapshot.caltropsAppearance=99;dg::runtime::Publish(snapshot);Check(dg::runtime::Get().caltropsAppearance==0&&Normalize(-1)==0&&Normalize(5)==0,"invalid persisted/runtime choice normalizes to Native");
 dg::runtime::Get().caltropsAppearance=3;OnLivePlayer(c);Shutdown();HookCreate(f.assets[2],f.worlds[0],nullptr,true,0);Check(sentAsset==f.assets[2],"shutdown restores future native effects with no persistent changes to undo");
}
}
namespace dg::native_tps {bool OnGameThread(){return thread;}}
int main(){try{Run();std::cout<<"Caltrops appearance ABI, template-only substitution, bounded discovery, Native fallback and world isolation passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
