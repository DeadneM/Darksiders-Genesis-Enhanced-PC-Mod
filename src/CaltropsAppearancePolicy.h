#pragma once
namespace dg::caltrops_appearance {
// 0 Native, 1 Base, 2/3/4 Frost levels 1/2/3. Only two emitted FX roles.
inline constexpr int Normalize(int choice){return choice>=0&&choice<=4?choice:0;}
inline constexpr int Target(int choice,int source){
    if(!Normalize(choice)||source<0||source>5)return source;
    return source<2?(choice==1?0:1):choice+1;
}
}
