#include "../src/RenderSafetyFeature.cpp"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace {
using namespace dg::render_safety;
unsigned calls=0;void* seenContext=nullptr;const void* seenInput=nullptr;TargetsCopy seen{};
void Check(bool value,const char* text){if(!value)throw std::runtime_error(text);}
template<class T> void Put(TargetsCopy& packet,size_t offset,T value){std::memcpy(packet.bytes.data()+offset,&value,sizeof(value));}
void Original(void* context,const void* packet){++calls;seenContext=context;seenInput=packet;if(packet)std::memcpy(seen.bytes.data(),packet,TargetsBytes);}
void Run(){
    TargetsCopy captured;auto& b=captured.bytes;
    Put<uintptr_t>(captured,0,0x12345678);Put<uint32_t>(captured,0xC0,1);
    const unsigned char actions[]={0,0,2,1,0x20,0,0,0};std::memcpy(b.data()+0xD0,actions,8);b[0xD9]=1;
    // Preserve even unused/padding/UAV bytes. The count is zero in all four dumps.
    for(size_t i=0xE0;i<0x120;++i)b[i]=static_cast<unsigned char>(i);
    const auto before=captured;void* context=reinterpret_cast<void*>(uintptr_t{0x87654321});
    Check(Forward(Original,context,b.data(),true)&&calls==1&&seenContext==context&&seenInput!=b.data(),"one call with the same context and a private packet");
    Check(captured.bytes==before.bytes,"caller input and full UAV tail remain unchanged");
    unsigned differences=0;for(size_t i=0;i<TargetsBytes;++i)if(seen.bytes[i]!=before.bytes[i]){++differences;Check(i==0xD9&&seen.bytes[i]==0,"only the impossible stencil clear is removed");}
    Check(differences==1,"exactly one byte changes");
    const auto pass=[&](const void* packet,bool enabled){const auto n=calls;Check(!Forward(Original,context,packet,enabled)&&calls==n+1&&seenInput==packet&&seenContext==context,"nonmatching/disabled request forwards exact pointer once");};
    pass(b.data(),false);pass(nullptr,true);
    auto valid=captured;Put<uintptr_t>(valid,0xC8,0x1111);pass(valid.bytes.data(),true);
    for(const auto offset:{size_t(0xC4),size_t(0xD8)}){auto p=captured;p.bytes[offset]=1;pass(p.bytes.data(),true);}
    auto noClear=captured;noClear.bytes[0xD9]=0;pass(noClear.bytes.data(),true);
    for(const auto count:{0u,2u,8u,99u}){auto p=captured;Put<uint32_t>(p,0xC0,count);pass(p.bytes.data(),true);}
    auto noColor=captured;Put<uintptr_t>(noColor,0,0);pass(noColor.bytes.data(),true);
    auto uav=captured;Put<uint32_t>(uav,0x120,1);pass(uav.bytes.data(),true);
    auto other=captured;other.bytes[0xD4]=0;pass(other.bytes.data(),true);
    g_original=Original;dg::runtime::Snapshot settings{};dg::runtime::Publish(settings);
    const auto n=calls;HookSetTargets(context,b.data());Check(calls==n+1&&AvoidedClears()==1,"hook integrates the single correction and counter");
    settings.molochBossCrashFixEnabled=false;dg::runtime::Publish(settings);HookSetTargets(context,b.data());Check(AvoidedClears()==1&&seenInput==b.data(),"runtime checkbox disables correction");
    std::vector<unsigned char> image(0xD5D100);auto base=reinterpret_cast<uintptr_t>(image.data());
    const unsigned char entry[]={0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x74,0x24,0x20,0x55,0x57,0x41,0x56,0x48,0x8D};
    const unsigned char uavs[]={0x48,0x63,0xBA,0x20,0x01,0,0};
    const unsigned char flags[]={0x44,0x0F,0xB6,0x83,0xD8,0,0,0,0x0F,0xB6,0x93,0xD9,0,0,0};
    const unsigned char fault[]={0x48,0x8B,0x83,0xC8,0,0,0,0x0F,0x10,0x40,0x20};
    std::memcpy(image.data()+0xD5CDD0,entry,sizeof(entry));std::memcpy(image.data()+0xD5CDF8,uavs,sizeof(uavs));std::memcpy(image.data()+0xD5D082,flags,sizeof(flags));std::memcpy(image.data()+0xD5D09A,fault,sizeof(fault));
    Check(Boundary(base),"supported entry, UAV layout and fault consumers match");
    for(const auto at:{0xD5CDD0,0xD5CDF8,0xD5D082,0xD5D09A}){image[at]^=1;Check(!Boundary(base),"any changed boundary refuses interception");image[at]^=1;}
    Check(!Boundary(0),"missing module refuses interception");
    std::cout<<"Captured invalid clear, valid pointer identity, full 0x128 input, runtime switch and exact RHI boundaries passed\n";
}
}
int main(){try{Run();return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
