#include "UpdateFeature.h"
#include <windows.h>
#include <winhttp.h>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <regex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace dg::update {
namespace {
namespace fs=std::filesystem;
constexpr char kRepo[]="DeadneM/Darksiders-Genesis-Enhanced-PC-Mod";
constexpr char kBuiltArchive[]="DarksidersGenesis_V1.14_Z_ASI_UPDATER_TEST.zip";
constexpr int kMajor=1,kMinor=14;
constexpr int kResourceId=101;
constexpr size_t kMaxResponse=4*1024*1024;
struct Package {
    int major=0,minor=0;
    long long bytes=0;
    bool beta=false;
    std::string name,url,sha;
};
std::mutex g_lock;
Status g_status{};
Package g_package{};
std::atomic_bool g_busy{false};

std::wstring Widen(const std::string& s){
    if(s.empty())return {};
    int count=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),nullptr,0);
    if(count<=0)throw std::runtime_error("Bad UTF-8 text");
    std::wstring result(static_cast<size_t>(count),L'\0');
    if(MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),result.data(),count)!=count)
        throw std::runtime_error("UTF-8 conversion error");
    return result;
}
struct Handle {
    HINTERNET h=nullptr;
    explicit Handle(HINTERNET ptr):h(ptr){}
    ~Handle(){if(h)WinHttpCloseHandle(h);}
    Handle(const Handle&)=delete;
    Handle& operator=(const Handle&)=delete;
};
std::string DownloadListing(bool beta){
    const std::wstring host=L"api.github.com";
    const std::wstring path=L"/repos/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/contents/downloads?ref="+
       std::wstring(beta?L"debug":L"main");
    Handle session(WinHttpOpen(L"DarksidersGenesisASI/1.14",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0));
    if(!session.h)throw std::runtime_error("WinHTTP unavailable");
    WinHttpSetTimeouts(session.h,5000,5000,12000,15000);
    Handle connect(WinHttpConnect(session.h,host.c_str(),INTERNET_DEFAULT_HTTPS_PORT,0));
    if(!connect.h)throw std::runtime_error("Could not reach api.github.com");
    Handle request(WinHttpOpenRequest(connect.h,L"GET",path.c_str(),nullptr,WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,WINHTTP_FLAG_SECURE));
    if(!request.h)throw std::runtime_error("Could not create HTTPS request");
    const wchar_t headers[]=L"Accept: application/vnd.github+json\r\n";
    WinHttpAddRequestHeaders(request.h,headers,static_cast<DWORD>(-1),WINHTTP_ADDREQ_FLAG_ADD);
    if(!WinHttpSendRequest(request.h,WINHTTP_NO_ADDITIONAL_HEADERS,0,
            WINHTTP_NO_REQUEST_DATA,0,0,0) || !WinHttpReceiveResponse(request.h,nullptr))
        throw std::runtime_error("GitHub request failed");
    DWORD status=0,bytes=sizeof(status);
    if(!WinHttpQueryHeaders(request.h,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,&status,&bytes,WINHTTP_NO_HEADER_INDEX) || status!=200)
        throw std::runtime_error("GitHub HTTP "+std::to_string(status));
    std::string output;
    for(;;){
        char buf[8192]{};
        DWORD read=0;
        if(!WinHttpReadData(request.h,buf,sizeof(buf),&read))
            throw std::runtime_error("GitHub response read failed");
        if(!read)break;
        if(output.size()+read>kMaxResponse)throw std::runtime_error("GitHub response too large");
        output.append(buf,read);
    }
    return output;
}
std::vector<std::string> Objects(const std::string& json){
    std::vector<std::string> results;
    bool quoted=false,escaped=false;
    int depth=0;
    size_t start=0;
    for(size_t i=0;i<json.size();++i){
        char c=json[i];
        if(quoted){
            if(escaped)escaped=false;
            else if(c=='\\')escaped=true;
            else if(c=='"')quoted=false;
        }else if(c=='"')quoted=true;
        else if(c=='{'){if(depth++==0)start=i;}
        else if(c=='}'){
            if(depth==0)throw std::runtime_error("Unexpected GitHub JSON");
            if(--depth==0)results.push_back(json.substr(start,i-start+1));
        }
    }
    if(depth||quoted)throw std::runtime_error("Incomplete GitHub JSON");
    return results;
}
std::string Field(const std::string& json,const char* name){
    const std::regex rx(std::string("\"")+name+"\"\\s*:\\s*\"([^\"]*)\"");
    std::smatch m;
    return std::regex_search(json,m,rx)?m[1].str():std::string{};
}
long long LengthField(const std::string& json){
    std::smatch m;
    if(!std::regex_search(json,m,std::regex("\"size\"\\s*:\\s*(\\d+)")))return 0;
    try{return std::stoll(m[1].str());}catch(...){return 0;}
}
Package Fetch(bool beta){
    std::string channel=beta?"debug":"main";
    const std::string prefix="https://raw.githubusercontent.com/"+std::string(kRepo)+
        "/"+channel+"/downloads/";
    const std::regex version(R"(^DarksidersGenesis_V(\d+)\.(\d+)(?:_[A-Za-z0-9_-]+)?\.zip$)");
    const std::regex hash("^[0-9a-fA-F]{40}$");
    Package latest{};
    for(const auto& obj:Objects(DownloadListing(beta))){
        const std::string name=Field(obj,"name");
        std::smatch match;
        if(!std::regex_match(name,match,version))continue;
        int major=0,minor=0;
        try{major=std::stoi(match[1].str());minor=std::stoi(match[2].str());}
        catch(...){continue;}
        const std::string url=Field(obj,"download_url");
        const std::string sha=Field(obj,"sha");
        const long long size=LengthField(obj);
        if(url!=prefix+name || size<1000 || size>32*1024*1024 ||
            !std::regex_match(sha,hash))continue;
        if(latest.name.empty() || major>latest.major ||
           (major==latest.major && minor>latest.minor) ||
           (major==latest.major && minor==latest.minor && name>latest.name)){
            latest={major,minor,size,beta,name,url,sha};
        }
    }
    if(latest.name.empty())throw std::runtime_error("No valid mod ZIP found on "+channel);
    return latest;
}
void PinThisModule(){
    HMODULE module=nullptr;
    if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN|GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
            reinterpret_cast<LPCWSTR>(&PinThisModule),&module))
        throw std::runtime_error("Background ASI lifetime not secured");
}
fs::path GameDir(){
    HMODULE module=nullptr;
    if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
            reinterpret_cast<LPCWSTR>(&GameDir),&module))
        throw std::runtime_error("ASI module location not found");
    wchar_t file[32768]{};
    const DWORD len=GetModuleFileNameW(module,file,32768);
    FreeLibrary(module);
    if(!len || len>=32768)throw std::runtime_error("ASI module pathname invalid");
    fs::path folder=fs::path(file).parent_path();
    if(!fs::exists(folder/L"DarksidersGenesis-Win64-Shipping.exe"))
        throw std::runtime_error("Supported game binary not found next to ASI");
    return folder;
}
fs::path ExtractHelper(){
    HMODULE module=nullptr;
    if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
            reinterpret_cast<LPCWSTR>(&ExtractHelper),&module))
        throw std::runtime_error("Helper resource module unavailable");
    HRSRC resource=FindResourceW(module,MAKEINTRESOURCEW(kResourceId),RT_RCDATA);
    if(!resource)throw std::runtime_error("No embedded updater resource");
    const DWORD length=SizeofResource(module,resource);
    HGLOBAL loaded=LoadResource(module,resource);
    const void* raw=loaded?LockResource(loaded):nullptr;
    if(!raw||length<1024 || length>2*1024*1024)
        throw std::runtime_error("Embedded updater failed validation");
    wchar_t local[32768]{};
    const DWORD count=GetEnvironmentVariableW(L"LOCALAPPDATA",local,32768);
    if(!count||count>=32768)throw std::runtime_error("LOCALAPPDATA not configured");
    fs::path dir=fs::path(local)/L"DarksidersGenesisEnhancedUpdater"/L"embedded";
    fs::create_directories(dir);
    fs::path target=dir/(L"Updater-1.14-"+std::to_wstring(GetCurrentProcessId())+L".exe");
    {
        std::ofstream out(target,std::ios::binary|std::ios::trunc);
        if(!out)throw std::runtime_error("Cannot write embedded updater");
        out.write(static_cast<const char*>(raw),length);
        if(!out.good())throw std::runtime_error("Updater helper write failed");
    }
    return target;
}
void Error(const std::string& text){
    std::lock_guard<std::mutex> guard(g_lock);
    g_status.message=text;
    g_status.busy=false;
    g_busy.store(false);
}
void DoCheck(bool beta){
    try{
        Package p=Fetch(beta);
        std::lock_guard<std::mutex> guard(g_lock);
        g_package=p;
        g_status.checkedBeta=beta;
        g_status.hasLatest=true;
        g_status.major=p.major;
        g_status.minor=p.minor;
        g_status.latest="V"+std::to_string(p.major)+"."+
            (p.minor<10?"0":"")+std::to_string(p.minor);
        g_status.newer=p.major>kMajor || (p.major==kMajor && p.minor>kMinor);
        const bool same=(p.major==kMajor && p.minor==kMinor && p.name==kBuiltArchive);
        g_status.sameBuild=same;
        g_status.message=same ? "This ASI matches the newest "+std::string(beta?"beta":"stable")+" build." :
            g_status.newer ? "Newer build found. Choose Install after Exit." :
            "Selected version differs from this ASI. Downgrade requires confirmation.";
        g_status.busy=false;
        g_busy.store(false);
    }catch(const std::exception& e){Error(std::string("Update check failed: ")+e.what());}
}
void StartHelperCommand(const std::wstring& arguments){
    const fs::path helper=ExtractHelper();
    const std::wstring text=L"\""+helper.wstring()+L"\" "+arguments;
    std::vector<wchar_t> cmd(text.begin(),text.end());
    cmd.push_back(L'\0');
    STARTUPINFOW si{}; si.cb=sizeof(si);
    PROCESS_INFORMATION pi{};
    if(!CreateProcessW(helper.c_str(),cmd.data(),nullptr,nullptr,FALSE,
         CREATE_NO_WINDOW|DETACHED_PROCESS,nullptr,nullptr,&si,&pi))
        throw std::runtime_error("Cannot launch post-exit helper");
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
}
void DoInstall(Package p){
    try{
        const fs::path folder=GameDir();
        const std::wstring channel=p.beta?L"debug":L"main";
        const std::wstring args=L"--install-on-exit \""+folder.wstring()+L"\" "+
            channel+L" \""+Widen(p.name)+L"\" "+Widen(p.sha);
        StartHelperCommand(args);
        std::lock_guard<std::mutex> guard(g_lock);
        g_status.busy=false;
        g_status.message="Update queued. Close the game; personal INI preserved.";
        g_busy.store(false);
    }catch(const std::exception& e){Error(std::string("Install staging failed: ")+e.what());}
}
void DoRestore(){
    try{
        const fs::path folder=GameDir();
        StartHelperCommand(L"--restore-on-exit \""+folder.wstring()+L"\"");
        std::lock_guard<std::mutex> guard(g_lock);
        g_status.busy=false;
        g_status.message="Rollback queued. Close game to restore previous mod files.";
        g_busy.store(false);
    }catch(const std::exception& e){Error(std::string("Rollback staging failed: ")+e.what());}
}
} // anonymous namespace

