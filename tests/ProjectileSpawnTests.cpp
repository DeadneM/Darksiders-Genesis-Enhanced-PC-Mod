#include "../src/ProjectileAimFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
alignas(16) unsigned char character[0xD00]{},weapon[0x200]{},grenade[0x200]{},cls[0x80]{},foreignClass[0x80]{};
void* tree[1]{cls+0x30};void* foreignTree[1]{foreignClass+0x30};
bool providerEnabled=true,gameThread=true,paused=false;
dg::local_players::Context context{};
const void* forwarded=nullptr;
template<class T> void Put(void* p,size_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
void Check(bool b,const char* msg){if(!b)throw std::runtime_error(msg);}
dg::projectile_aim::Rotator* Original(void*,dg::projectile_aim::Rotator* out,const void* in){forwarded=in;*out={4,23,7};return out;}
bool Provider(const dg::local_players::Context& c,float& pitch,float& yaw){
    Check(c.character==reinterpret_cast<uintptr_t>(character),"provider receives current native owner");
    pitch=-12;yaw=117;return providerEnabled;
}
void Run(){
    std::vector<unsigned char> image(0x3900000);
    dg::projectile_aim::g_base=reinterpret_cast<uintptr_t>(image.data());
    Put(image.data(),0x380C0E0,static_cast<void*>(cls));Put(cls,0x30,tree+0);Put(cls,0x38,0);
    Put(foreignClass,0x30,foreignTree+0);Put(foreignClass,0x38,0);
    Put(weapon,0x10,static_cast<void*>(cls));Put(weapon,0xE8,static_cast<void*>(character));
    Put(grenade,0x10,static_cast<void*>(foreignClass));Put(grenade,0xE8,static_cast<void*>(character));
    character[0xC18]=1;
    context.index=0;context.character=reinterpret_cast<uintptr_t>(character);
    dg::projectile_aim::g_original=Original;dg::projectile_aim::g_provider=Provider;
    dg::projectile_aim::Rotator out{},in{44,55,66};
    auto* result=dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(result==&out&&out.pitch==-12&&out.yaw==117&&out.roll==7,"first projectile uses full emitted pitch/yaw, native roll and return ABI");
    Check(forwarded==&in&&in.pitch==44&&in.yaw==55&&in.roll==66&&character[0]==0,"native input is forwarded and actor memory stays untouched");
    providerEnabled=false;dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(out.pitch==4&&out.yaw==23,"mouse, vanilla aim and inactive views remain native when provider rejects");providerEnabled=true;
    dg::projectile_aim::Rotation(grenade,&out,&in,0x7CFA3B);
    Check(out.yaw==23,"gear/grenades outside native pistol ancestry stay native");
    dg::projectile_aim::Rotation(weapon,&out,&in,0x123);
    Check(out.yaw==23,"unproved caller stays native");
    context.index=1;dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(out.yaw==117&&dg::projectile_aim::g_adjusted[1]==1,"P2 gets its own provider context");
    character[0xC18]=0;dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(out.yaw==23,"War is excluded");character[0xC18]=1;
    paused=true;dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(out.yaw==23,"pause cannot correct projectiles");paused=false;
    gameThread=false;dg::projectile_aim::Rotation(weapon,&out,&in,0x7CFA3B);
    Check(out.yaw==23,"other threads stay native");
}
}
namespace dg::native_tps {
bool OnGameThread(){return gameThread;}
bool CinematicActive(unsigned){return false;}
bool IsWorldPaused(void*){return paused;}
}
namespace dg::local_players {
bool ResolveActor(void* a,Context& c){if(a!=character)return false;c=context;return true;}
}
int main(){try{Run();std::cout<<"Actual native projectile return ABI and local pistol scopes passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
