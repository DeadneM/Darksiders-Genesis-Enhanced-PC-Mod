#include "CoopCameraFeature.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "TpsCameraPitch.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace {
unsigned char g_character[0x1000]{},g_respawn[0x1000]{};
dg::local_players::Context g_owner{1,1,2,0x1000,0x2000,reinterpret_cast<uintptr_t>(g_character),reinterpret_cast<uintptr_t>(g_character),0x3000};
dg::local_players::Pose g_pose{2000,3000,100,50};
bool g_cinema=false,g_paused=false,g_swap=false,g_known=true,g_radial[2]{};
void Check(bool v,const char* msg){if(!v)throw std::runtime_error(msg);}
bool Equal(const XINPUT_GAMEPAD& a,const XINPUT_GAMEPAD& b){return std::memcmp(&a,&b,sizeof(a))==0;}
bool Close(float a,float b){return std::fabs(a-b)<0.001f;}
void Native(float* out){const float view[7]={0,0,2200,-45,10,0,70};std::memcpy(out,view,sizeof(view));}
void View(float* out){Native(out);dg::coop_camera::ApplyView(reinterpret_cast<void*>(0x4000),out,g_owner);}
void Input(const XINPUT_GAMEPAD* p,float dt=0.016f,bool focused=true){dg::coop_camera::UpdateInput(p,focused,dt,0x20,0x40,0);}
void Run(){
    auto& rt=dg::runtime::Get();rt.thirdPersonEnabled=true;rt.cameraControllerOrbitEnabled=true;
    rt.cameraOrbitInputEnabled=true;rt.cameraStickSpeed=180;rt.tpsCameraStickMode=2;
    rt.tpsFirePrimaryMask=0x20000;rt.tpsFireSecondaryMask=0x10;rt.tpsWheelPadMask=0x100;
    rt.tpsMountedAimMode=0;rt.tpsCutsceneAutoSuspend=true;
    rt.thirdPersonFovEnabled=true;rt.thirdPersonFovDegrees=120;rt.thirdPersonCoopFovDegrees=83;
    rt.cameraOrbitYawDegrees=177;rt.cameraOrbitPitchDegrees=22;
    g_character[0xC18]=1;g_respawn[0xC18]=1;
    dg::coop_camera::Initialize(nullptr);
    for(float framing:{-30.0f,-12.0f,15.0f}){
        const auto neutral=dg::tps_camera::CameraPitch(framing,0,true,0);
        Check(Close(neutral.boom,framing)&&Close(neutral.view,0),"3D recenter levels the rendered view without changing camera position/framing");
        for(float tilt:{-20.0f,0.0f,25.0f}){
            const auto spatial=dg::tps_camera::CameraPitch(framing,tilt,true,0);
            const auto simple=dg::tps_camera::CameraPitch(framing,tilt,false,0);
            Check(Close(spatial.view,tilt)&&Close(spatial.boom,simple.boom)&&Close(simple.view,simple.boom),"up/down aim remains coherent and simple mode restores the previous entire view");
        }
        Check(Close(dg::tps_camera::CameraPitch(framing,0,true,1).view,framing),"Vanilla Aim keeps its previous orientation");
    }
    float view[7]{};View(view);
    Check(Close(view[3],0),"actual recentered P2 view is level in 3D mode");
    const float savedX=view[0],savedZ=view[2];rt.tps3DAimEnabled=false;View(view);
    Check(Close(view[3],rt.thirdPersonPitchDegrees.load())&&Close(view[0],savedX)&&Close(view[2],savedZ),"simple P2 mode restores old orientation, position untouched");
    rt.tps3DAimEnabled=true;View(view);
    Check(Close(view[4],50)&&view[0]>1000&&view[1]>2000,"P2 view follows its own pose, not primary/origin");
    Check(Close(view[6],83)&&Close(rt.thirdPersonFovDegrees.load(),120),"P2 uses coop FOV and preserves independent solo setting");
    rt.thirdPersonFovEnabled=false;View(view);
    Check(Close(view[6],70),"disabling TPS FOV preserves native split-screen FOV");
    rt.thirdPersonFovEnabled=true;
    XINPUT_GAMEPAD pad{};pad.sThumbRX=30000;Input(&pad);View(view);
    Check(view[4]>50&&Close(rt.cameraOrbitYawDegrees.load(),177)&&Close(rt.cameraOrbitPitchDegrees.load(),22),"P2 orbit cannot move primary orbit");
    auto unchanged=pad;
    Check(!dg::coop_camera::FilterPad(0,pad)&&Equal(pad,unchanged),"P2 cannot filter primary input");
    Check(dg::coop_camera::FilterPad(1,pad)&&pad.sThumbRX==0,"P2 native aim stick isolated from its camera");
    pad=unchanged;pad.wButtons=0x100;Input(&pad);const auto wheel=pad;
    Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,wheel),"rebindable wheel preserves native selection axes/buttons");
    pad=unchanged;pad.wButtons=0x10;Input(&pad);View(view);const auto buttons=pad.wButtons;
    Check(dg::coop_camera::FilterPad(1,pad)&&pad.sThumbRX==0&&pad.sThumbRY==32767&&pad.wButtons==buttons,"rebindable secondary fire points forward without modifying buttons");
    float x=0,y=0,z=0;
    Check(dg::coop_camera::AdjustLook(g_character,x,y,z,true)&&Close(y,view[4]),"P2 firing uses its own emitted yaw");
    dg::native_tps::MouseRay shot{};
    Check(dg::coop_camera::ShotRay(g_owner,shot)&&Close(shot.origin.x,view[0])&&Close(shot.direction.z,std::sin(view[3]*0.01745329251994329577f)),"P2 projectile uses its emitted 3D camera orientation at spawn time");
    rt.tps3DAimEnabled=false;Check(!dg::coop_camera::ShotRay(g_owner,shot),"simple P2 mode never overrides native projectile trajectories");rt.tps3DAimEnabled=true;
    g_radial[1]=true;pad=unchanged;Input(&pad);const auto nativeWheel=pad;
    Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,nativeWheel),"actual native P2 selector protects axes even without the configured button");
    g_radial[1]=false;pad={};Input(&pad);View(view);
    Check(dg::coop_camera::ShotRay(g_owner,shot),"native Finish may aim after the physical trigger was released");
    rt.tpsMountedAimMode=1;
    Check(!dg::coop_camera::ShotRay(g_owner,shot),"P2 vanilla aim cannot enter camera-forward projectile correction");rt.tpsMountedAimMode=0;
    g_known=false;pad=unchanged;Input(&pad);const auto unknown=pad;
    Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,unknown),"unknown native stick layout fails open");g_known=true;
    rt.tpsCameraStickMode=1;pad={};pad.sThumbLX=30000;pad.sThumbRX=12000;Input(&pad);View(view);
    Check(dg::coop_camera::FilterPad(1,pad)&&pad.sThumbLX==12000&&pad.sThumbRX==0,"camera-left exchanges full stick roles on P2");
    rt.tpsCameraStickMode=2;pad={};pad.wButtons=0x40;Input(&pad);View(view);
    Check(Close(view[0],0)&&Close(view[3],-45)&&!rt.tpsTemporarilySuspended.load(),"P2 temporary native view is private and uses configured binding");
    pad={};Input(&pad);View(view);const float priorYaw=view[4];
    g_paused=true;pad.sThumbRX=32000;View(view);Input(&pad);View(view);
    Check(Close(view[4],priorYaw)&&view[0]>1000,"pause holds P2 composition without orbit accumulation");g_paused=false;
    g_cinema=true;View(view);Check(Close(view[3],-45)&&Close(view[4],10),"native cinematic owns P2 view");
    pad=unchanged;Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,unchanged),"cinematic leaves P2 input native");g_cinema=false;
    pad={};Input(&pad);View(view);
    pad.wButtons=0x20;Input(&pad);View(view);Check(Close(view[4],g_pose.yaw),"P2 recenter follows its own actor");
    g_owner.character=reinterpret_cast<uintptr_t>(g_respawn);g_owner.pawn=g_owner.character;g_pose={8000,9000,200,-70};
    View(view);Check(Close(view[4],-70)&&view[0]>7000&&Close(rt.cameraOrbitYawDegrees.load(),177),"P2 respawn resets only its own held view/orbit");
    g_character[0xC18]=2;g_owner.character=reinterpret_cast<uintptr_t>(g_character);g_owner.pawn=g_owner.character;g_pose.yaw=130;
    View(view);Check(Close(view[4],-70),"switching Strife/War retains world camera angle for the same local player");
    // Recenter sharing a long-press binding follows the same release policy as P1.
    pad={};pad.wButtons=0x20;
    Check(!dg::coop_camera::UpdateInput(&pad,true,0.016f,0x20,0x40,0,0x20,500),"short P2 press does not toggle the shared setting");
    View(view);Check(Close(view[4],-70),"P2 does not recenter before knowing whether a shared press is long");
    pad={};dg::coop_camera::UpdateInput(&pad,true,0.016f,0x20,0x40,0,0x20,500);View(view);
    Check(Close(view[4],130),"P2 short release recenters its own view");
    // Equipment uses the shared policy even while its same button owns temporary native view.
    rt.tpsGrenadeButtonMask=0x4000;pad={};pad.wButtons=0x4000;
    dg::coop_camera::UpdateInput(&pad,true,0.016f,0x20,0x4000,0);View(view);
    Check(Close(view[4],10),"P2 temporary view remains private while equipment is physically held");
    Check(dg::coop_camera::FilterPad(1,pad)&&(pad.wButtons&0x80)&&!(pad.wButtons&0x4000),"P2 forwards configured equipment hold before the temporary-view gate");
    pad={};pad.wButtons=0x4000;
    Check(dg::coop_camera::FilterPad(1,pad,false)&&pad.wButtons==0x4000,"overlay or focus loss cannot emit equipment pulses");
    pad={};Input(&pad);View(view);
    rt.horsePadButtonA=0x1000;rt.horsePadButtonB=0x4000;rt.horseEverywhereEnabled=true;
    pad={};pad.wButtons=0x1000|0x4000;Input(&pad);View(view);
    Check(dg::coop_camera::FilterPad(1,pad)&&(pad.wButtons&0x300)==0x300&&rt.horseSummonIntentTick.load(),"P2 uses the configured two-button horse action and native pre-arm");
    rt.horsePadButtonA=0x100;rt.horsePadButtonB=0x200;rt.tpsGrenadeButtonMask=0x80;
    pad=unchanged;Input(&pad,0.016f,false);const auto unfocused=pad;
    Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,unfocused),"focus loss preserves native input");
    rt.thirdPersonEnabled=false;Input(&pad);const auto off=pad;
    Check(dg::coop_camera::FilterPad(1,pad)&&Equal(pad,off),"top-down view preserves native aiming and dash input");
}
}
// Read-only native boundaries are replaced only for this unit executable.
// The actual camera/input implementation is compiled and exercised above.
namespace dg::local_players {
Slot PublishedSlot(unsigned index){if(index==1)return {g_owner.localPlayer,g_owner.character,g_owner.controllerId,GetTickCount64()};return {};}
bool ReadPose(const Context&,Pose& p){p=g_pose;return true;}
bool ResolveActor(void* actor,Context& c){if(reinterpret_cast<uintptr_t>(actor)!=g_owner.character)return false;c=g_owner;return true;}
}
namespace dg::native_tps {
bool IsWorldPaused(void*){return g_paused;}
bool RadialActive(unsigned index){return index<2&&g_radial[index];}
bool CinematicActive(unsigned){return g_cinema;}
bool NativeStickSwap(){return g_swap;}
bool NativeStickSwapKnown(){return g_known;}
}
int main(){try{Run();std::cout<<"Actual coop view/input ownership isolation checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
