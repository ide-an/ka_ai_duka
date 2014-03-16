#pragma once
#include <cassert>

namespace ka_ai_duka{
    enum TH9Version{
        Ver1_0,
        Ver1_5a,
    };

    class TH9Monitor
    {
    public:
        TH9Monitor(void){};
        virtual ~TH9Monitor(void){};
        virtual void Attach(void) = 0;
        virtual void Detach(void) = 0;
        virtual void OnFrameUpdate(void) = 0;
        virtual void OnGameStart(void) = 0;
        virtual void OnGameEnd(void) = 0;
        static TH9Version CheckVersion(void){
            //TODO: check if version is 1.0
            return Ver1_5a;
        }
    };

    class TH9ver1_5aMonitor : public TH9Monitor
    {
    public:
        void Attach(void);
        void Detach(void){
        }
        void OnFrameUpdate(void);
        void OnGameStart(void);
        void OnGameEnd(void);
    private:
        void InjectOnFrameUpdate(void);
        void InjectOnGameStart(void);
        void InjectOnGameEnd(void);
        bool IsNetBattle(void){
            //TODO: implement
            return false;
        }
    };

    class TH09ver1_0Monitor : public TH9Monitor
    {
    public:
        void Attach(void){
            assert(0);//TODO: implement
        }
        void Detach(void){
            assert(0);//TODO: implement
        }
        void OnFrameUpdate(void){
            assert(0);//TODO: implement
        };
        void OnGameStart(void){
            assert(0);//TODO: implement
        };
        void OnGameEnd(void){
            assert(0);//TODO: implement
        }
    };

    extern TH9Monitor* monitor;
}
