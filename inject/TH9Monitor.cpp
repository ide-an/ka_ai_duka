#include "TH9Monitor.h"
#include <Windows.h>

namespace ka_ai_duka{
    TH9Monitor* monitor = nullptr;
    
    int __declspec(naked) OnFrameUpdateVer1_5(void)
    {   
        __asm{
            pushad;
            pushfd;
        }
        if(monitor){
            monitor->OnFrameUpdate();
        }
        __asm{
            popfd;
            popad;
            mov eax, 1;
            ret;
        }
    }

    void SetJumpTo(char* code, int from, int to){
        *((int*)code) = to - from;
    }

    void WriteCode(char* inject_to, char* new_code, size_t size)
    {
        DWORD old_protect;
        ::VirtualProtect(inject_to, size, PAGE_EXECUTE_READWRITE, &old_protect);
        ::memcpy_s(inject_to, size, new_code, size);
    }

    void TH9ver1_5aMonitor::Attach(void)
    {
        InjectOnFrameUpdate();
    }

    void TH9ver1_5aMonitor::InjectOnFrameUpdate(void)
    {
        /**
        .text:00420290                 retn
        を
        .text:00420290                 call OnFrameUpdate ;; return 1
        .text:00420295                 retn
        に書き換える
        */
        //TODO: アドレスをどっかにまとめたい
        char* inject_to = reinterpret_cast<char*>(0x420290);
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnFrameUpdate
            0xC3              // retn
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnFrameUpdateVer1_5);
        WriteCode(inject_to, code, sizeof(code));
    }
    
    void TH9ver1_5aMonitor::OnFrameUpdate(void)
    {
        //TODO: とりあえずテストとしてキー入力に介入。後で正式なコードに変える。
        static int frame = 0;
        unsigned short* keystate_1P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C);
        unsigned short* keystate_2P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C+0x8E);
        unsigned short* keystate_3P = reinterpret_cast<unsigned short*>(0x4ACE18+0x2C+0x8E*2);
        *keystate_1P |= frame%60 < 30 ? 0x40 : 0x80;
        frame++;
    }
}