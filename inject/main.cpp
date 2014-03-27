#include <Windows.h>
#include "TH9Monitor.h"
#include "AIManager.h"
#include <string>

ka_ai_duka::AIManager* ai_manager = nullptr;

void Attach(void){
    using namespace ka_ai_duka;
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
    ai_manager = new AIManager();
    std::string file_1p("C:/Users/ide/Desktop/1p.lua");
    std::string file_2p("C:/Users/ide/Desktop/2p.lua");
    ai_manager->SetFilename1P(file_1p);
    ai_manager->SetFilename2P(file_2p);
    //TODO: AI‰Šú‰»
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
        Attach();
        break;
    case DLL_PROCESS_DETACH:
        Detach();
        break;
    }
    return TRUE;
}
