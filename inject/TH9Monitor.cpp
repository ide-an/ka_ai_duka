#include "TH9Monitor.h"
#include <Windows.h>
#include "th09address.h"
#include <cstdio>
#include <sstream>

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
        static bool prev_p = false;
        if(is_playing && observer){
            game_sides[0]->Update(idgen);
            game_sides[1]->Update(idgen);
            observer->OnFrameUpdate(*this);
            if(key_states[2].keys & keys::p){
                OnSnapshotSave();
            }
            prev_p = (key_states[2].keys & keys::p);
        }
    }

    void TH9Monitor::OnGameStart(void)
    {
        if(!IsNetBattle()){
            is_playing = true;
        }
        if(is_playing && observer){
            idgen.Reset();
            game_sides[0] = new managed_types::GameSide(board[0], round_win[0], ex_attack_func_addr, ex_attack_container, Side_1P);
            game_sides[1] = new managed_types::GameSide(board[1], round_win[1], ex_attack_func_addr, ex_attack_container, Side_2P);
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
        key_states[side].keys |= key_state;
    }

    void TH9Monitor::SetWindowTitle(const char* text)
    {
        ::SetWindowTextA((HWND)hwnd, text);
    }
}
