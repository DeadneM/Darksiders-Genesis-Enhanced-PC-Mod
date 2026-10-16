#include "ConfigStore.h"
#include <windows.h>
#include <cstdio>
#include <string>
int main(){
    unsigned failures=0;
    auto check=[&](bool ok,const char* name){if(!ok){++failures;std::printf("FAIL %s\n",name);}};
    wchar_t directory[MAX_PATH]{},path[MAX_PATH]{};
    if(!GetTempPathW(MAX_PATH,directory)||!GetTempFileNameW(directory,L"DGW",0,path))return 1;
    // The test owns this file, then a same-named directory. No game files are touched.
    if(!DeleteFileW(path)||!CreateDirectoryW(path,nullptr))return 1;
    dg::config::Store config;config.SetPath(path);
    check(!config.Load(),"directory path is rejected when loading INI");
    config.cameraStickSpeed=217.0f;config.Save();
    check(!config.SaveNow(),"writing an INI to a directory reports failure");
    check(config.IsDirty(),"failed write stays pending");
    const std::wstring missing=std::wstring(path)+L"\\missing\\settings.ini";
    dg::config::Store noFile;noFile.SetPath(missing);
    check(!noFile.Load(),"failed missing-INI generation is reported");
    check(noFile.IsDirty(),"failed generation remains retryable");
    const std::wstring valid=std::wstring(path)+L"\\settings.ini";
    config.SetPath(valid);
    check(config.SaveNow(),"subsequent write to valid path succeeds");
    check(!config.IsDirty(),"successful retry clears pending state");
    dg::config::Store loaded;loaded.SetPath(valid);
    check(loaded.Load()&&loaded.cameraStickSpeed==217.0f,"successful retry preserves requested value");
    DeleteFileW(valid.c_str());RemoveDirectoryW(path);
    return failures?1:0;
}
