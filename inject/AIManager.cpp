#include "AIManager.h"
#include "bind.h"
#include <cstdio>
#include <Windows.h>
namespace ka_ai_duka{
    AIManager::AIManager(void)
    {
        lua_states[0] = nullptr;
        lua_states[1] = nullptr;
    }

    AIManager::~AIManager(void)
    {
    }

    void AIManager::OnGameStart(TH9Monitor &monitor)
    {
        for(int i=0;i<2;i++){
            if(ShouldRunAI(i)){
                ::lua_State* ls = ::luaL_newstate();
                ::luaL_openlibs(ls);//TODO: limit available libs
                BindToLua(ls, i == 0 ? Side_1P : Side_2P);
                ExportVariables(ls, monitor, i == 0 ? Side_1P : Side_2P);
                int res = luaL_dofile(ls, filenames[i].c_str());
                if(!res){
                    lua_states[i] = ls;
                }else{
                    //TODO: もう少しましなエラー表示
                    char s[0xff];::sprintf(s,"dofile error %s : %d", filenames[i].c_str(), res);
                    ::MessageBoxA(NULL, s, "error", MB_OK);
                    const char* ss = ::lua_tostring(ls, -1);
                    ::MessageBoxA(NULL, ss, "erro", MB_OK);
                    //まぁ実行するの無理だし諦めるね
                    ::lua_close(ls);
                }
            }
        }
    }

    void AIManager::OnFrameUpdate(TH9Monitor &monitor)
    {
        for(int i=0;i<2;i++){
            if(lua_states[i]){
                UpdateVariables(lua_states[i], monitor);
                lua_getglobal(lua_states[i], "main");
                if(lua_pcall(lua_states[i], 0, 0, 0)){
                    //TODO: error handling
                    ::MessageBoxA(NULL, "call main error", "error", MB_OK);
                    const char* ss = ::lua_tostring(lua_states[i], -1);
                    ::MessageBoxA(NULL, ss, "error", MB_OK);
                    //実行を諦めるぜ！
                    ::lua_close(lua_states[i]);
                    lua_states[i] = nullptr;
                }
            }
        }
    }

    void AIManager::OnGameEnd(TH9Monitor &monitor)
    {
        for(int i=0;i<2;i++){
            if(lua_states[i]){
                ::lua_close(lua_states[i]);
                lua_states[i] = nullptr;
            }
        }
    }
}
