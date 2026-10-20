#include "../src/TpsCameraFocus.h"
#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
namespace {
using View=std::array<unsigned char,0x580>;
void Check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
template<class T> void Put(View& v,size_t at,T value){std::memcpy(v.data()+at,&value,sizeof(value));}
float Focus(const View& v){float f=0;std::memcpy(&f,v.data()+0x494,4);return f;}
View Native(){
    View v{};v.fill(0xA5);float frame[7]{0,0,0,0,0,0,90};
    std::memcpy(v.data(),frame,sizeof(frame));Put(v,0x38,1.0f);Put(v,0x494,1000.0f);return v;
}
struct Reader {
        std::array<unsigned char,0x400> actor{},mesh{};
        template<class T> bool Read(uintptr_t object,size_t offset,T& result){
            const auto a=reinterpret_cast<uintptr_t>(actor.data()),m=reinterpret_cast<uintptr_t>(mesh.data());
            if((object!=a&&object!=m)||offset>0x400||sizeof(T)>0x400-offset)return false;
            std::memcpy(&result,reinterpret_cast<const void*>(object+offset),sizeof(T));return true;
        }
};
void Run(){
    auto v=Native(),before=v;
    Check(!dg::camera_focus::Compensate(v.data(),{300,100,20})&&v==before,"unknown layout never changes a native frame");
    dg::camera_focus::layoutReady=true;
    Check(dg::camera_focus::Compensate(v.data(),{300,400,0})&&std::fabs(Focus(v)-300)<.001f,
        "focus follows forward depth, not 500-unit slant distance");
    for(size_t i=0;i<v.size();++i)
        if(i!=0x56&&(i<0x494||i>=0x498))Check(v[i]==before[i],"all non-focus bytes, blend and authored blur settings survive");
    Check(v[0x56]==(before[0x56]|0x08),"only the native focal-distance override bit is added");
    // Retail render blend 1B2F693 consumes bit 0x08 for FocalDistance;
    // bit 0x10 belongs to Fstop. Reproduce that consumer, not our setter.
    auto renderFocus=[](const View& frame){return frame[0x56]&8?Focus(frame):1000.0f;};
    v=Native();v[0x56]=0x10;Put(v,0x494,365.0f);
    Check(renderFocus(v)==1000,"V1.56's wrong flag leaves the render focus native");
    Check(dg::camera_focus::Compensate(v.data(),{365,0,0})&&renderFocus(v)==365,
        "corrected override actually reaches the native focal-distance branch");
    Check(v[0x56]==0x18,"an authored aperture override is preserved");
    v=Native();v[0x56]=0;Put(v,0x340,2.8f);
    dg::camera_focus::Compensate(v.data(),{365,0,0});
    Check(v[0x56]==8&&renderFocus(v)==365,"fresh native frame activates focus alone, never aperture");
    v=Native();Put(v,16,90.0f);Check(dg::camera_focus::Compensate(v.data(),{0,275,0})&&std::fabs(Focus(v)-275)<.001f,"yaw follows the emitted TPS view");
    v=Native();Put(v,12,-30.0f);Check(dg::camera_focus::Compensate(v.data(),{259.80762f,0,-150})&&std::fabs(Focus(v)-300)<.01f,"pitched view uses its own forward plane");
    v=Native();before=v;Check(!dg::camera_focus::Compensate(v.data(),{-100,0,0})&&v==before,"player behind camera cannot create a negative focal plane");
    for(float weight:{0.0f,.5f,2.0f,std::numeric_limits<float>::quiet_NaN()}){
        v=Native();Put(v,0x38,weight);before=v;
        Check(!dg::camera_focus::Compensate(v.data(),{300,0,0})&&v==before,"partial or invalid weights are never raised");
    }
    v=Native();before=v;Check(!dg::camera_focus::Compensate(v.data(),{std::numeric_limits<float>::infinity(),0,0})&&v==before,"invalid pose stays native");
    auto p1=Native(),p2=Native();dg::camera_focus::Compensate(p1.data(),{300,0,0});
    Check(Focus(p2)==1000,"P1 output cannot modify P2");dg::camera_focus::Compensate(p2.data(),{700,0,0});
    Check(Focus(p1)==300&&Focus(p2)==700,"split-screen focus planes are independent");
    Check(Focus(Native())==1000,"fresh native frames retain authored focus without a restore write");
    auto legacy=Native();legacy[0x63]=1;legacy[0x58]=4;Put(legacy,0x340,2.8f);
    auto missingLayout=legacy;
    dg::camera_focus::Compensate(legacy.data(),{400,0,0},false);
    dg::camera_focus::Compensate(missingLayout.data(),{400,0,0},true);
    Check(legacy==missingLayout,"unverified lens layout falls back exactly to the V1.60 focus path");
    dg::camera_focus::lensLayoutReady=true;
    auto lens=Native();lens[0x63]=1;lens[0x58]=4;lens[0x57]=4;Put(lens,0x4A0,0.0f);Put(lens,0x340,2.8f);before=lens;
    // Native level method and all far-blur fields must survive. Only the
    // close Gaussian layer and the character's sharp region are adapted.
    const dg::camera_focus::Bounds body{{400,0,0},{120,80,100},true};
    lens=before;Check(dg::camera_focus::Compensate(lens.data(),{400,0,0},true,body),"native focus band accepts a live mesh");
    Check(lens[0x63]==before[0x63]&&!(lens[0x58]&2),"the level's method is retained, never forced to CircleDOF");
    float nearSize=1,region=0;std::memcpy(&nearSize,lens.data()+0x4B4,4);std::memcpy(&region,lens.data()+0x4A0,4);
    Check(nearSize==0&&(lens[0x57]&0x80)&&std::fabs(region-120)<.001f&&(lens[0x57]&4),"the whole mesh behind the pivot is sharp; front Gaussian blur is disabled");
    Check(Focus(lens)==400,"the validated character focus plane is preserved");
    for(size_t i=0;i<lens.size();++i)
        if(i!=0x56&&i!=0x57&&(i<0x494||i>=0x498)&&(i<0x4A0||i>=0x4A4)&&(i<0x4B4||i>=0x4B8))
            Check(lens[i]==before[i],"far blur size, far transition, method, aperture, weight and geometry remain authored");
    auto off=before;dg::camera_focus::Compensate(off.data(),{400,0,0},false,body);
    auto focusOnly=before;dg::camera_focus::Compensate(focusOnly.data(),{400,0,0});
    Check(off==focusOnly,"OFF remains byte-identical to the previous focal-distance-only policy");
    auto unknown=before;unknown[0x63]=9;dg::camera_focus::Compensate(unknown.data(),{400,0,0},true,body);
    auto nativeUnknown=before;nativeUnknown[0x63]=9;dg::camera_focus::Compensate(nativeUnknown.data(),{400,0,0},false);
    Check(unknown==nativeUnknown,"unknown native methods retain the focal-distance-only fallback");
    auto missing=before;missing[0x57]=0;Put(missing,0x4A0,3400.0f);
    dg::camera_focus::Compensate(missing.data(),{400,0,0},true);
    Check(missing[0x57]==0x80,"missing bounds cannot override the native focal region");
    auto hub=before;hub[0x57]=0;Put(hub,0x4A0,0.0f);
    dg::camera_focus::Compensate(hub.data(),{400,0,0},true,body);
    const auto volumeRegion=[](const View& frame){float f=0;std::memcpy(&f,frame.data()+0x4A0,4);return frame[0x57]&4?f:3000.0f;};
    Check(volumeRegion(hub)==3000,"hub/arene regression: mesh bounds cannot replace a level's 30m clear region with centimetres");
    auto stale=before;stale[0x57]=0;dg::camera_focus::Compensate(stale.data(),{400,0,0},true,{{20000,0,0},{120,80,100},true});
    Check(stale[0x57]==0x80,"bounds of a different pose cannot alter the region during a swap");
    auto authored=before;authored[0x57]=4;Put(authored,0x4A0,3400.0f);
    dg::camera_focus::Compensate(authored.data(),{400,0,0},true,body);std::memcpy(&region,authored.data()+0x4A0,4);
    Check(region==3400,"an explicitly wider camera region is never shortened");
    // Gaussian region invariant; this is not a GPU rendering test. The far
    // layer ramps over FarTransitionRegion and retains its authored size.
    auto farAmount=[](float z,float focus,float sharpRegion,float transition){return std::clamp((z-focus-sharpRegion)/transition,0.0f,1.0f);};
    Check(farAmount(520,400,120,500)==0&&farAmount(770,400,120,500)==.5f&&farAmount(1020,400,120,500)==1,
        "the rear of the character is protected without removing the distant layer");
    auto nearConsumer=[](const View& frame){float f=0;std::memcpy(&f,frame.data()+0x4B4,4);return frame[0x57]&0x80?f:.6f;};
    auto wrong=before;wrong[0x57]=0;wrong[0x58]|=1;Put(wrong,0x4B4,0.0f);
    Check(nearConsumer(wrong)==.6f,"the neighbouring FAR override cannot activate the NEAR repair");
    Check(nearConsumer(lens)==0,"the verified native NEAR branch consumes the repair");
    Reader reader;const auto actorAddress=reinterpret_cast<uintptr_t>(reader.actor.data()),meshAddress=reinterpret_cast<uintptr_t>(reader.mesh.data());
    std::memcpy(reader.actor.data()+0x380,&meshAddress,8);std::memcpy(reader.mesh.data()+0xE8,&actorAddress,8);
    float nativeBounds[]{400,0,0,120,80,100,175};std::memcpy(reader.mesh.data()+0x148,nativeBounds,sizeof(nativeBounds));
    auto copied=dg::camera_focus::ReadBounds(reader,actorAddress);
    Check(copied.valid&&copied.center.x==400&&copied.extent.z==100,"retail mesh+148 bounds are copied only through reciprocal ownership");
    uintptr_t foreignOwner=1;std::memcpy(reader.mesh.data()+0xE8,&foreignOwner,8);
    Check(!dg::camera_focus::ReadBounds(reader,actorAddress).valid,"a foreign component cannot supply the focus band");
    std::memcpy(reader.mesh.data()+0xE8,&actorAddress,8);nativeBounds[6]=10;std::memcpy(reader.mesh.data()+0x148,nativeBounds,sizeof(nativeBounds));
    Check(!dg::camera_focus::ReadBounds(reader,actorAddress).valid,"inconsistent native bounds fail open");
    // A mounted camera protects the mount and its rider together.
    const auto combined=dg::camera_focus::Merge(body,{{470,0,130},{90,70,80},true});
    Check(combined.valid&&combined.center.x-combined.extent.x==280&&combined.center.x+combined.extent.x==560&&
        combined.center.z+combined.extent.z==210,"mount and rider bounds are united without persistent pointers");
    for(float weight:{0.0f,.5f,2.0f}){
        auto partial=before;Put(partial,0x38,weight);auto original=partial;
        Check(!dg::camera_focus::Compensate(partial.data(),{400,0,0},true)&&partial==original,
            "lens mode cannot raise partial weights or modify an invalid native frame");
    }
    p1=before;p2=before;dg::camera_focus::Compensate(p1.data(),{300,0,0},true);
    Check(p2==before,"P1 focus band cannot leak into P2 or native views");
    auto blur=before;blur[0x63]=2;Put(blur,0x4B4,6.0f);Put(blur,0x4B8,6.0f);auto authoredBlur=blur;
    Check(!dg::camera_focus::DisableBlur(blur.data(),true)&&blur==authoredBlur,"unverified blur layout is never patched");
    dg::camera_focus::blurLayoutReady=true;
    Check(!dg::camera_focus::DisableBlur(blur.data(),false)&&blur==authoredBlur,"default OFF preserves the validated output exactly");
    Check(!dg::camera_focus::DisableBlur(blur.data(),true,true)&&blur==authoredBlur,"cinematic policy always preserves the native effect");
    Check(dg::camera_focus::DisableBlur(blur.data(),true),"enabled suppression accepts a verified full-weight gameplay view");
    float near=1,far=1;std::memcpy(&near,blur.data()+0x4B4,4);std::memcpy(&far,blur.data()+0x4B8,4);
    Check(blur[0x63]==1&&(blur[0x57]&0x80)&&(blur[0x58]&3)==3&&near==0&&far==0,
        "native method, NEAR and FAR consumers select a zero Gaussian kernel");
    for(size_t i=0;i<blur.size();++i)
        if(i!=0x57&&i!=0x58&&i!=0x63&&(i<0x4B4||i>=0x4BC))
            Check(blur[i]==authoredBlur[i],"optional blur suppression changes no geometry, aperture, focus, quality or unrelated postprocess bytes");
    dg::camera_focus::Compensate(blur.data(),{400,0,0},true,body);
    std::memcpy(&far,blur.data()+0x4B8,4);
    Check(far==0&&blur[0x63]==1,"the validated focus path cannot reactivate the disabled distant layer");
    p1=authoredBlur;p2=authoredBlur;dg::camera_focus::DisableBlur(p1.data(),true);
    Check(p2==authoredBlur,"one player's suppression cannot modify the other view");
    for(float weight:{0.0f,.5f,2.0f}){
        auto partial=authoredBlur;Put(partial,0x38,weight);auto original=partial;
        Check(!dg::camera_focus::DisableBlur(partial.data(),true)&&partial==original,"blur option never raises a partial native weight");
    }
    auto unknownBlur=authoredBlur;unknownBlur[0x63]=9;auto unknownOriginal=unknownBlur;
    Check(!dg::camera_focus::DisableBlur(unknownBlur.data(),true)&&unknownBlur==unknownOriginal,"unknown method retains native blur");
    auto fresh=authoredBlur;dg::camera_focus::DisableBlur(fresh.data(),false);
    Check(fresh==authoredBlur,"OFF uses a fresh native frame, without any persistent restore write");
    using dg::camera_blur::Profile;
    const dg::camera_focus::Point target{400,0,0};
    auto custom=authoredBlur;auto original=custom;
    Check(!dg::camera_focus::ApplyBlur(custom.data(),{},false,&target,body)&&custom==original,
        "unselected customization keeps the validated per-frame output byte-identical");
    Check(!dg::camera_focus::ApplyBlur(custom.data(),{false,true,10,.6f},true,&target,body)&&custom==original,
        "cinematics cannot inherit either custom profile");
    Check(dg::camera_focus::ApplyBlur(custom.data(),{false,true,10,.6f},false,&target,body),"explicit background customization accepts the live target");
    std::memcpy(&region,custom.data()+0x4A0,4);std::memcpy(&far,custom.data()+0x4B8,4);
    Check(region==1000&&far==.6f&&Focus(custom)==400&&custom[0x63]==1,"10 metres and absolute .6 percent kernel reach the native Gaussian consumers");
    for(size_t i=0;i<custom.size();++i)
        if(i!=0x56&&i!=0x57&&i!=0x58&&i!=0x63&&(i<0x494||i>=0x498)&&(i<0x4A0||i>=0x4A4)&&(i<0x4B4||i>=0x4BC))
            Check(custom[i]==original[i],"custom blur preserves geometry, transition, aperture, weights and all unrelated postprocess fields");
    custom=authoredBlur;dg::camera_focus::ApplyBlur(custom.data(),{false,true,0,2},false,&target,body);
    std::memcpy(&region,custom.data()+0x4A0,4);
    Check(region==120,"zero clear distance still protects the rear mesh bound");
    custom=authoredBlur;Check(dg::camera_focus::ApplyBlur(custom.data(),{true,true,10,3},false),"disable takes precedence without requiring a target");
    std::memcpy(&far,custom.data()+0x4B8,4);Check(far==0,"disable wins over a stored nonzero amount");
    for(float meters:{1000.0f,1250.5f,dg::camera_blur::MaxDistanceMeters}){
        custom=authoredBlur;
        Check(dg::camera_focus::ApplyBlur(custom.data(),{false,true,meters,.6f},false,&target,body),"long clear distances reach the output without the old 200 metre gate");
        std::memcpy(&region,custom.data()+0x4A0,4);
        Check(region==meters*100.0f&&Focus(custom)==400,"metres convert to the native region without moving the character focus");
    }
    for(Profile bad: {Profile{false,true,-1,.6f},Profile{false,true,dg::camera_blur::MaxDistanceMeters+1,.6f},Profile{false,true,10,6},
        Profile{false,true,10,std::numeric_limits<float>::quiet_NaN()}}){
        custom=authoredBlur;Check(!dg::camera_focus::ApplyBlur(custom.data(),bad,false,&target,body)&&custom==authoredBlur,"invalid custom values fail open without partial writes");
    }
    custom=authoredBlur;Check(!dg::camera_focus::ApplyBlur(custom.data(),{false,true,10,.6f},false)&&custom==authoredBlur,"missing current pose cannot invent a focal plane");
    p1=authoredBlur;p2=authoredBlur;
    dg::camera_focus::ApplyBlur(p1.data(),{false,true,5,1},false,&target,body);
    dg::camera_focus::ApplyBlur(p2.data(),{false,true,20,2},false,&target,body);
    float regionP1=0,regionP2=0;std::memcpy(&regionP1,p1.data()+0x4A0,4);std::memcpy(&regionP2,p2.data()+0x4A0,4);
    Check(regionP1==500&&regionP2==2000,"native and TPS profiles can be applied to independent player outputs without shared render state");
}
}
int main(){try{Run();std::cout<<"TPS per-view focus and native postprocess preservation passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
