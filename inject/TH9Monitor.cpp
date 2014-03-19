#include "TH9Monitor.h"
#include "th09address.h"
#include <Windows.h>
#include <cstdio>

namespace ka_ai_duka{
    TH9Monitor* monitor = nullptr;

    TH9ver1_5aMonitor::TH9ver1_5aMonitor(void) : 
        TH9Monitor(
            address::globals_ver1_5->board,
            address::globals_ver1_5->key_states,
            address::globals_ver1_5->ex_attack_container,
            address::globals_ver1_5->round,
            address::globals_ver1_5->round_win,
            address::globals_ver1_5->difficulty,
            address::globals_ver1_5->d3d8_device
            ),
            net_info(address::globals_ver1_5->net_info)
    {
    }
    TH9ver1_0Monitor::TH9ver1_0Monitor(void) :
        TH9Monitor(
            address::globals_ver1_0->board,
            address::globals_ver1_0->key_states,
            address::globals_ver1_0->ex_attack_container,
            address::globals_ver1_0->round,
            address::globals_ver1_0->round_win,
            address::globals_ver1_0->difficulty,
            address::globals_ver1_0->d3d8_device
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
        を
        .text:0041B2C3                 call OnGameStart
        に書き換える
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
        を
        .text:0041B9A2                 call OnGameEnd
        .text:0041B9A7                 nop
        に書き換える
        */
        char* inject_to = address::addr_on_game_end.ver1_5;
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
    //FILE* fp;
    void TH9Monitor::OnFrameUpdate(void)
    {
        //static int frame = 0;//TODO: デバッグコード。削除する。
        //key_states[0].keys |= frame%60 < 30 ? 0x40 : 0x80;
        //frame++;
        //デバッグ描画。DirectXを叩いて座標表示
        if(draw_util){
            struct raw_types::Vector3D &player_pos = board[0].player->position;
            int is_2P = 0;
            float d = 5.0f;
            float offset_x = 144.0 + (is_2P ? 336.0 : 16.0);
            float offset_y = 16.0;
            char s[0xff];
            ::sprintf(s, "x:%f y:%f\n", player_pos.x, player_pos.y);
            ::OutputDebugStringA(s);
            //::fprintf(fp, s);
            draw_util->BeginScene();
            draw_util->DrawRect(
                    player_pos.x + offset_x - d,
                    player_pos.y + offset_y - d,
                    player_pos.x + offset_x + d,
                    player_pos.y + offset_y + d,
                    draw::DrawUtil::D3DColorARGB(0xff,0,0,0xff));
            draw_util->EndScene();
        }
    }

    void TH9Monitor::OnGameStart(void)
    {
        if(IsNetBattle()){
            ::MessageBeep(MB_ICONASTERISK);
        }
        char s[0xff];
        ::sprintf(s,"%X", address::globals_ver1_5->d3d8_device);
        ::MessageBoxA(NULL, s, "D3DDeivce",MB_OK);
        ::sprintf(s,"%X", *address::globals_ver1_5->d3d8_device);
        ::MessageBoxA(NULL, s, "*D3DDeivce",MB_OK);
        draw_util = new draw::DrawUtil(d3d_device);
        ::sprintf(s,"%X", draw_util->begin_scene);
        ::MessageBoxA(NULL, s, "BeginScene",MB_OK);
     //   fp = ::fopen("C:/users/ide/desktop/hoge.txt","wt");
    }

    void TH9Monitor::OnGameEnd(void)
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        if(draw_util){
            delete draw_util;
            draw_util = nullptr;
        }
        //if(fp){
        //    fclose(fp);
        //    fp = nullptr;
        //}
    }
}
