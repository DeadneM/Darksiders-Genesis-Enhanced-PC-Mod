#include "../src/ForeignMontage.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using namespace dg::foreign_montage;
struct Fixture {
    unsigned char original[0x200]{},copy[0x200]{},war[0x180]{},idle[0x180]{},warSkeleton[0x180]{},strifeSkeleton[0x180]{};
    unsigned char classes[6][0x80]{},originalPackage[0x80]{},warPackage[0x80]{},transient[0x80]{},notify[0x80]{},notifyVtable[0x280]{};
    unsigned char tracks[24]{},segments[32]{},events[368]{},sections[88]{},sequenceEvents[184]{};
    unsigned char copyTracks[24]{},copySegments[32]{},copyEvents[368]{},copySections[88]{};
} f;
unsigned char entries[40][256]{};uintptr_t names[40]{},chunks[1]{reinterpret_cast<uintptr_t>(names)};
bool gameThread=true;int mode=0;unsigned constructors=0,skeletonCalls=0;
Source source{};
template<class T>void Put(void* p,size_t offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
template<class T>T At(const void* p,size_t offset){T value{};std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof(value));return value;}
void Check(bool pass,const char* why){if(!pass)throw std::runtime_error(why);}
void Label(void* object,unsigned index,const char* text){Put(object,0x18,int(index));Put(object,0x1C,0);strcpy_s(reinterpret_cast<char*>(entries[index]+12),244,text);}
void* PackageNative(){return f.transient;}
void SkeletonNative(void* asset,void* skeleton){++skeletonCalls;Put(asset,0x38,reinterpret_cast<uintptr_t>(skeleton));}
void* ConstructNative(void* cls,void* outer,Name name,uint32_t flags,uint32_t internal,void* templ,bool transients,void* graph,bool assume){
    ++constructors;
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
    f={};entries[0][12]=0;gameThread=true;mode=0;constructors=skeletonCalls=0;g_ready=true;
    g_construct=ConstructNative;g_package=PackageNative;g_skeleton=SkeletonNative;
    const char* labels[]={"AnimMontage","Class","AnimSequence","Skeleton","Package","AnimNotify_CallAbilityFunction"};
    for(unsigned i=0;i<6;++i)Label(f.classes[i],i+1,labels[i]);
    Put(f.classes[0],0x10,reinterpret_cast<uintptr_t>(f.classes[1]));
    Put(f.original,0x10,reinterpret_cast<uintptr_t>(f.classes[0]));Label(f.original,7,"War_Wrath_BladeGeyser_Montage");
    Label(f.originalPackage,8,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Montage");
    Put(f.original,0x20,reinterpret_cast<uintptr_t>(f.originalPackage));Put(f.original,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));
    Put(f.original,0x90,2.3666665554f);Put(f.original,0x94,1.0f);
    Put(f.war,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));Label(f.war,9,"War_Wrath_BladeGeyser");
    Label(f.warPackage,10,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser");
    Put(f.war,0x20,reinterpret_cast<uintptr_t>(f.warPackage));Put(f.war,0x38,reinterpret_cast<uintptr_t>(f.warSkeleton));
    Put(f.war,0x80,Array{reinterpret_cast<uintptr_t>(f.sequenceEvents),1,1});
    Label(f.warSkeleton,11,"War_Mesh_Skeleton");Put(f.warSkeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
    Label(f.strifeSkeleton,12,"Strife_Mesh_Skeleton");Put(f.strifeSkeleton,0x10,reinterpret_cast<uintptr_t>(f.classes[3]));
    Label(f.idle,13,"Strife_Idle");Put(f.idle,0x10,reinterpret_cast<uintptr_t>(f.classes[2]));
    Put(f.idle,0x38,reinterpret_cast<uintptr_t>(f.strifeSkeleton));Put(f.idle,0x90,2.6666667461f);Put(f.idle,0x94,1.0f);
    Label(f.transient,14,"/Engine/Transient");Put(f.transient,0x10,reinterpret_cast<uintptr_t>(f.classes[4]));
    strcpy_s(reinterpret_cast<char*>(entries[15]+12),244,"TPS");source={reinterpret_cast<uintptr_t>(f.idle),reinterpret_cast<uintptr_t>(f.strifeSkeleton),15,0};
    Put(f.tracks,0,Name{15,0});Put(f.tracks,8,Array{reinterpret_cast<uintptr_t>(f.segments),1,1});
    Segment segment{};segment.sequence=reinterpret_cast<uintptr_t>(f.war);segment.last=2.3666665554f;segment.rate=1;segment.loops=1;segment.valid=1;Put(f.segments,0,segment);
    Put(f.original,0x80,Array{reinterpret_cast<uintptr_t>(f.events),2,2});
    Put(f.original,0x148,Array{reinterpret_cast<uintptr_t>(f.sections),1,1});Put(f.original,0x158,Array{reinterpret_cast<uintptr_t>(f.tracks),1,1});
    Put(f.notify,0,reinterpret_cast<uintptr_t>(f.notifyVtable));Put(f.notify,0x10,reinterpret_cast<uintptr_t>(f.classes[5]));
    Put(f.notify,0x20,reinterpret_cast<uintptr_t>(f.war));strcpy_s(reinterpret_cast<char*>(entries[16]+12),244,"SpawnBlast");
    Put(f.notify,0x38,Name{16,0});Put(f.notifyVtable,0x240,g_base+0x5A7C60);
    Put(f.sequenceEvents,0x48,reinterpret_cast<uintptr_t>(f.notify));Put(f.sequenceEvents,0x24,.4880667329f);
    Put(f.sequenceEvents,0x28,reinterpret_cast<uintptr_t>(f.war));
}
void Run(){
    std::vector<unsigned char> image(0x3960000);g_base=reinterpret_cast<uintptr_t>(image.data());Put(image.data(),0x3955198,reinterpret_cast<uintptr_t>(chunks));
    for(unsigned i=0;i<40;++i)names[i]=reinterpret_cast<uintptr_t>(entries[i]);
    Fresh();const auto original=f;
    Check(CanBuild(source)&&Geyser(reinterpret_cast<uintptr_t>(f.original),source)==f.copy,"known template creates private compatible native montage");
    Check(constructors==1&&skeletonCalls==1,"one engine-owned constructor and native skeleton assignment");
    Check(!std::memcmp(f.original,original.original,sizeof(f.original))&&!std::memcmp(f.war,original.war,sizeof(f.war))&&
          !std::memcmp(f.sequenceEvents,original.sequenceEvents,sizeof(f.sequenceEvents))&&!std::memcmp(f.events,original.events,sizeof(f.events))&&
          !std::memcmp(f.tracks,original.tracks,sizeof(f.tracks))&&!std::memcmp(f.segments,original.segments,sizeof(f.segments))&&
          !std::memcmp(f.sections,original.sections,sizeof(f.sections)),"all source assets and arrays remain byte-identical");
    Check(At<int>(f.copy,0x88)==1&&At<uintptr_t>(f.copy,0x38)==source.skeleton&&At<float>(f.copy,0x90)==2.3666665554f,"one notify on original duration with compatible skeleton");
    const auto segment=At<Segment>(f.copySegments,0);
    Check(segment.sequence==source.sequence&&segment.last==2.6666667461f&&std::abs(segment.last/segment.rate-2.3666665554f)<.00001f,"compatible idle segment retains source timeline");
    Check(At<uintptr_t>(f.copyEvents,0x48)==reinterpret_cast<uintptr_t>(f.notify)&&At<float>(f.copyEvents,0x24)==.4880667329f&&
          At<uintptr_t>(f.copyEvents,8)==reinterpret_cast<uintptr_t>(f.copy)&&At<uintptr_t>(f.copyEvents,0x28)==source.sequence&&
          At<uintptr_t>(f.copyEvents,0x68)==reinterpret_cast<uintptr_t>(f.copy)&&At<float>(f.copySections,0x24)==0,
          "native SpawnBlast callback and queued time retained with only private link targets replaced");
    for(int failure=1;failure<=9;++failure){
        Fresh();mode=failure;const auto before=f;
        Check(!Geyser(reinterpret_cast<uintptr_t>(f.original),source)&&!skeletonCalls,"allocation, alias, CDO and outer failures reject before first mutation");
        Check(!std::memcmp(f.original,before.original,sizeof(f.original))&&!std::memcmp(f.events,before.events,sizeof(f.events))&&
              !std::memcmp(f.tracks,before.tracks,sizeof(f.tracks))&&!std::memcmp(f.segments,before.segments,sizeof(f.segments))&&
              !std::memcmp(f.sections,before.sections,sizeof(f.sections)),"failed clone never writes source buffers");
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
