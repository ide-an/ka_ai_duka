#include "TH9Monitor.h"
#include <Windows.h>
#include "th09address.h"

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
        if(is_playing && observer){
            game_sides[0]->Update();
            game_sides[1]->Update();
            observer->OnFrameUpdate(*this);
        }
    }

    void TH9Monitor::OnGameStart(void)
    {
        ::MessageBeep(MB_ICONASTERISK);
        if(!IsNetBattle()){
            is_playing = true;
        }
        if(is_playing && observer){
            game_sides[0] = new managed_types::GameSide(board[0]);
            game_sides[1] = new managed_types::GameSide(board[1]);
            observer->OnGameStart(*this);
        }
        //char s[0xff];
        //::sprintf(s,"%X %X", &address::globals_ver1_5->hwnd, &hwnd);
        //::MessageBoxA(NULL, s, "D3DDeivce",MB_OK);
    }

    void TH9Monitor::OnGameEnd(void)
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        if(is_playing && observer){
            observer->OnGameEnd(*this);
            delete game_sides[0];
            delete game_sides[1];
        }
        is_playing = false;
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
