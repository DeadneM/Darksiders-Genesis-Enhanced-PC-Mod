#include "CoreUiRefreshPolicy.h"
#include <stdexcept>
#include <iostream>
void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int main(){try{
    dg::core_ui::Warmup first;
    Check(!first.Next(),"first widget tick cannot refresh during initial construction");
    Check(first.Next()&&first.completed,"refresh consumed only after two completed ticks");
    for(int i=0;i<100;++i)Check(!first.Next(),"nested events and later ticks cannot replay a completed refresh");
    dg::core_ui::Warmup second;
    Check(!second.Next()&&second.Next(),"another widget has an independent first-display refresh");
    first={};Check(!first.Next()&&first.Next(),"replacement identity starts a fresh warmup");
    std::cout<<"Core UI refresh lifecycle checks passed\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
