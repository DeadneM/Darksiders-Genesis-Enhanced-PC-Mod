#pragma once
#include "BuildInfo.h"
#include <string_view>
namespace dg::update {
constexpr bool NewerThanCurrent(int major,int minor){
    return major>build::Major||(major==build::Major&&minor>build::Minor);
}
constexpr bool MatchesCurrent(int major,int minor,std::string_view archive){
    return major==build::Major&&minor==build::Minor&&archive==build::Archive;
}
}
