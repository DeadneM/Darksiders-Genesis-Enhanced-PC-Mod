#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <limits>

namespace {
using namespace dg::foreign_montage;
struct Fixture {
    unsigned char original[0x200]{},copy[0x200]{},war[0x180]{},idle[0x180]{},warSkeleton[0x180]{},strifeSkeleton[0x180]{};
    unsigned char classes[8][0x80]{},originalPackage[0x80]{},warPackage[0x80]{},transient[0x80]{},notify[0x100]{},notifyVtable[0x280]{};
    unsigned char tracks[24]{},segments[96]{},events[4*184]{},sections[88]{},sequenceEvents[8*184]{};
    unsigned char copyTracks[24]{},copySegments[96]{},copyEvents[64*184]{},copySections[88]{};
    unsigned char blade[0x100]{},bladeCopy[0x100]{},nova[0x100]{},sound[0x100]{},other[0x100]{},assets[4][0x80]{};
    unsigned char beamSequences[3][0x180]{},beamDestroy[0x100]{},beamInterrupt[0x100]{},beamStateClass[0x80]{};
    unsigned char gesture[0x240]{},gesturePackage[0x80]{},gestureEvents[552]{},gestureNotifies[3][0x100]{},footstepClass[0x80]{},gestureTrack[4]{},items[8*24]{};
 unsigned char interruptClass[0x80]{},interrupts[2][0x80]{};
    unsigned char blendProperty[0x80]{},blendStruct[0x80]{},blendTime[0x80]{},structClass[0x80]{},floatClass[0x80]{};
} f;
unsigned char entries[64][256]{};uintptr_t names[64]{},chunks[1]{reinterpret_cast<uintptr_t>(names)},objectChunks[1]{};
bool gameThread=true;int mode=0;unsigned constructors=0,skeletonCalls=0,reserves=0;
Source source{};
template<class T>void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T>T At(const void* p,size_t offset){T value{};std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* object,unsigned index,const char* text){Put(object,0x18,int(index));Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),244,text);}
void* PackageNative(){return f.transient;}
void SkeletonNative(void* asset,void* skeleton){++skeletonCalls;Put(asset,0x38,reinterpret_cast<uintptr_t>(skeleton));}
void ReserveNative(void* array,int count){
    ++reserves;Check(array==f.copy+0x80&&count>=3&&count<=64,"native reserve only receives private event header and bounded merged count");
    if(mode==10)return;
    Put(array,0,Array{reinterpret_cast<uintptr_t>(mode==11?f.events:f.copyEvents),At<Array>(array,0).count,count});
}
void* ConstructNative(void* cls,void* outer,Name name,uint32_t flags,uint32_t internal,void* templ,bool transients,void* graph,bool assume){
    ++constructors;
    if(templ==f.blade){
        Check(cls==f.classes[6]&&outer==f.copy&&!name.index&&!name.number&&flags==0x40u&&!internal&&!transients&&!graph&&!assume,
              "root notify clone uses its exact native class and private montage outer");
        if(mode==12)return nullptr;if(mode==13)return f.blade;
        std::memcpy(f.bladeCopy,f.blade,sizeof(f.blade));Put(f.bladeCopy,8,flags);Put(f.bladeCopy,0x20,reinterpret_cast<uintptr_t>(outer));
        if(mode==14)Put(f.bladeCopy,0x20,uintptr_t(0));if(mode==15)Put(f.bladeCopy,8,uint32_t(0x50));
        return f.bladeCopy;
    }
    Check(cls==f.classes[0]&&outer==f.transient&&name.index==0&&name.number==0&&flags==0x40u&&internal==0&&
          templ==f.original&&!transients&&!graph&&!assume,"all nine native constructor arguments preserve exact ABI and template ownership");
    if(mode==1)return nullptr;if(mode==2)return f.original;
    std::memcpy(f.copy,f.original,sizeof(f.original));Put(f.copy,8,flags);Put(f.copy,0x20,reinterpret_cast<uintptr_t>(outer));
    std::memcpy(f.copyTracks,f.tracks,sizeof(f.tracks));std::memcpy(f.copySegments,f.segments,sizeof(f.segments));
    std::memcpy(f.copyEvents,f.events,sizeof(f.events));std::memcpy(f.copySections,f.sections,sizeof(f.sections));
    const auto sourceEvents=At<Array>(f.original,0x80);
    Put(f.copy,0x80,Array{reinterpret_cast<uintptr_t>(mode==3?f.events:f.copyEvents),sourceEvents.count,sourceEvents.capacity});
    Put(f.copy,0x148,Array{reinterpret_cast<uintptr_t>(mode==4?f.sections:f.copySections),1,1});
    Put(f.copy,0x158,Array{reinterpret_cast<uintptr_t>(mode==5?f.tracks:f.copyTracks),1,1});
    const auto segments=At<Array>(f.tracks,8);
    Put(f.copyTracks,8,Array{reinterpret_cast<uintptr_t>(mode==6?f.segments:f.copySegments),segments.count,segments.capacity});
    if(mode==7)Put(f.copy,8,uint32_t(0x50));
    if(mode==8)Put(f.copy,0x20,uintptr_t(0));
    if(mode==9)Put(f.copy,8,uint32_t(0));
    return f.copy;
}
void Fresh(){
    f={};entries[0][12]=0;gameThread=true;mode=0;constructors=skeletonCalls=reserves=0;g_reserveSegments=nullptr;g_ready=true;g_gestures[0]=g_gestures[1]={};
    g_construct=ConstructNative;g_package=PackageNative;g_skeleton=SkeletonNative;g_reserve=ReserveNative;
    const char* labels[]={"AnimMontage","Class","AnimSequence","Skeleton","Package","AnimNotify_CallAbilityFunction","AnimNotify_PlayParticleEffect","AnimNotify_PlaySound"};
    const unsigned ids[]={1,2,3,4,5,6,17,18};for(unsigned i=0;i<8;++i)Label(f.classes[i],ids[i],labels[i]);
    Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
    Label(f.blendProperty,49,"BlendOut");Label(f.blendStruct,50,"AlphaBlend");Label(f.blendTime,51,"BlendTime");Label(f.structClass,52,"StructProperty");Label(f.floatClass,53,"FloatProperty");
    Put(f.classes[0],0x48,reinterpret_cast<uintptr_t>(f.blendProperty));Put(f.blendProperty,0x10,reinterpret_cast<uintptr_t>(f.structClass));Put(f.blendProperty,0x34,48);Put(f.blendProperty,0x44,0xE0);Put(f.blendProperty,0x70,reinterpret_cast<uintptr_t>(f.blendStruct));
    Put(f.blendStruct,0x48,reinterpret_cast<uintptr_t>(f.blendTime));Put(f.blendTime,0x10,reinterpret_cast<uintptr_t>(f.floatClass));Put(f.blendTime,0x34,4);Put(f.blendTime,0x44,8);
    Put(f.original,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Label(f.original,7,"War_Wrath_BladeGeyser_Montage");
    Label(f.originalPackage,8,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Montage");
    Put(f.original,0x20,reinterpret_cast<uintptr_t>(f.originalPackage));Put(f.original,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));
    Put(f.original,0x90,2.3666665554f);Put(f.original,0x94,1.0f);
    Put(f.war,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Label(f.war,9,"War_Wrath_BladeGeyser");
    Label(f.warPackage,10,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser");
    Put(f.war,0x20,reinterpret_cast<uintptr_t>(f.warPackage));Put(f.war,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));
    Put(f.war,0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents),3,3});
    Label(f.warSkeleton,11,"War_Mesh_Skeleton");Put(f.warSkeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
    Label(f.strifeSkeleton,12,"Strife_Mesh_Skeleton");Put(f.strifeSkeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
    Label(f.idle,13,"Strife_Idle");Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));
    Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.strifeSkeleton));Put(f.idle,0x90,2.6666667461f);Put(f.idle,0x94,1.0f);
    Label(f.transient,14,"/Engine/Transient");Put(f.transient,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));
    strcpy_s(reinterpret_cast<char*>(entries[15]+12),244,"TPS");source={reinterpret_cast<uintptr_t>(f.idle),reinterpret_cast<uintptr_t>(f.strifeSkeleton),15,0,19,0};
    strcpy_s(reinterpret_cast<char*>(entries[19]+12),244,"Bone_Strife_Root");strcpy_s(reinterpret_cast<char*>(entries[20]+12),244,"Bone_War_Root");
    Put(f.tracks,0,Name{15,0});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),1,1});
    Segment segment{};segment.sequence=reinterpret_cast<uintptr_t>(f.war);segment.last=2.3666665554f;segment.rate=1;segment.loops=1;segment.valid=1;Put(f.segments,0,segment);
    Put(f.original,0x80,Array{reinterpret_cast<uintptr_t>(f.events),2,2});
    Put(f.original,0x148,Array{reinterpret_cast<uintptr_t>(f.sections),1,1});Put(f.original,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});
    Put(f.notify,0,reinterpret_cast<uintptr_t>(f.notifyVtable));Put(f.notify,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));
    Put(f.notify,0x20,reinterpret_cast<uintptr_t>(f.war));strcpy_s(reinterpret_cast<char*>(entries[16]+12),244,"SpawnBlast");
    Put(f.notify,0x38,Name{16,0});Put(f.notifyVtable,0x240,g_base+0x5A7C60);
    Put(f.sequenceEvents,0x48,reinterpret_cast<uintptr_t>(f.notify));Put(f.sequenceEvents,0x24,.4880667329f);
    Put(f.sequenceEvents,0x28,reinterpret_cast<uintptr_t>(f.war));
    Label(f.assets[0],21,"VFX_war_wrath_bladeGeyser_blade");Label(f.assets[1],22,"VFX_war_wrath_bladeGeyser_nova");
    Label(f.assets[2],23,"char_war_wrath_bladegeyser");Label(f.assets[3],24,"P_War_Swipe_03");
    for(auto* particle:{f.blade,f.nova,f.other})Put(particle,0x10,reinterpret_cast<uintptr_t>(f.classes[6]));
    Put(f.sound,0x10,reinterpret_cast<uintptr_t>(f.classes[7]));
    Put(f.blade,0x20,reinterpret_cast<uintptr_t>(f.war));Put(f.blade,0x38,reinterpret_cast<uintptr_t>(f.assets[0]));Put(f.blade,0x84,Name{20,0});
    Put(f.blade,0x40,37.0f);Put(f.blade,0x70,.25f);
    Put(f.nova,0x20,reinterpret_cast<uintptr_t>(f.original));Put(f.nova,0x38,reinterpret_cast<uintptr_t>(f.assets[1]));
    Put(f.sound,0x20,reinterpret_cast<uintptr_t>(f.original));Put(f.sound,0x38,reinterpret_cast<uintptr_t>(f.assets[2]));
    Put(f.other,0x20,reinterpret_cast<uintptr_t>(f.war));Put(f.other,0x38,reinterpret_cast<uintptr_t>(f.assets[3]));
    Put(f.sequenceEvents,184+0x48,reinterpret_cast<uintptr_t>(f.blade));Put(f.sequenceEvents,184+0x24,.4880667329f);
    Put(f.sequenceEvents,368+0x48,reinterpret_cast<uintptr_t>(f.other));Put(f.sequenceEvents,368+0x24,.8f);
    Put(f.events,0x48,reinterpret_cast<uintptr_t>(f.sound));Put(f.events,184+0x48,reinterpret_cast<uintptr_t>(f.nova));Put(f.events,184+0x24,.459302f);
}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Fresh();const auto original=std::make_unique<Fixture>(f);
    Check(CanBuild(source)&&Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"known template creates private compatible native montage");
    Check(constructors==2&&skeletonCalls==1&&reserves==1,"one montage, one root-only notify clone and native private reserve");
    Check(!std::memcmp(f.original,original->original,sizeof(f.original))&&!std::memcmp(f.war,original->war,sizeof(f.war))&&
          !std::memcmp(f.sequenceEvents,original->sequenceEvents,sizeof(f.sequenceEvents))&&!std::memcmp(f.events,original->events,sizeof(f.events))&&
          !std::memcmp(f.tracks,original->tracks,sizeof(f.tracks))&&!std::memcmp(f.segments,original->segments,sizeof(f.segments))&&
          !std::memcmp(f.sections,original->sections,sizeof(f.sections))&&!std::memcmp(f.blade,original->blade,sizeof(f.blade)),"all source assets, notifies and arrays remain byte-identical");
    Check(At<int>(f.copy,0x88)==4&&At<uintptr_t>(f.copy,0x38)==source.skeleton&&At<float>(f.copy,0x90)==2.3666665554f,"merged native presentation and single callback retain original duration");
    const auto segment=At<Segment>(f.copySegments,0);
    Check(segment.sequence==source.sequence&&segment.last==2.6666667461f&&std::abs(segment.last/segment.rate-2.3666665554f)<.00001f,"compatible idle segment retains source timeline");
    Check(At<uintptr_t>(f.copyEvents,2*184+0x48)==reinterpret_cast<uintptr_t>(f.notify)&&At<float>(f.copyEvents,2*184+0x24)==.4880667329f&&
          At<uintptr_t>(f.copyEvents,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<uintptr_t>(f.copyEvents,0x28)==source.sequence&&
          At<uintptr_t>(f.copyEvents,0x68)==reinterpret_cast<uintptr_t>(f.copy)&&At<float>(f.copySections,0x24)==0,
          "native SpawnBlast callback and queued time retained with only private link targets replaced");
    Check(At<uintptr_t>(f.copyEvents,0x48)==reinterpret_cast<uintptr_t>(f.sound)&&At<float>(f.copyEvents,0x24)==0&&
          At<uintptr_t>(f.copyEvents,184+0x48)==reinterpret_cast<uintptr_t>(f.nova)&&At<float>(f.copyEvents,184+0x24)==.459302f&&
          At<uintptr_t>(f.copyEvents,3*184+0x48)==reinterpret_cast<uintptr_t>(f.bladeCopy),"native sound/nova/blade timing survives in stable order; War sword FX omitted");
    Check(At<Name>(f.bladeCopy,0x84).index==19&&At<float>(f.bladeCopy,0x40)==37&&At<float>(f.bladeCopy,0x70)==.25f&&
          At<Name>(f.blade,0x84).index==20,"private root notify preserves native offsets/cached rotation and binds own Strife root");
    for(int failure=1;failure<=9;++failure){
        Fresh();mode=failure;const auto before=std::make_unique<Fixture>(f);
        Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!skeletonCalls,"allocation, alias, CDO and outer failures reject before first mutation");
        Check(!std::memcmp(f.original,before->original,sizeof(f.original))&&!std::memcmp(f.events,before->events,sizeof(f.events))&&
              !std::memcmp(f.tracks,before->tracks,sizeof(f.tracks))&&!std::memcmp(f.segments,before->segments,sizeof(f.segments))&&
              !std::memcmp(f.sections,before->sections,sizeof(f.sections)),"failed clone never writes source buffers");
    }
    for(unsigned failure=0;failure<13;++failure){
        Fresh();
        if(failure==0)gameThread=false;
        if(failure==1)g_ready=false;
        if(failure==2)Label(f.originalPackage,8,"/Game/Other");
        if(failure==3)Put(f.sequenceEvents,0x91,static_cast<unsigned char>(1));
        if(failure==4)Put(f.sequenceEvents,0x18,static_cast<unsigned char>(1));
        if(failure==5)Put(f.sequenceEvents,0x58,1.0f);
        if(failure==6)Put(f.original,0x168,Array{reinterpret_cast<uintptr_t>(f.events),1,1});
        if(failure==7)Put(f.original,0x128,Array{reinterpret_cast<uintptr_t>(f.events),1,1});
        if(failure==8)Put(f.idle,0x80,Array{reinterpret_cast<uintptr_t>(f.events),1,1});
        if(failure==9)Put(f.notifyVtable,0x240,g_base+0x5A7C61);
        if(failure==10)Put(f.original,0x90,2.0f);
        if(failure==11)Put(f.segments,0x18,2);
        if(failure==12)Put(f.notify,0x20,source.sequence);
        Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!constructors&&!skeletonCalls,"unproved source/notify/thread contracts stop before allocation");
    }
    for(int failure=10;failure<=15;++failure){
        Fresh();mode=failure;const auto before=std::make_unique<Fixture>(f);
        Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!skeletonCalls,"reserve and private root clone failure never reach playback-ready skeleton");
        Check(!std::memcmp(f.events,before->events,sizeof(f.events))&&!std::memcmp(f.blade,before->blade,sizeof(f.blade)),"failed presentation adaptation preserves original events and root notify");
    }
    for(unsigned failure=0;failure<9;++failure){
        Fresh();
        if(failure==0)source.rootIndex=0;
        if(failure==1)Put(f.events,0x48,uintptr_t(0));
        if(failure==2)Put(f.nova,0x20,uintptr_t(0));
        if(failure==3)Put(f.events,184+0x24,std::numeric_limits<float>::quiet_NaN());
        if(failure==4)Put(f.events,184+0x58,1.0f);
        if(failure==5)Put(f.events,184+0x50,reinterpret_cast<uintptr_t>(f.other));
        if(failure==6)Put(f.events,184+0x91,static_cast<unsigned char>(1));
        if(failure==7)g_reserve=nullptr;
        if(failure==8)Put(f.events,184+0x48,reinterpret_cast<uintptr_t>(f.sound));
        Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!constructors&&!skeletonCalls,"missing sound/root, duplicate sound or invalid timing/state contracts reject before native construction");
    }
    Fresh();Put(f.blade,0x84,Name{15,0});
    Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!skeletonCalls,"unknown non-root sockets never fall back to foreign War attachments");
    Fresh();Put(f.blade,0x84,Name{});
    Check(Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy&&constructors==1,
          "socket-free effects retain native notify without an unnecessary clone");
    Fresh();Presentation full{};full.count=int(full.events.size());
    Check(!Collect(reinterpret_cast<uintptr_t>(f.war),Array{reinterpret_cast<uintptr_t>(f.sequenceEvents+184),1,1},full),"presentation bound rejects overflow rather than dropping an event or writing past storage");
    Fresh();Put(f.war,0x80,Array{reinterpret_cast<uintptr_t>(f.events),2,2});
    std::memcpy(f.events,f.sequenceEvents,184);std::memcpy(f.events+184,f.sequenceEvents,184);
    Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!constructors,"duplicate native blast callback is rejected");
    for(const char* level:{"Level1","Level2","Level3"}){
        Fresh();char label[100]{},package[200]{};sprintf_s(label,"War_Wrath_BladeGeyser_%s_Montage",level);
        sprintf_s(package,"/Game/Characters/Heroes/Hero_War/Animation/%s",label);Label(f.original,7,label);Label(f.originalPackage,8,package);
        Check(Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"each known upgrade montage uses the same guarded native contract");
    }
}
void Batch(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    const char* labels[]={"Strife_SpawnClone_Back_Montage","Strife_SpawnClone_F_Montage"};
    for(unsigned i=0;i<2;++i){
        Fresh();char package[200]{};sprintf_s(package,"/Game/Characters/Heroes/%s/%s/%s",i<3?"Hero_Strife":"Hero_War",i<3?"Animations":"Animation",labels[i]);
        Label(f.original,7,labels[i]);Label(f.originalPackage,8,package);
        Put(f.war,0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents),1,1});
        const float span=At<float>(f.original,0x90);Put(f.original,0x90,span*2);
        auto segment=At<Segment>(f.segments,0);segment.start=span;Put(f.segments,32,segment);Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),2,2});
        Put(f.original,0x188,2);Put(f.original,0x198,2);Put(f.sequenceEvents,0x91,static_cast<unsigned char>(1));
        const auto before=std::make_unique<Fixture>(f);
        Check(Compatible(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"both Clone montages support multiple source segments");
        Check(At<int>(f.copy,0x88)==4&&At<int>(f.copy,0x188)==0&&At<int>(f.copy,0x198)==0,"private queued notifications replace private branch caches");
        Check(At<uintptr_t>(f.copyEvents,2*184+0x48)==reinterpret_cast<uintptr_t>(f.notify)&&At<uintptr_t>(f.copyEvents,3*184+0x48)==reinterpret_cast<uintptr_t>(f.notify)&&
              std::abs(At<float>(f.copyEvents,3*184+0x24)-(.4880667329f+span))<.00001f,"native callbacks keep exact mapped segment times without duplication");
        Check(At<Segment>(f.copySegments,32).sequence==source.sequence&&At<Segment>(f.copySegments,32).start==span,"compatible pose preserves later segment start");
        Check(!std::memcmp(f.original,before->original,sizeof(f.original))&&!std::memcmp(f.sequenceEvents,before->sequenceEvents,sizeof(f.sequenceEvents))&&
              !std::memcmp(f.segments,before->segments,sizeof(f.segments)),"generic transfer leaves original montage/sequence/events untouched");
        for(int bad:{2,3,4,5,6,7,8,9}){mode=bad;Check(!Compatible(reinterpret_cast<uintptr_t>(f.original),source),"generic clone rejects shared arrays, CDO and wrong outer");}
    }
    Check(dg::native_objects::NameValueEquals(g_base,19,0,"Bone_Strife_Root")&&!dg::native_objects::NameValueEquals(g_base,19,0,"Bone_Strife")&&
          !dg::native_objects::NameValueEquals(g_base,19,1,"Bone_Strife_Root")&&!dg::native_objects::NameValueEquals(g_base,-1,0,"X"),"dynamic names require exact full value, valid index and no instance number");
    Put(entries[39],8,uint32_t(1));const wchar_t wide[]=L"WideName";std::memcpy(entries[39]+12,wide,sizeof(wide));
    Check(dg::native_objects::NameValueEquals(g_base,39,0,"WideName")&&!dg::native_objects::NameValueEquals(g_base,39,0,"Wide"),"bounded dynamic name lookup supports wide names");
}
void GestureFixture(bool war=false,bool reset=true){
    if(reset)Fresh();
    auto* skeleton=war?f.warSkeleton:f.strifeSkeleton;
    Label(skeleton,war?11:12,war?"War_Mesh_Skeleton":"Strife_Skeleton");
    Label(f.gesture,40,war?"War_Channel_Start":"Strife_Aether_Channel_Start");
    Label(f.gesturePackage,41,war?"/Game/Characters/Heroes/Hero_War/Animation/War_Channel_Start":"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Aether_Channel_Start");
    Label(f.footstepClass,42,"AnimNotify_PlayFootstep");
    Put(f.gesture,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Put(f.gesture,0x20,reinterpret_cast<uintptr_t>(f.gesturePackage));Put(f.gesture,0x38,reinterpret_cast<uintptr_t>(skeleton));
    Put(f.gesture,0x90,war?.5666666627f:1.f);Put(f.gesture,0x94,1.f);Put<unsigned char>(f.gesture,0x1A1,war?1:0);
    Put(f.gesture,0x80,Array{reinterpret_cast<uintptr_t>(f.gestureEvents),3,3});Put(f.gesture,0xB0,Array{reinterpret_cast<uintptr_t>(f.gestureTrack),1,1});
    const float times[2][3]={{0,0,.4308005273f},{0,.1327852011f,.3913668990f}};
    for(int i=0;i<3;++i){auto* e=f.gestureEvents+i*184;auto* n=f.gestureNotifies[i];Put(e,0x48,reinterpret_cast<uintptr_t>(n));Put(e,0x24,times[war?1:0][i]);
        Put(n,0x10,reinterpret_cast<uintptr_t>(war&&i>0?f.footstepClass:f.classes[7]));Put(n,0x20,reinterpret_cast<uintptr_t>(f.gesture));}
    Put(f.gesture,0xC,1);Put(f.items,24,reinterpret_cast<uintptr_t>(f.gesture));Put(f.items,24+16,77);
    objectChunks[0]=reinterpret_cast<uintptr_t>(f.items);Put(reinterpret_cast<void*>(g_base),0x3959680,reinterpret_cast<uintptr_t>(objectChunks));Put(reinterpret_cast<void*>(g_base),0x3959694,8);
}
void Gestures(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    for(bool war:{false,true}){
        GestureFixture(war);const auto before=std::make_unique<Fixture>(f);ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));float duration=5;
        const auto skeleton=reinterpret_cast<uintptr_t>(war?f.warSkeleton:f.strifeSkeleton);
        Check(CastingSequence(source.sequence,skeleton,duration)==reinterpret_cast<uintptr_t>(f.gesture)&&duration==(war?.5666666627f:1.f),"exact receiver gesture selected with its duration");
        Check(!constructors&&!skeletonCalls&&!std::memcmp(f.gesture,before->gesture,sizeof(f.gesture))&&!std::memcmp(f.gestureEvents,before->gestureEvents,sizeof(f.gestureEvents)),"gesture discovery and selection leave assets and notifies unchanged");
        duration=5;Check(CastingSequence(source.sequence,reinterpret_cast<uintptr_t>(f.other),duration)==source.sequence&&duration==5,"foreign skeleton keeps validated idle fallback");
        Put(f.items,24+16,78);duration=5;Check(CastingSequence(source.sequence,skeleton,duration)==source.sequence&&duration==5,"expired gesture cannot be reused by address");
    }
    for(int bad=0;bad<10;++bad){
        GestureFixture();
        if(bad==0)Label(f.gesturePackage,41,"/Unrelated/Strife_Aether_Channel_Start");
        if(bad==1)Put(f.gesture,0x90,std::numeric_limits<float>::quiet_NaN());
        if(bad==2)Put<unsigned char>(f.gesture,0x1A1,1);
        if(bad==3)Put(f.gestureEvents,0x50,reinterpret_cast<uintptr_t>(f.other));
        if(bad==4)Put(f.gestureEvents,0x58,.1f);
        if(bad==5)Put(f.gestureNotifies[0],0x10,reinterpret_cast<uintptr_t>(f.classes[5]));
        if(bad==6)Put(f.gestureNotifies[0],0x10,reinterpret_cast<uintptr_t>(f.classes[6]));
        if(bad==7)Put(f.gestureEvents,2*184+0x24,.8f);
        if(bad==8)Put(f.gestureTrack,0,1);
        if(bad==9)gameThread=false;
        ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));float duration=5;
        Check(CastingSequence(source.sequence,source.skeleton,duration)==source.sequence&&duration==5&&!constructors,"wrong package, root, time, state, callback or effect cannot enter gesture playback");
    }
    GestureFixture();ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));
    Check(Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"gesture integrates with validated Geyser builder");
    const auto segment=At<Segment>(f.copySegments,0);
    Check(segment.sequence==reinterpret_cast<uintptr_t>(f.gesture)&&segment.last==1&&std::fabs(segment.rate-1/2.3666665554f)<.00001f,"gesture fits existing cast span instead of changing callback timing");
    Check(At<uintptr_t>(f.copyEvents,2*184+0x48)==reinterpret_cast<uintptr_t>(f.notify)&&At<float>(f.copyEvents,2*184+0x24)==.4880667329f&&At<int>(f.copy,0x88)==4,"one original blast callback and complete presentation remain on their native times");
    GestureFixture(true);ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));
    Label(f.idle,13,"War_Idle");Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));source.skeleton=reinterpret_cast<uintptr_t>(f.warSkeleton);source.rootIndex=20;
    Label(f.original,7,"Strife_SpawnClone_F_Montage");Label(f.originalPackage,8,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_SpawnClone_F_Montage");
    Check(Compatible(reinterpret_cast<uintptr_t>(f.original),source)==f.copy&&At<Segment>(f.copySegments,0).sequence==reinterpret_cast<uintptr_t>(f.gesture),"Clone builder uses War gesture on own skeleton");
    Check(At<int>(f.copy,0x88)==5&&std::fabs(At<float>(f.copy,0x90)-.5666666627f)<.00001f&&
          std::fabs(At<float>(f.copyEvents,2*184+0x24)-.4880667329f*.5666666627f/2.3666665554f)<.00001f&&
          std::fabs(At<Segment>(f.copySegments,0).rate-1.f)<.00001f,"Clone keeps its complete event plan while the owned gesture and callbacks use the shorter authored duration");
}
void BeamFixture(){
    Fresh();
    Label(f.idle,13,"War_Idle");Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));source.skeleton=reinterpret_cast<uintptr_t>(f.warSkeleton);source.rootIndex=20;
    Label(f.original,7,"Strife_Aim_WorldEnder_Montage");Label(f.originalPackage,8,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Aim_WorldEnder_Montage");
    Put(f.original,0x38,reinterpret_cast<uintptr_t>(f.strifeSkeleton));Put(f.original,0x90,4.8999996185f);
    const float spans[]={.6666666865f,.9666666389f,1.3333330154f};float start=0;
    for(int i=0;i<3;++i){auto* seq=f.beamSequences[i];Put(seq,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));
        Put(seq,0x38,reinterpret_cast<uintptr_t>(f.strifeSkeleton));Put(seq,0x90,spans[i]);Put(seq,0x94,1.f);
        Segment segment{};segment.sequence=reinterpret_cast<uintptr_t>(seq);segment.start=start;segment.last=spans[i];segment.rate=1;segment.loops=i==1?3:1;segment.valid=1;
        Put(f.segments,size_t(i)*32,segment);start+=spans[i]*float(segment.loops);
    }
    Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),3,3});std::memset(f.events,0,sizeof(f.events));
    strcpy_s(reinterpret_cast<char*>(entries[16]+12),244,"StartBeam");Put(f.notify,0x20,reinterpret_cast<uintptr_t>(f.original));
    Put(f.beamDestroy,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));Put(f.beamDestroy,0x20,reinterpret_cast<uintptr_t>(f.original));Put(f.beamDestroy,0x38,Name{43,0});
    strcpy_s(reinterpret_cast<char*>(entries[43]+12),244,"DestroyBeam");Label(f.beamStateClass,44,"AnimNotify_EnableAbilityInterrupt");
    Put(f.beamInterrupt,0x10,reinterpret_cast<uintptr_t>(f.beamStateClass));Put(f.beamInterrupt,0x20,reinterpret_cast<uintptr_t>(f.original));
    Put(f.events,0x48,reinterpret_cast<uintptr_t>(f.notify));Put(f.events,0x24,.6564341187f);
    Put(f.events,184+0x48,reinterpret_cast<uintptr_t>(f.beamDestroy));Put(f.events,184+0x24,3.5871195793f);
    Put(f.events,368+0x50,reinterpret_cast<uintptr_t>(f.beamInterrupt));Put(f.events,368+0x24,1.014648795f);Put(f.events,368+0x58,3.885350704f);
    Put(f.original,0x80,Array{reinterpret_cast<uintptr_t>(f.events),3,3});
}
void Fades(){
    Fresh();Check(!SmoothExit(f.original),"original montage is never edited");
    Check(ConstructNative(f.classes[0],f.transient,{},0x40,0,f.original,false,nullptr,false)==f.copy,"private copy");
    Check(SmoothExit(f.copy)&&At<float>(f.copy,0xE8)==.18f&&At<float>(f.original,0xE8)==0,"only private fade extended");
    Put(f.copy,0xE8,.4f);Check(SmoothExit(f.copy)&&At<float>(f.copy,0xE8)==.4f,"longer authored fade retained");
    Put(f.blendTime,0x44,48);Check(!SmoothExit(f.copy),"invalid reflected blend member fails closed");
}
void GeyserPacing(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));for(unsigned i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Fades();GestureFixture(false);ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));
    g_reserveSegments=[](void* header,int count){Check(header==f.copyTracks+8&&count==2,"32-byte native reserve restricted to own cloned animation track");Put(header,0,Array{reinterpret_cast<uintptr_t>(f.copySegments),1,3});};
    Put(f.war,0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents),5,8});Label(f.interruptClass,48,"AnimNotify_EnableAbilityInterrupt");
    for(int i=0;i<2;++i){Put(f.interrupts[i],0x10,reinterpret_cast<uintptr_t>(f.interruptClass));Put(f.interrupts[i],0x20,reinterpret_cast<uintptr_t>(f.war));auto* e=f.sequenceEvents+(3+i)*184;const float time=i?1.0523383617f:.7892537713f;Put(e,0x50,reinterpret_cast<uintptr_t>(f.interrupts[i]));Put(e,0x24,time);Put(e,0x58,2.3666665554f-time);}
    const auto before=std::make_unique<Fixture>(f);Check(Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"Geyser gesture and native cancel windows build privately");
    Check(At<Array>(f.copyTracks,8).count==2&&At<int>(f.copy,0x88)==6&&At<float>(f.copy,0x90)==2.3666665554f,"native montage duration and all gameplay cues retained with two own-pose segments");
    auto cast=At<Segment>(f.copySegments,0),idle=At<Segment>(f.copySegments,32);
    Check(cast.sequence==reinterpret_cast<uintptr_t>(f.gesture)&&cast.rate==1&&cast.last==1&&idle.sequence==cast.sequence&&idle.start==1&&idle.first>idle.last-.034f&&std::fabs((idle.last-idle.first)/idle.rate-(2.3666665554f-1))<.00001f,"one-second gesture keeps native speed; own idle covers remaining native cast span");
    Check(At<float>(f.copyEvents,2*184+0x24)==.4880667329f&&At<uintptr_t>(f.copyEvents,2*184+0x48)==reinterpret_cast<uintptr_t>(f.notify),"actual blast callback not sped up or delayed to match a visual workaround");
    Check(At<uintptr_t>(f.copyEvents,4*184+0x50)==reinterpret_cast<uintptr_t>(f.interrupts[0])&&At<int>(f.copyEvents,4*184+0x74)==1&&std::fabs(At<float>(f.copyEvents,4*184+0x84)-2.3666665554f)<.00001f,"native MOVE state can begin in gesture and end in idle segment");
    Check(!std::memcmp(f.war,before->war,sizeof(f.war))&&!std::memcmp(f.sequenceEvents,before->sequenceEvents,sizeof(f.sequenceEvents))&&!std::memcmp(f.interrupts,before->interrupts,sizeof(f.interrupts)),"source animation, callback times and control windows never edited");
}
void Beams(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<64;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    BeamFixture();const auto before=std::make_unique<Fixture>(f);
    Check(WorldEnder(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"finite three-segment beam montage builds on own skeleton");
    Check(At<uintptr_t>(f.copy,0x38)==source.skeleton&&At<int>(f.copy,0x88)==3,"private skeleton and complete native callback/cancellation event plan");
    Check(At<Segment>(f.copySegments,32).loops==1&&At<Segment>(f.copySegments,64).loops==1&&At<float>(f.copy,0x90)==4.8999996185f,"native complete duration retained without repeating the casting gesture");
    const auto hold=At<Segment>(f.copySegments,32),end=At<Segment>(f.copySegments,64);
    Check(hold.first>hold.last-.034f&&std::fabs((hold.last-hold.first)/hold.rate-2.9f)<.0001f&&end.sequence==hold.sequence&&end.first==hold.first,
          "beam holds the final source frame for the full fire phase, then returns to own idle");
    Check(At<float>(f.copyEvents,0x24)==.6564341187f&&At<float>(f.copyEvents,2*184+0x24)==3.5871195793f,"StartBeam and DestroyBeam keep exact original times");
    Check(At<uintptr_t>(f.copyEvents,184+0x50)==reinterpret_cast<uintptr_t>(f.beamInterrupt)&&At<float>(f.copyEvents,184+0x58)==3.885350704f&&
          std::fabs(At<float>(f.copyEvents,184+0x60+0x24)-4.8999996185f)<.00001f,"native cancellation state keeps duration and linked end time, not start time");
    Check(!std::memcmp(f.original,before->original,sizeof(f.original))&&!std::memcmp(f.events,before->events,sizeof(f.events))&&!std::memcmp(f.segments,before->segments,sizeof(f.segments)),"original beam template and animation assets untouched");
    for(int bad=0;bad<7;++bad){BeamFixture();
        if(bad==0)Put(f.events,0x48,uintptr_t(0));
        if(bad==1)Put(f.events,184+0x24,3.f);
        if(bad==2)Put(f.events,368+0x58,20.f);
        if(bad==3)source.rootIndex=19;
        if(bad==4)Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),2,3});
        if(bad==5)Label(f.originalPackage,8,"/Wrong/Strife_Aim_WorldEnder_Montage");
        if(bad==6)Put(f.beamInterrupt,0x20,reinterpret_cast<uintptr_t>(f.other));
        Check(!WorldEnder(reinterpret_cast<uintptr_t>(f.original),source)&&!constructors,"missing/shifted callback, invalid state/recipient/package/shape rejected before construction");
    }
    for(int bad:{2,3,4,5,6,7,8,9}){BeamFixture();mode=bad;Check(!WorldEnder(reinterpret_cast<uintptr_t>(f.original),source),"beam clone rejects aliased arrays, CDO and bad owner");}
    BeamFixture();Check(!Compatible(reinterpret_cast<uintptr_t>(f.original),source),"validated Clone API does not silently accept the new beam montage");
    BeamFixture();Label(f.beamStateClass,44,"AnimNotify_SoundLoopRange");Put(f.events,368+0x24,0.f);Put(f.events,368+0x58,4.8999996185f);
    Check(WorldEnder(reinterpret_cast<uintptr_t>(f.original),source)==f.copy&&At<uintptr_t>(f.copyEvents,0x50)==reinterpret_cast<uintptr_t>(f.beamInterrupt),"native sound loop state retains engine-owned begin/end lifecycle on receiver");
    BeamFixture();GestureFixture(true,false);ObserveSequence(reinterpret_cast<uintptr_t>(f.gesture));
    Check(WorldEnder(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"quick beam uses the compatible owned gesture");
    const float start=At<float>(f.copyEvents,0x24),stop=At<float>(f.copyEvents,368+0x24);
    Check(std::fabs(start-.5666666627f)<.00001f&&std::fabs((stop-start)-(3.5871195793f-.6564341187f))<.00001f&&
          std::fabs(At<float>(f.copy,0x90)-stop-.2f)<.00001f,"wind-up/recovery shorten while the exact native damage interval is preserved");
}
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
int main(){try{Run();Batch();Gestures();GeyserPacing();Beams();std::cout<<"Private native montage allocation, callback mapping, source preservation and alias rejection passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
