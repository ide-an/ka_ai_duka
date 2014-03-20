#include "TH9Monitor.h"
#include "th09address.h"

namespace ka_ai_duka{
    TH9ver1_5aMonitor::TH9ver1_5aMonitor(void) : TH9Monitor(
        address::globals_ver1_5->board,
        address::globals_ver1_5->key_states,
        address::globals_ver1_5->ex_attack_container,
        address::globals_ver1_5->round,
        address::globals_ver1_5->round_win,
        address::globals_ver1_5->difficulty
        ),
        net_info(address::globals_ver1_5->net_info)
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

    void TH9ver1_5aMonitor::Attach(void)
    {
        InjectOnFrameUpdate();
        InjectOnGameStart();
        InjectOnGameEnd();
    }

    void TH9ver1_5aMonitor::InjectOnFrameUpdate(void)
    {
        /**
        .text:00420290                 retn
        Ç
        .text:00420290                 call OnFrameUpdate ;; return 1
        .text:00420295                 retn
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_frame_update.ver1_5;
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
        Ç
        .text:0041B2C3                 call OnGameStart
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_game_start.ver1_5;
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
        Ç
        .text:0041B9A2                 call OnGameEnd
        .text:0041B9A7                 nop
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_game_end.ver1_5;
        char code[] = {
            0xE8, 0, 0, 0, 0, //call OnGameEnd
            0x90              //nop
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnGameEndVer1_5);
        WriteCode(inject_to, code, sizeof(code));
    }
}