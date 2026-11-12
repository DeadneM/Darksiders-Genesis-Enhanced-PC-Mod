#include "NativeObjectIdentity.h"
#include "DiscoveryObjectBatch.h"
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>

namespace {
struct Reader {
    std::map<uintptr_t,unsigned char> bytes;unsigned reads=0;
    template<class T> void Put(uintptr_t at,const T& value){
        const auto* raw=reinterpret_cast<const unsigned char*>(&value);
        for(size_t i=0;i<sizeof(value);++i)bytes[at+i]=raw[i];
    }
    template<class T> bool Read(uintptr_t at,size_t offset,T& value){
        ++reads;auto* raw=reinterpret_cast<unsigned char*>(&value);
        for(size_t i=0;i<sizeof(value);++i){auto item=bytes.find(at+offset+i);if(item==bytes.end())return false;raw[i]=item->second;}
        return true;
    }
};
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
void Run(){
    using namespace dg::native_objects;
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
