#pragma once
#include <string>

namespace dg::update {
struct Status {
    bool busy = false;
    bool hasLatest = false;
    bool checkedBeta = false;
    bool newer = false;
    int major = 0;
    int minor = 0;
    std::string latest;
    std::string message = "Choose a channel, then Check for Updates.";
};
Status GetStatus();
void Check(bool beta);
void InstallAfterExit(bool beta);
void Invalidate();
}
