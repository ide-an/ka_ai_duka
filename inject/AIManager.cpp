#include "AIManager.h"
#include "bind.h"
#include <sstream>
#include <cstdio>
#include <Windows.h>
#include "sandbox.h"
#include "../common/errorhandle.h"
namespace ka_ai_duka{
    AIManager::AIManager(void)
    {
        lua_states[0] = nullptr;
        lua_states[1] = nullptr;
    }

    AIManager::~AIManager(void)
    {
    }

    static const char* Side2Str(PlayerSide p)
    {
        return (p == Side_1P) ? "1P" : "2P";
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
                std::string basename;
                std::string script_dir;
                FilenameToDirAndBasename(filenames[i], script_dir, basename);
                ::lua_State* ls = ::luaL_newstate();
                ::luaL_openlibs(ls);
                PlayerSide side = i == 0 ? Side_1P : Side_2P;
                BindToLua(ls, side);
                ExportVariables(ls, monitor, side, script_dir.c_str());
                if(!sandbox::Initialize(ls)){
                    std::ostringstream os;
                    os << "Sandbox�̏������Ɏ��s���܂����B(" <<  Side2Str(side) << "��)" << std::endl;
                    os << ::lua_tostring(ls, -1);
                    ReportError(os);
                    ::lua_close(ls);
                }else if(!sandbox::DoFile(ls, basename)){
                    std::ostringstream os;
                    os << "�X�N���v�g�̓ǂݍ��݂Ɏ��s���܂����B(" <<  Side2Str(side) << "��)" << std::endl;
                    os << ::lua_tostring(ls, -1);
                    ReportError(os);
                    //�܂����s����̖����������߂��
                    ::lua_close(ls);
                }else{
                    lua_states[i] = ls;
                }
            }
        }
    }

    void AIManager::OnFrameUpdate(TH9Monitor &monitor)
    {
        //bool should_reset = false;
        for(int i=0;i<2;i++){
            if(lua_states[i]){
                UpdateVariables(lua_states[i], monitor);
                //if(should_reset){
                //    monitor.GetGameSide(Side_1P)->Reset();
                //    monitor.GetGameSide(Side_2P)->Reset();
                //}
                if(!sandbox::CallMain(lua_states[i])){
                    std::ostringstream os;
                    os << "�X�N���v�g�̎��s�G���[�B(" <<  Side2Str(i==0 ? Side_1P : Side_2P) << "��)" << std::endl;
                    os << ::lua_tostring(lua_states[i], -1);
                    ReportError(os);
                    //���s����߂邺�I
                    ::lua_close(lua_states[i]);
                    lua_states[i] = nullptr;
                }
                //should_reset = true;
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
