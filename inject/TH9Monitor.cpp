#include "TH9Monitor.h"
#include "th09address.h"
#include <Windows.h>
#include <cstdio>

namespace ka_ai_duka{
    TH9Monitor* monitor = nullptr;

    TH9ver1_5aMonitor::TH9ver1_5aMonitor(void) : 
        TH9Monitor(
            raw_address::globals_ver1_5->board,
            raw_address::globals_ver1_5->key_states
            ),
            net_info(raw_address::globals_ver1_5->net_info)
    {
    }
    TH9ver1_0Monitor::TH9ver1_0Monitor(void) :
        TH9Monitor(
            raw_address::globals_ver1_0->board,
            raw_address::globals_ver1_0->key_states
            )
    {
    }
    
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

    void __declspec(naked) OnGameStartVer1_5(void)
    {
        __asm{
            pushad;
            pushfd;
        }
        if(monitor){
            monitor->OnGameStart();
        }
        __asm{
            popfd;
            popad;
            mov edx, 320h;
            ret;
        }
    }

    void __declspec(naked) OnGameEndVer1_5(void)
    {
        __asm{
            pushad;
            pushfd;
        }
        if(monitor){
            monitor->OnGameEnd();
        }
        __asm{
            popfd;
            popad;
            add ebx, 0E8h;
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
        //TODO: 排他制御すべきか?
        InjectOnFrameUpdate();
        InjectOnGameStart();
        InjectOnGameEnd();
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
        char* inject_to = raw_address::addr_on_frame_update.ver1_5;
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnFrameUpdate
            0xC3              // retn
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnFrameUpdateVer1_5);
        WriteCode(inject_to, code, sizeof(code));
    }

    void TH9ver1_5aMonitor::InjectOnGameStart(void)
    {
        /**
        .text:0041B2C3                 mov     edx, 320h
        を
        .text:0041B2C3                 call OnGameStart
        に書き換える
        */
        char* inject_to = raw_address::addr_on_game_start.ver1_5;
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnGameStart
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnGameStartVer1_5);
        WriteCode(inject_to, code, sizeof(code));
    }

    void TH9ver1_5aMonitor::InjectOnGameEnd(void)
    {
        /**      
        .text:0041B9A2                 add     ebx, 0E8h
        を
        .text:0041B9A2                 call OnGameEnd
        .text:0041B9A7                 nop
        に書き換える
        */
        char* inject_to = raw_address::addr_on_game_end.ver1_5;
        char code[] = {
            0xE8, 0, 0, 0, 0, //call OnGameEnd
            0x90              //nop
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnGameEndVer1_5);
        WriteCode(inject_to, code, sizeof(code));
    }
/**
ここから先はバージョン依存しない部分
*/
    void TH9Monitor::OnFrameUpdate(void)
    {
        static int frame = 0;//TODO: デバッグコード。削除する。
        key_states[0].keys |= frame%60 < 30 ? 0x40 : 0x80;
        frame++;
    }

    void TH9Monitor::OnGameStart(void)
    {
        if(IsNetBattle()){
            ::MessageBeep(MB_ICONASTERISK);
        }
    }

    void TH9Monitor::OnGameEnd(void)
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
    }
}