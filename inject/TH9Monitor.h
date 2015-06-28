#pragma once
#include <cassert>
#include "th09types.h"
#include "player_side.h"
#include "GameSide.h"
#include "idgen.h"
#ifdef DEBUG_DRAW
#include "DrawUtil.h"
#endif

namespace ka_ai_duka{
    enum TH9Version{
        Ver1_0,
        Ver1_5a,
        Unknown_Version
    };

    typedef unsigned short KeyState;
    namespace keys{
        const KeyState r      = 1 << 14;
        const KeyState d      = 1 << 13;
        const KeyState enter  = 1 << 12;
        const KeyState p      = 1 << 11;
        const KeyState s      = 1 << 10;
        const KeyState q      = 1 << 9;
        const KeyState ctrl   = 1 << 8;
        const KeyState right  = 1 << 7;
        const KeyState left   = 1 << 6;
        const KeyState down   = 1 << 5;
        const KeyState up     = 1 << 4;
        const KeyState esc    = 1 << 3;
        const KeyState shift  = 1 << 2;
        const KeyState x      = 1 << 1;
        const KeyState z      = 1 << 0;
    }

    class TH9Monitor;

    class IObserver
    {
    public:
        virtual void OnGameStart(TH9Monitor &monitor) = 0;
        virtual void OnFrameUpdate(TH9Monitor &monitor) = 0;
        virtual void OnGameEnd(TH9Monitor &monitor) = 0;
    };

    class TH9Monitor
    {
    private:
        raw_types::Board (&board)[2];
        raw_types::KeyState (&key_states)[3];
        raw_types::ExAttackContainer* &ex_attack_container;
        unsigned int &round;
        unsigned int (&round_win)[2];
        unsigned int &difficulty;
        unsigned int &play_status;
        raw_types::ExAttackFuncAddr &ex_attack_func_addr;
        IObserver* observer;
        bool is_playing;// 0x1000: cut-in, 0x8: replay
        managed_types::GameSide* game_sides[2];
        IDGenerator idgen;
        char (&charge_types)[2];
        int &hwnd;//for debugging
        bool can_save_snapshot;
        void OnSnapshotSave(void);
    protected:
        void SetJumpTo(char* code, int from, int to);
        void WriteCode(char* inject_to, char* new_code, size_t size);
    public:
        TH9Monitor(
            raw_types::Board (&board)[2],
            raw_types::KeyState (&key_states)[3],
            raw_types::ExAttackContainer* &ex_attack_container,
            unsigned int &round,
            unsigned int (&round_win)[2],
            unsigned int &difficulty,
            unsigned int &play_status,
            char (&charge_types)[2],
            int &hwnd,
            raw_types::ExAttackFuncAddr &ex_attack_func_addr
            ) : board(board), key_states(key_states), ex_attack_container(ex_attack_container),
            round(round), round_win(round_win), difficulty(difficulty), is_playing(false), play_status(play_status), hwnd(hwnd),
            ex_attack_func_addr(ex_attack_func_addr), charge_types(charge_types), can_save_snapshot(false)
        {};
        virtual ~TH9Monitor(void){};
        virtual void Attach(void) = 0;
        virtual void Detach(void) = 0;
        void SetObserver(IObserver *observer)
        {
            this->observer = observer;
        }
        void OnFrameUpdate(void);
        void OnGameStart(void);
        void OnGameEnd(void);
        static TH9Version CheckVersion(void);
        virtual bool IsNetBattle(void){
            return false;
        }
        virtual bool IsReplay(void){
            return (play_status & 0x8) != 0;
        }
        void SetKeyState(PlayerSide side, KeyState key_state);
        void SaveSnapshot(void); 
        bool CanSaveSnapshot(void) const
        {
            return can_save_snapshot;
        }
        void SetCanSaveSnapshot(bool b)
        {
            can_save_snapshot = b;
        }
        managed_types::GameSide* GetGameSide(PlayerSide side)
        {
            return game_sides[side];
        }
        unsigned int GetRound(void)
        {
            return round;
        }
        unsigned int GetDifficulty(void)
        {
            return difficulty;
        }
        void SetWindowTitle(const char* text);
    };

    class TH9ver1_5aMonitor : public TH9Monitor
    {
    private:
        raw_types::NetInfo* &net_info;
    public:
        TH9ver1_5aMonitor(void);
        void Attach(void);
        void Detach(void){
        }
        bool IsNetBattle(void){
            return net_info && net_info->host_address;
        }
    private:
        void InjectOnFrameUpdate(void);
        void InjectOnGameStart(void);
        void InjectOnGameEnd(void);
    };

    class TH9ver1_0Monitor : public TH9Monitor
    {
    public:
        TH9ver1_0Monitor(void);
        void Attach(void);
        void Detach(void){
        }
    private:
        void InjectOnFrameUpdate(void);
        void InjectOnGameStart(void);
        void InjectOnGameEnd(void);
        void InjectOnReplayUpdate(void);
    };

    extern TH9Monitor* monitor;
}
