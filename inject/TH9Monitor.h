#pragma once
#include <cassert>
#include "th09types.h"

namespace ka_ai_duka{
    enum TH9Version{
        Ver1_0,
        Ver1_5a,
    };

    class TH9Monitor
    {
    private:
        raw_types::Board (&board)[2];
        raw_types::KeyState (&key_states)[3];
    public:
        TH9Monitor(
            raw_types::Board (&board)[2],
            raw_types::KeyState (&key_states)[3]
        ) : board(board), key_states(key_states)
        {};
        virtual ~TH9Monitor(void){};
        virtual void Attach(void) = 0;
        virtual void Detach(void) = 0;
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
