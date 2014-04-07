#include "AIManager.h"
#include "bind.h"
#include <sstream>
#include <cstdio>
#include <Windows.h>
#include "sandbox.h"
namespace ka_ai_duka{
    AIManager::AIManager(void)
    {
        lua_states[0] = nullptr;
        lua_states[1] = nullptr;
    }

    AIManager::~AIManager(void)
    {
    }

    void FilenameToDirAndBasename(std::string &fullpath, std::string &dir, std::string &basename)
    {
        int len = fullpath.size();
        char* cdrive = new char[len];
        char* cdir = new char[len];
        char* cfile = new char[len];
        char* cext = new char[len];
        ::_splitpath_s(fullpath.c_str(), cdrive, len, cdir, len, cfile, len, cext, len);
        std::ostringstream os;
        os << cdrive << cdir;
        dir = os.str();
        os.str("");
        os << cfile << cext;
        basename = os.str();
        delete[] cext;
        delete[] cfile;
        delete[] cdir;
        delete[] cdrive;
    }

    void AIManager::OnGameStart(TH9Monitor &monitor)
    {
        for(int i=0;i<2;i++){
            if(ShouldRunAI(i, monitor)){
                ::MessageBoxA(NULL, filenames[i].c_str(), "hoge", MB_OK);
                std::string basename;
                std::string script_dir;
                FilenameToDirAndBasename(filenames[i], script_dir, basename);
                ::lua_State* ls = ::luaL_newstate();
                ::luaL_openlibs(ls);
                BindToLua(ls, i == 0 ? Side_1P : Side_2P);
                ExportVariables(ls, monitor, i == 0 ? Side_1P : Side_2P, script_dir.c_str());
                if(!sandbox::Initialize(ls)){
                    //TODO: もう少しましなエラー表示
                    char s[0xff];::sprintf(s,"sandbox initialize error %s", filenames[i].c_str());
                    ::MessageBoxA(NULL, s, "error", MB_OK);
                    const char* ss = ::lua_tostring(ls, -1);
                    ::MessageBoxA(NULL, ss, "erro", MB_OK);
                    //まぁ実行するの無理だし諦めるね
                    ::lua_close(ls);
                }else if(!sandbox::DoFile(ls, basename)){
                    //TODO: もう少しましなエラー表示
                    char s[0xff];::sprintf(s,"dofile error %s", filenames[i].c_str());
                    ::MessageBoxA(NULL, s, "error", MB_OK);
                    const char* ss = ::lua_tostring(ls, -1);
                    ::MessageBoxA(NULL, ss, "erro", MB_OK);
                    //まぁ実行するの無理だし諦めるね
                    ::lua_close(ls);
                }else{
                    lua_states[i] = ls;
                }
            }
        }
    }

    void AIManager::OnFrameUpdate(TH9Monitor &monitor)
    {
        for(int i=0;i<2;i++){
            if(lua_states[i]){
                UpdateVariables(lua_states[i], monitor);
                if(!sandbox::CallMain(lua_states[i])){
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
