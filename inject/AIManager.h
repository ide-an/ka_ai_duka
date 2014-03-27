#pragma once
#include "TH9Monitor.h"
#include <lua.hpp>
#include <string>

namespace ka_ai_duka{
    class AIManager : public IObserver
    {
    private:
        unsigned int frame;
        std::string filenames[2];
        ::lua_State* lua_states[2];
        bool ShouldRunAI(int player_side)//1P: 0, 2P: 1
        {
            return !filenames[player_side].empty();
        }
    public:
        AIManager(void);
        virtual ~AIManager(void);
        void OnGameStart(TH9Monitor &monitor);
        void OnFrameUpdate(TH9Monitor &monitor);
        void OnGameEnd(TH9Monitor &monitor);
        void SetFilename1P(const std::string &filename)
        {
            filenames[0] = filename;
        }
        void SetFilename2P(const std::string &filename)
        {
            filenames[1] = filename;
        }
    };
}

