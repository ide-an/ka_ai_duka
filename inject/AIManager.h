#pragma once
#include "TH9Monitor.h"
#include <lua.hpp>
#include <luabind\luabind.hpp>
#include <string>

namespace ka_ai_duka{
    class AIManager : public IObserver
    {
    private:
        unsigned int frame;
        std::string* filenames[2];//TODO: nullptrÇ≈èâä˙âª
        ::lua_State* lua_states[2];
        bool ShouldRunAI(int player_side)//1P: 0, 2P: 1
        {
            return filenames[player_side] != nullptr;
        }
    public:
        AIManager(void);
        virtual ~AIManager(void);
        void OnGameStart(TH9Monitor &monitor);
        void OnFrameUpdate(TH9Monitor &monitor);
        void OnGameEnd(TH9Monitor &monitor);
        void SetFilename1P(std::string *filename)
        {
            if(filename){
                filenames[0] = filename;
            }else{
                filenames[0] = nullptr;
            }
        }
        void SetFilename2P(std::string *filename)
        {
            if(filename){
                filenames[1] = filename;
            }else{
                filenames[1] = nullptr;
            }
        }
    };
}

