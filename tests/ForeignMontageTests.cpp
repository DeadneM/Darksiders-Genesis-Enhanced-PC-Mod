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
    unsigned char tracks[24]{},segments[32]{},events[368]{},sections[88]{},sequenceEvents[552]{};
    unsigned char copyTracks[24]{},copySegments[32]{},copyEvents[64*184]{},copySections[88]{};
    unsigned char blade[0x100]{},bladeCopy[0x100]{},nova[0x100]{},sound[0x100]{},other[0x100]{},assets[4][0x80]{};
} f;
unsigned char entries[40][256]{};uintptr_t names[40]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
bool gameThread=true;int mode=0;unsigned constructors=0,skeletonCalls=0,reserves=0;
Source source{};
template<class T>void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T>T At(const void* p,size_t offset){T value{};std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* object,unsigned index,const char* text){Put(object,0x18,int(index));Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),244,text);}
void* PackageNative(){return f.transient;}
void SkeletonNative(void* asset,void* skeleton){++skeletonCalls;Put(asset,0x38,reinterpret_cast<uintptr_t>(skeleton));}
void ReserveNative(void* array,int count){
    ++reserves;Check(array==f.copy+0x80&&count==4,"native reserve only receives private event header and merged count");
    if(mode==10)return;
    Put(array,0,Array{reinterpret_cast<uintptr_t>(mode==11?f.events:f.copyEvents),2,count});
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
    Put(f.copy,0x80,Array{reinterpret_cast<uintptr_t>(mode==3?f.events:f.copyEvents),2,2});
    Put(f.copy,0x148,Array{reinterpret_cast<uintptr_t>(mode==4?f.sections:f.copySections),1,1});
    Put(f.copy,0x158,Array{reinterpret_cast<uintptr_t>(mode==5?f.tracks:f.copyTracks),1,1});
    Put(f.copyTracks,8,Array{reinterpret_cast<uintptr_t>(mode==6?f.segments:f.copySegments),1,1});
    if(mode==7)Put(f.copy,8,uint32_t(0x50));
    if(mode==8)Put(f.copy,0x20,uintptr_t(0));
    if(mode==9)Put(f.copy,8,uint32_t(0));
    return f.copy;
}
void Fresh(){
    f={};entries[0][12]=0;gameThread=true;mode=0;constructors=skeletonCalls=reserves=0;g_ready=true;
    g_construct=ConstructNative;g_package=PackageNative;g_skeleton=SkeletonNative;g_reserve=ReserveNative;
    const char* labels[]={"AnimMontage","Class","AnimSequence","Skeleton","Package","AnimNotify_CallAbilityFunction","AnimNotify_PlayParticleEffect","AnimNotify_PlaySound"};
    const unsigned ids[]={1,2,3,4,5,6,17,18};for(unsigned i=0;i<8;++i)Label(f.classes[i],ids[i],labels[i]);
    Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
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
    for(unsigned i=0;i<40;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
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
}
namespace dg::native_tps {bool OnGameThread(){return gameThread;}}
int main(){try{Run();std::cout<<"Private native montage allocation, nine-argument ABI, notify timing, source preservation and alias rejection passed\n";return 0;}
catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
