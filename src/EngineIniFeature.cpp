#include "EngineIniFeature.h"
#include "UpdateFeature.h"
#include <cstdio>

namespace dg::engine_ini {
namespace {LogFn g_logger=nullptr;}
void Initialize(LogFn logger){g_logger=logger;}
bool QueueGraphicsAdapter(int adapter){
    if(adapter<0||adapter>4)return false;
    std::string error;
    const bool queued=update::QueueGraphicsAdapterAfterExit(error);
    if(g_logger){
        char message[512]{};
        sprintf_s(message,"GPU V1.151: preference=%d %s; %s",adapter,queued?"queued for game exit":"queue FAILED",
            queued?"helper reads final saved choice after UE closes; no startup or in-session Engine.ini write":error.c_str());
        g_logger(message);
    }
    return queued;
}
}
