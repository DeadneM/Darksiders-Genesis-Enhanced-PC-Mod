#pragma once
#include "NativeMemoryReader.h"
#include <array>

namespace dg::native_objects {
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
