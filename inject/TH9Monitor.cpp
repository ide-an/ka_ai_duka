#include "TH9Monitor.h"
#include <Windows.h>
#include "th09address.h"
#include <cstdio>
#include <sstream>
#include <string.h>

namespace ka_ai_duka{
    TH9Monitor* monitor = nullptr;

    TH9Version TH9Monitor::CheckVersion(void)
    {
        char* title_ver_1_0 = "東方花映塚　～ Phantasmagoria of Flower View. ver 1.00";
        char* title_ver_1_5 = "東方花映塚　～ Phantasmagoria of Flower View. ver 1.50a";
        if(::memcmp(address::addr_window_title.ver1_0, title_ver_1_0, ::strlen(title_ver_1_0)) == 0){
            return Ver1_0;
        }
        if(::memcmp(address::addr_window_title.ver1_5, title_ver_1_5, ::strlen(title_ver_1_5)) == 0){
            return Ver1_5a;
        }
        return Unknown_Version;
    }
    
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
        static bool prev_p = false;
        if(is_playing && observer){
            game_sides[0]->Update(idgen);
            game_sides[1]->Update(idgen);
            observer->OnFrameUpdate(*this);
            //if(key_states[2].keys & keys::p){
            //    OnSnapshotSave();
            //}
            prev_p = (key_states[2].keys & keys::p);
        }
    }

    void TH9Monitor::OnGameStart(void)
    {
        if (!IsNetBattle() && (!IsReplay() || should_run_ai_while_replay)){
            is_playing = true;
        }
        if(is_playing && observer){
            idgen.Reset();
            game_sides[0] = new managed_types::GameSide(board[0], round_win[0], charge_types[0], ex_attack_func_addr, ex_attack_container, Side_1P);
            game_sides[1] = new managed_types::GameSide(board[1], round_win[1], charge_types[1], ex_attack_func_addr, ex_attack_container, Side_2P);
            observer->OnGameStart(*this);
        }
    }

    void TH9Monitor::OnGameEnd(void)
    {
        if(is_playing && observer){
            observer->OnGameEnd(*this);
            delete game_sides[0];
            delete game_sides[1];
        }
        is_playing = false;
    }

    void TH9Monitor::OnSnapshotSave(void)
    {
        static int count = 0;
        FILE* fp;
        char filename[0xff];
        count++;
        ::sprintf(filename, "C:/Users/ide/Desktop/hoge-snapshot-%d.txt", count);
        fp = fopen(filename, "wt");
        fprintf(fp, "bullet()\n");
        std::stringstream ss;
        for(auto it=game_sides[0]->Bullets().begin();it!=game_sides[0]->Bullets().end();++it){
            ss.str("");
            ss << *((*it)->HittableObject());
            fprintf(fp, "pos(%f,%f);v(%f,%f);body(%s);\n", (*it)->X(),(*it)->Y(),(*it)->Vx(),(*it)->Vy(), ss.str().c_str());
        }
        fprintf(fp, "enemy()\n");
        for(auto it=game_sides[0]->Enemies().begin();it!=game_sides[0]->Enemies().end();++it){
            ss.str("");
            ss << *((*it)->HittableObject());
            fprintf(fp, "pos(%f,%f);v(%f,%f);body(%s);\n", (*it)->X(),(*it)->Y(),(*it)->Vx(),(*it)->Vy(), ss.str().c_str());
        }
        fclose(fp);
    }

    void TH9Monitor::SetKeyState(PlayerSide side, KeyState key_state)
    {
        if (!IsReplay()){
            key_states[side].keys |= key_state;
            KeyState changed_keys = key_states[side].keys ^ key_states[side].prev_keys;
            key_states[side].start_pushing_keys = changed_keys & key_states[side].keys;
            key_states[side].start_leaving_keys = changed_keys & ~key_states[side].keys;
        }
    }

    void TH9Monitor::SaveSnapshot()
    {
        //スナップショット取得については通常のキーとは別の領域をチェックしているのでそっちを書き換える
        //.text:0042D3DA                 mov     ecx, offset key_states_3P
        //.text:0042D3DF                 call    andEcx_6
        //.text:0042D3E4                 test    ax, ax; check if P is pushed
        //.text:0042D3E7                 jz      short loc_42D44C
        key_states[2].system_keys |= keys::p;
    }

    void TH9Monitor::SetWindowTitle(const char* text)
    {
        ::SetWindowTextA((HWND)hwnd, text);
    }
}
