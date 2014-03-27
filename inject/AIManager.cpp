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
    FILE* fp;
    int count = 0;
    void AIManager::OnGameStart(TH9Monitor &monitor)
    {
        //TODO: debug code
        char filename[0xff];
        frame = 0;
        count++;
        ::sprintf(filename, "C:/Users/ide/Desktop/hoge-%d.txt", count);
        fp = fopen(filename, "wt");
        // init lua
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
        static KeyState keystate = 0;//keys::right | keys::up;
        auto game_side_1p = monitor.GetGameSide(Side_1P);
        auto game_side_2p = monitor.GetGameSide(Side_2P);
        //auto player = game_side_1p->Player();
        //if(player->X() < -130.0 && (keystate & keys::left)){
        //    keystate &= ~keys::left;
        //    keystate |= keys::right;
        //}
        //if(player->X() > 130.0 && (keystate & keys::right)){
        //    keystate &= ~keys::right;
        //    keystate |= keys::left;   
        //}
        //if(player->Y() < 20.0 && (keystate & keys::up)){
        //    keystate &= ~keys::up;
        //    keystate |= keys::down;
        //}
        //if(player->Y() > 380.0 && (keystate & keys::down)){
        //    keystate &= ~keys::down;
        //    keystate |= keys::up;
        //}
        //TODO: debug only
        char text[0xff];
        ::sprintf(text, "%d : %d" , game_side_1p->Bullets().size(), game_side_2p->Bullets().size());
        monitor.SetWindowTitle(text);
        //fprintf(fp, "bullet: %d : %d\n", 
        //    game_side_1p->Bullets().size(),
        //    game_side_2p->Bullets().size()
        //    );
        
        monitor.SetKeyState(Side_1P, keystate);
        
        frame++;
        for(int i=0;i<2;i++){
            if(lua_states[i]){
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
        //TODO: debug code
        fclose(fp);
        //finite lua
        for(int i=0;i<2;i++){
            if(lua_states[i]){
                ::lua_close(lua_states[i]);
                lua_states[i] = nullptr;
            }
        }
    }
}
