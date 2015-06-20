#include <Windows.h>
#include "TH9Monitor.h"
#include "AIManager.h"
#include <string>
#include "../common/common.h"
#include "../common/errorhandle.h"

ka_ai_duka::AIManager* ai_manager = nullptr;

void Attach(HANDLE hModule){
    using namespace ka_ai_duka;
    common::Config conf;
    std::string ini_path;
    try{
        common::Config::IniFilePath(hModule, ini_path);
        conf.Load(ini_path);
    }catch(const std::exception &e){
        std::ostringstream os;
        os << e.what() << std::endl;
        ka_ai_duka::ReportError(os);
        return;
    }
    switch(TH9Monitor::CheckVersion()){
    case Ver1_5a:
        monitor = new TH9ver1_5aMonitor();
        break;
    case Ver1_0:
        monitor = new TH9ver1_0Monitor();
        break;
    default:
        assert(0);
        break;
    }
    monitor->SetCanSaveSnapshot(conf.EnableSnapshot());
    ai_manager = new AIManager();
    if(conf.Enable1P()){
        ai_manager->SetFilename1P(conf.ScriptPath1P());
    }
    if(conf.Enable2P()){
        ai_manager->SetFilename2P(conf.ScriptPath2P());
    }
    monitor->SetObserver(ai_manager);
    if(monitor){
        monitor->Attach();
    }
}

void Detach(void){
    using namespace ka_ai_duka;
    if(monitor){
        monitor->Detach();
        delete monitor;
        monitor = nullptr;
    }
    if(ai_manager){
        delete ai_manager;
        ai_manager = nullptr;
    }
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
    switch(ul_reason_for_call){
    case DLL_PROCESS_ATTACH:
        Attach(hModule);
        break;
    case DLL_PROCESS_DETACH:
        Detach();
        break;
    }
    return TRUE;
}
