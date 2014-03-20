#pragma once
#include "TH9Monitor.h"

namespace ka_ai_duka{
    class AIManager : public IObserver
    {
    private:
        unsigned int frame;
    public:
        AIManager(void);
        virtual ~AIManager(void);
        void OnGameStart(TH9Monitor &monitor);
        void OnFrameUpdate(TH9Monitor &monitor);
        void OnGameEnd(TH9Monitor &monitor);
    };
}

