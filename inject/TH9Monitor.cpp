#include "TH9Monitor.h"
#include <Windows.h>

namespace ka_ai_duka{
    TH9Monitor* monitor = nullptr;
    
    void TH9Monitor::SetJumpTo(char* code, int from, int to)
    {
        *((int*)code) = to - from;
    }

    void TH9Monitor::WriteCode(char* inject_to, char* new_code, size_t size)
    {
        DWORD old_protect;
        ::VirtualProtect(inject_to, size, PAGE_EXECUTE_READWRITE, &old_protect);
        ::memcpy_s(inject_to, size, new_code, size);
    }

    void TH9Monitor::OnFrameUpdate(void)
    {
        if(observer){
            observer->OnFrameUpdate(*this);
        }
    }

    void TH9Monitor::OnGameStart(void)
    {
        ::MessageBeep(MB_ICONASTERISK);
        
        if(observer){
            observer->OnGameStart(*this);
        }
        //char s[0xff];
        //::sprintf(s,"%X", address::globals_ver1_5->d3d8_device);
        //::MessageBoxA(NULL, s, "D3DDeivce",MB_OK);
    }

    void TH9Monitor::OnGameEnd(void)
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        if(observer){
            observer->OnGameEnd(*this);
        }
    }
}
