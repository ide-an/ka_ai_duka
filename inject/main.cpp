#include <Windows.h>
#include "TH9Monitor.h"
#include "AIManager.h"

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
    monitor->SetObserver(ai_manager);
    if(monitor){
        monitor->Attach();
    }
    //TODO: AI Manager instantiation
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
