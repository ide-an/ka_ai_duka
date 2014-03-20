#pragma once
#include <cassert>
#include "th09types.h"
#ifdef DEBUG_DRAW
#include "DrawUtil.h"
#endif

namespace ka_ai_duka{
    enum TH9Version{
        Ver1_0,
        Ver1_5a,
    };

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
        IObserver* observer;
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
            unsigned int &difficulty
            ) : board(board), key_states(key_states), ex_attack_container(ex_attack_container),
            round(round), round_win(round_win), difficulty(difficulty)
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
        static TH9Version CheckVersion(void){
            //TODO: check if version is 1.0
            return Ver1_5a;
        }
        virtual bool IsNetBattle(void){
            return false;
        }
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
        void Attach(void){
            assert(0);//TODO: implement
        }
        void Detach(void){
            assert(0);//TODO: implement
        }
    };

    extern TH9Monitor* monitor;
}
