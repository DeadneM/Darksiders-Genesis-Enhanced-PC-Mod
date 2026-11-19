#include "NativeObjectIdentity.h"
#include "DiscoveryObjectBatch.h"
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace {
struct Reader {
    std::map<uintptr_t,unsigned char> bytes;unsigned reads=0,blockReads=0;
    bool rejectBlocks=false;uintptr_t refusedRead=0;size_t refusedSize=0;
    template<class T> void Put(uintptr_t at,const T& value){
        const auto* raw=reinterpret_cast<const unsigned char*>(&value);
        for(size_t i=0;i<sizeof(value);++i)bytes[at+i]=raw[i];
    }
    template<class T> bool Read(uintptr_t at,size_t offset,T& value){
        ++reads;auto* raw=reinterpret_cast<unsigned char*>(&value);
        if(sizeof(value)==20){
            ++blockReads;
            if(rejectBlocks){std::memset(&value,0xA5,sizeof(value));return false;}
        }
        if(at+offset==refusedRead&&sizeof(value)==refusedSize)return false;
        for(size_t i=0;i<sizeof(value);++i){auto item=bytes.find(at+offset+i);if(item==bytes.end())return false;raw[i]=item->second;}
        return true;
    }
};
// Matches field-oriented template readers used by other native-layout tests:
// a larger read at the same address is refused rather than synthesized.
struct FieldReader {
    std::map<uintptr_t,std::vector<unsigned char>> fields;unsigned reads=0,blockReads=0;
    template<class T> void Put(uintptr_t at,const T& value){
        auto& bytes=fields[at];bytes.resize(sizeof(value));std::memcpy(bytes.data(),&value,sizeof(value));
    }
    template<class T> bool Read(uintptr_t at,size_t offset,T& value){
        ++reads;if(sizeof(value)==20)++blockReads;
        const auto item=fields.find(at+offset);
        if(item==fields.end()||item->second.size()!=sizeof(value))return false;
        std::memcpy(&value,item->second.data(),sizeof(value));return true;
    }
};
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
// Reference implementation before the grouped read. Compare failure outputs as
// well as the result, including the original short-circuit write order.
template<class R>
bool PreviousItem(R& reader,uintptr_t base,int32_t index,uintptr_t& address,int32_t& serial,uint32_t& flags){
    int32_t count=0;uintptr_t chunks=0,chunk=0;
    if(index<0||!reader.Read(base+0x3959694,0,count)||count<=0||count>2000000||index>=count||
       !reader.Read(base+0x3959680,0,chunks)||!chunks||
       !reader.Read(chunks,size_t(index/65536)*8,chunk)||!chunk)return false;
    const auto offset=size_t(index%65536)*24;
    return reader.Read(chunk,offset,address)&&address&&reader.Read(chunk,offset+8,flags)&&
        reader.Read(chunk,offset+16,serial)&&serial>=0&&!(flags&0x30000000u);
}
constexpr uintptr_t Base=0x10000000,Chunks=0x20000000,Chunk=0x21000000,Object=0x30000200;
constexpr int32_t Index=2;
constexpr uintptr_t Entry=Chunk+size_t(Index)*24;
template<class R> R IdentityReader(){
    R reader;reader.Put(Base+0x3959694,int32_t(16));reader.Put(Base+0x3959680,Chunks);reader.Put(Chunks,Chunk);
    reader.Put(Entry,Object);reader.Put(Entry+8,uint32_t(0x80));reader.Put(Entry+12,uint32_t(0xABCD));reader.Put(Entry+16,int32_t(7));
    reader.Put(Object+8,uint32_t(0));reader.Put(Object+0xC,Index);return reader;
}
struct ReadCounts {unsigned previous,current,blocks;};
template<class R> ReadCounts CompareItem(R reader,int32_t index,bool expected,const char* message){
    auto previous=reader;
    uintptr_t oldAddress=0xDEADBEEF,address=oldAddress;int32_t oldSerial=-123,serial=oldSerial;uint32_t oldFlags=0xA55AA55A,flags=oldFlags;
    const bool oldResult=PreviousItem(previous,Base,index,oldAddress,oldSerial,oldFlags);
    const bool result=dg::native_objects::Item(reader,Base,index,address,serial,flags);
    Check(oldResult==expected&&result==oldResult&&address==oldAddress&&serial==oldSerial&&flags==oldFlags,message);
    return {previous.reads,reader.reads,reader.blockReads};
}
template<class R> void CompareLifecycle(R reader,const dg::native_objects::Identity& identity,bool expectedAlive,bool expectedCapture){
    using namespace dg::native_objects;
    auto previous=reader;uintptr_t address=0;int32_t serial=0;uint32_t flags=0;
    const bool oldAlive=identity&&PreviousItem(previous,Base,identity.index,address,serial,flags)&&address==identity.address&&serial==identity.serial;
    Check(oldAlive==expectedAlive&&Alive(reader,Base,identity)==oldAlive,"old/new liveness and address-reuse guard agree");
    previous=reader;int32_t index=-1;uint32_t objectFlags=0;Identity oldCapture{0xABC,99,99},capture=oldCapture;
    const bool oldCaptured=previous.Read(Object,8,objectFlags)&&!(objectFlags&0x30u)&&previous.Read(Object,0xC,index)&&
        PreviousItem(previous,Base,index,address,serial,flags)&&address==Object&&serial>0;
    if(oldCaptured)oldCapture={address,index,serial};
    const bool captured=Capture(reader,Base,Object,capture);
    Check(oldCaptured==expectedCapture&&captured==oldCaptured&&capture==oldCapture,"old/new capture result and unchanged failure identity agree");
}
void RunItemBatchChecks(){
    auto reader=IdentityReader<Reader>();
    const auto success=CompareItem(reader,Index,true,"grouped item has the old success result and outputs");
    Check(success.previous==6&&success.current==4&&success.blocks==1,"successful Item uses four reads instead of six and exactly one 20-byte read");
    // No bytes at Entry+20: success also proves the request excludes tail padding.
    auto refused=reader;refused.rejectBlocks=true;
    const auto fallback=CompareItem(refused,Index,true,"a rejected and dirtied block falls back to the original field reads");
    Check(fallback.previous==6&&fallback.current==7&&fallback.blocks==1,"block refusal adds one attempt and preserves the six original reads");
    const auto fields=CompareItem(IdentityReader<FieldReader>(),Index,true,"sizeof-keyed field reader remains compatible");
    Check(fields.previous==6&&fields.current==7&&fields.blocks==1,"field-only reader rejects the block and accepts the three separate fields");
    for(const auto flags:{0x10000000u,0x20000000u,0x30000000u}){
        auto gc=reader;gc.Put(Entry+8,flags);
        CompareItem(gc,Index,false,"grouped read preserves both native GC rejection flags and all outputs");
        CompareLifecycle(gc,{Object,Index,7},false,false);
        gc.rejectBlocks=true;CompareItem(gc,Index,false,"fallback preserves native GC rejection and outputs");
    }
    for(const auto serial:{-1,0,7}){
        auto generation=reader;generation.Put(Entry+16,int32_t(serial));
        CompareItem(generation,Index,serial>=0,"Item preserves negative rejection and zero-serial acceptance");
        CompareLifecycle(generation,{Object,Index,7},serial==7,serial>0);
    }
    CompareLifecycle(reader,{Object,Index,7},true,true);
    auto reused=reader;reused.Put(Entry+16,int32_t(8));CompareLifecycle(reused,{Object,Index,7},false,true);
    reused.Put(Entry,uintptr_t(Object+0x100));CompareLifecycle(reused,{Object,Index,8},false,false);
    for(const auto flags:{0x10u,0x20u,0x30u}){
        auto gc=reader;gc.Put(Object+8,flags);CompareLifecycle(gc,{Object,Index,7},true,false);
    }
    auto nullAddress=reader;nullAddress.Put(Entry,uintptr_t(0));
    CompareItem(nullAddress,Index,false,"null address leaves flags and serial outputs untouched");
    nullAddress.rejectBlocks=true;CompareItem(nullAddress,Index,false,"fallback keeps the null-address short circuit");
    for(const auto offset:{size_t(0),size_t(8),size_t(16)}){
        auto unreadable=refused;unreadable.refusedRead=Entry+offset;unreadable.refusedSize=offset?4:sizeof(uintptr_t);
        CompareItem(unreadable,Index,false,"field refusal preserves every previously written output and skips later outputs");
    }
    auto partial=reader;partial.bytes.erase(Entry+18);
    CompareItem(partial,Index,false,"partial block and serial failures preserve the original partial field output");
    Check(CompareItem(reader,-1,false,"negative index stays rejected before any read").current==0,"negative index does not touch memory");
    CompareItem(reader,16,false,"index equal to count stays rejected");
    for(const auto count:{-1,0,2000001}){
        auto bounded=reader;bounded.Put(Base+0x3959694,int32_t(count));
        Check(CompareItem(bounded,Index,false,"invalid object count stays rejected").blocks==0,"count guard precedes item access");
    }
    for(const auto at:{Base+0x3959680,Chunks}){
        auto missing=reader;missing.Put(at,uintptr_t(0));
        Check(CompareItem(missing,Index,false,"null chunk pointers stay rejected").blocks==0,"chunk guards precede item access");
    }
    for(const auto at:{Base+0x3959694,Base+0x3959680,Chunks}){
        auto missing=reader;missing.bytes.erase(at);
        Check(CompareItem(missing,Index,false,"unreadable table metadata stays rejected").blocks==0,"metadata read refusals precede item access");
    }
    auto boundary=reader;boundary.Put(Base+0x3959694,int32_t(65537));boundary.Put(Chunks+8,Chunk);
    boundary.Put(Chunk,Object);boundary.Put(Chunk+8,uint32_t(0));boundary.Put(Chunk+12,uint32_t(0));boundary.Put(Chunk+16,int32_t(7));
    Check(CompareItem(boundary,65536,true,"chunk boundary preserves index division and 24-byte stride").current==4,"chunk-boundary item also uses four reads");
}
void Run(){
    using namespace dg::native_objects;
    RunItemBatchChecks();
    Reader reader;const uintptr_t base=0x10000000,chunks=0x20000000,chunk=0x21000000;
    reader.Put(base+0x3959694,int32_t(16));reader.Put(base+0x3959680,chunks);reader.Put(chunks,chunk);
    const auto object=[&](int32_t index,int32_t serial,uint32_t flags=0){
        const uintptr_t address=0x30000000+uintptr_t(index)*0x100;
        reader.Put(address+8,uint32_t(0));reader.Put(address+0xC,index);
        reader.Put(chunk+size_t(index)*24,address);reader.Put(chunk+size_t(index)*24+8,flags);
        reader.Put(chunk+size_t(index)*24+16,serial);return address;
    };
    // Discovery's cached chunk is scoped to one slice, with unchanged flags
    // and index limits. Cross a chunk boundary and replace its table entry.
    Reader scan;const uintptr_t table=0x40000000,left=0x41000000,right=0x42000000;
    scan.Put(table,left);scan.Put(table+8,right);
    struct Head {uintptr_t object;uint32_t flags,cluster;};
    scan.Put(left+65535u*24,Head{0x50000000,0,0});scan.Put(right,Head{0x50000100,0,0});scan.Put(right+24,Head{0x50000200,0,0});
    DiscoveryObjectBatch batch(table,65538);uintptr_t found=0;
    Check(batch.Read(scan,65535,found)&&found==0x50000000,"last object before chunk boundary");
    Check(batch.Read(scan,65536,found)&&found==0x50000100&&batch.Read(scan,65537,found)&&found==0x50000200,"first objects after chunk boundary");
    Check(scan.reads==5,"three item reads need two chunk-table reads, not a read per object");
    scan.Put(right+24,Head{0x50000200,0x10000000,0});Check(!batch.Read(scan,65537,found)&&!found,"unreachable object is still rejected");
    scan.Put(right+24,Head{0x50000200,0x20000000,0});Check(!batch.Read(scan,65537,found)&&!found,"pending-kill object is still rejected");
    Check(!batch.Read(scan,-1,found)&&!batch.Read(scan,65538,found),"out-of-snapshot indices are rejected");
    const uintptr_t newer=0x43000000;scan.Put(table+8,newer);scan.Put(newer,Head{0x50000300,0,0});DiscoveryObjectBatch next(table,65538);
    Check(next.Read(scan,65536,found)&&found==0x50000300,"a new slice reads the replacement chunk instead of stale cached memory");
    Reader late;DiscoveryObjectBatch retry(table,2);Check(!retry.Read(late,0,found),"missing chunk table fails open");late.Put(table,left);late.Put(left,Head{0x50000400,0,0});
    Check(retry.Read(late,0,found)&&found==0x50000400,"failed chunk read is not cached as a permanent failure");
    Baselines<float,4> cache;
    Identity first{};Check(Capture(reader,base,object(0,1),first),"live native table identity");
    auto* native=cache.Acquire(reader,base,first);Check(native,"first baseline slot");*native=2;
    for(int32_t i=1;i<4;++i){Identity id{};Check(Capture(reader,base,object(i,1),id),"other live object");*cache.Acquire(reader,base,id)=2;}
    Identity fifth{};Check(Capture(reader,base,object(4,1),fifth),"fifth live object");
    Check(!cache.Acquire(reader,base,fifth),"full cache cannot evict a modified live baseline");
    Check(*cache.Find(first)==2,"revisiting first object retains original 2s, not modified 20s");
    object(0,2);Check(!Alive(reader,base,first),"serial change invalidates same-address old UObject");
    Check(cache.Acquire(reader,base,fifth),"expired table identity releases its baseline slot");
    Identity reused{};Check(Capture(reader,base,object(0,2),reused)&&reused!=first,"reused address has a new generation");
    object(1,1,0x10000000);Identity doomed{};
    Check(!Capture(reader,base,0x30000100,doomed),"pending/unreachable table entry rejected");
    Check(!Capture(reader,base,uintptr_t(123),doomed),"invalid live pointer fails open");
    Check(!Capture(reader,base,object(5,0),doomed),"unassigned serial cannot be guessed as a persistent identity");
}
}
int main(){try{Run();std::cout<<"Native object serial/baseline lifecycle checks passed\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
