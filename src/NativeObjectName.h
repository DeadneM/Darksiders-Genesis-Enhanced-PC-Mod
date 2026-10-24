#pragma once
#include "NativeMemoryReader.h"
#include <array>
#include <string_view>

namespace dg::native_objects {
// Bounded lookup for the new transfer tables; literal-name callers below keep
// their existing implementation and overload resolution unchanged.
inline bool NameValueEquals(uintptr_t base,int index,int number,std::string_view expected){
    MemoryReader r;uintptr_t names=0,chunk=0,entry=0;uint32_t flags=0;
    if(!base||number||index<0||index>=2000000||expected.empty()||expected.size()>255||
       !r.Read(base+0x3955198,0,names)||!r.Read(names,size_t(index/16384)*8,chunk)||
       !r.Read(chunk,size_t(index%16384)*8,entry)||!r.Read(entry,8,flags)||entry>UINTPTR_MAX-0xC)return false;
    const auto address=entry+0xC;const size_t bytes=(expected.size()+1)*((flags&1)?sizeof(wchar_t):1);
    MEMORY_BASIC_INFORMATION m{};if(!VirtualQuery(reinterpret_cast<void*>(address),&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto access=m.Protect&255;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=address-reinterpret_cast<uintptr_t>(m.BaseAddress);if(offset>m.RegionSize||bytes>m.RegionSize-offset)return false;
    if(flags&1){const auto* text=reinterpret_cast<const wchar_t*>(address);for(size_t i=0;i<expected.size();++i)if(text[i]!=wchar_t(expected[i]))return false;return text[expected.size()]==0;}
    const auto* text=reinterpret_cast<const char*>(address);return !std::memcmp(text,expected.data(),expected.size())&&text[expected.size()]==0;
}
template<size_t N> bool NameValueIs(uintptr_t base,int index,int number,const char (&expected)[N]){
    MemoryReader r;uintptr_t names=0,chunk=0,entry=0;uint32_t flags=0;
    if(!base||number||index<0||index>=2000000||!r.Read(base+0x3955198,0,names)||
       !r.Read(names,size_t(index/16384)*8,chunk)||!r.Read(chunk,size_t(index%16384)*8,entry)||
       !r.Read(entry,8,flags))return false;
    if(flags&1){
        std::array<wchar_t,N> text{};if(!r.Read(entry,0xC,text))return false;
        for(size_t i=0;i<N;++i)if(text[i]!=wchar_t(expected[i]))return false;
        return true;
    }
    std::array<char,N> text{};return r.Read(entry,0xC,text)&&std::memcmp(text.data(),expected,N)==0;
}
template<size_t N> bool NameIs(uintptr_t base,uintptr_t object,const char (&expected)[N]){
    MemoryReader r;int index=-1,number=-1;
    return r.Read(object,0x18,index)&&r.Read(object,0x1C,number)&&NameValueIs(base,index,number,expected);
}
}