Status GetStatus(){
    std::lock_guard<std::mutex> guard(g_lock);
    return g_status;
}
void Invalidate(){
    if(g_busy.load())return;
    std::lock_guard<std::mutex> guard(g_lock);
    g_status=Status{};
    g_package=Package{};
}
void Check(bool beta){
    if(g_busy.exchange(true))return;
    {
        std::lock_guard<std::mutex> guard(g_lock);
        g_status.busy=true;
        g_status.hasLatest=false;
        g_status.message=beta?"Checking beta (debug) on GitHub...":"Checking stable (main) on GitHub...";
    }
    try{PinThisModule();std::thread([beta](){DoCheck(beta);}).detach();}
    catch(const std::exception& e){Error(e.what());}
}
void InstallAfterExit(bool beta){
    if(g_busy.exchange(true))return;
    Package selected{};
    {
        std::lock_guard<std::mutex> guard(g_lock);
        if(!g_status.hasLatest || g_status.sameBuild || g_status.checkedBeta!=beta){
            g_busy.store(false);return;
        }
        selected=g_package;
        g_status.busy=true;
        g_status.message="Preparing verified embedded post-exit updater...";
    }
    try{PinThisModule();std::thread([selected](){DoInstall(selected);}).detach();}
    catch(const std::exception& e){Error(e.what());}
}
void RestoreAfterExit(){
    if(g_busy.exchange(true))return;
    {
        std::lock_guard<std::mutex> guard(g_lock);
        g_status.busy=true;
        g_status.message="Preparing safe post-exit rollback...";
    }
    try{PinThisModule();std::thread([](){DoRestore();}).detach();}
    catch(const std::exception& e){Error(e.what());}
}
} // namespace dg::update
