#pragma once
namespace dg::gameplay_menu {
struct Focus {
    bool window=false,foreground=false,minimized=false,closing=false,menu=false;
};
// Opening our editor does not change the game's gameplay rules. Native pause,
// radial and cinematic ownership are still checked by each native feature.
inline constexpr bool Runtime(const Focus& s){return s.window&&s.foreground&&!s.minimized&&!s.closing;}
// UI navigation, rebinding and new mod commands must not share input edges.
inline constexpr bool Commands(const Focus& s){return Runtime(s)&&!s.menu;}
// Function keys are not editor navigation. Only text entry or rebinding owns
// them; displaying the editor alone must not suspend configured mod actions.
inline constexpr bool Hotkeys(const Focus& s,bool capturingKeys){return Runtime(s)&&!capturingKeys;}
struct OrbitDelta {float yaw=0,pitch=0;};
inline constexpr OrbitDelta Orbit(const Focus& s,float padYaw,float padPitch,float mouseYaw,float mousePitch){
    if(!Runtime(s))return {};
    return {padYaw+(s.menu?0:mouseYaw),padPitch+(s.menu?0:mousePitch)};
}
}
