#include <Windows.h>
#include "TH9Monitor.h"

void Attach(void){
    using namespace ka_ai_duka;
    switch(TH9Monitor::CheckVersion()){
    case Ver1_5a:
        monitor = new TH9ver1_5aMonitor();
        break;
    case Ver1_0:
        monitor = new TH09ver1_0Monitor();
        break;
    default:
        assert(0);
        break;
    }
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
