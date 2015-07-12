#include "TH9Monitor.h"
#include "th09address.h"

namespace ka_ai_duka{
    TH9ver1_0Monitor::TH9ver1_0Monitor(void) :
        TH9Monitor(
            address::globals_ver1_0->board,
            address::globals_ver1_0->key_states,
            address::globals_ver1_0->ex_attack_container,
            address::globals_ver1_0->round,
            address::globals_ver1_0->round_win,
            address::globals_ver1_0->difficulty,
            address::globals_ver1_0->play_status,
            address::globals_ver1_0->charge_types,
            address::globals_ver1_0->hwnd,
            address::ex_attack_func_addr_ver1_0
            )
    {
    }

    int __declspec(naked) OnFrameUpdateVer1_0(void)
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

    void __declspec(naked) OnGameStartVer1_0(void)
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

    void __declspec(naked) OnGameEndVer1_0(void)
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

    void TH9ver1_0Monitor::Attach(void)
    {
        InjectOnFrameUpdate();
        InjectOnReplayUpdate();
        InjectOnGameStart();
        InjectOnGameEnd();
    }

    void TH9ver1_0Monitor::InjectOnFrameUpdate(void)
    {
        /**
          retn
        Ç
          call OnFrameUpdate ;; return 1
          retn
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_frame_update.ver1_0;
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnFrameUpdate
            0xC3              // retn
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnFrameUpdateVer1_0);
        WriteCode(inject_to, code, sizeof(code));
    }

    void TH9ver1_0Monitor::InjectOnReplayUpdate(void)
    {
        /**
          retn
        Ç
          call OnFrameUpdate ;; return 1
          retn
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_replay_update.ver1_0;
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnFrameUpdate
            0xC3              // retn
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnFrameUpdateVer1_0);
        WriteCode(inject_to, code, sizeof(code));
    }

    void TH9ver1_0Monitor::InjectOnGameStart(void)
    {
        /**
          mov     edx, 320h
        Ç
          call OnGameStart
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_game_start.ver1_0;
        char code[] = {
            0xE8, 0, 0, 0, 0, // call OnGameStart
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnGameStartVer1_0);
        WriteCode(inject_to, code, sizeof(code));
    }

    void TH9ver1_0Monitor::InjectOnGameEnd(void)
    {
        /**      
          add     ebx, 0E8h
        Ç
          call OnGameEnd
          nop
        Ç…èëÇ´ä∑Ç¶ÇÈ
        */
        char* inject_to = address::addr_on_game_end.ver1_0;
        char code[] = {
            0xE8, 0, 0, 0, 0, //call OnGameEnd
            0x90              //nop
        };
        SetJumpTo(code + 1, (int)(inject_to + 5), (int)OnGameEndVer1_0);
        WriteCode(inject_to, code, sizeof(code));
    }
}
