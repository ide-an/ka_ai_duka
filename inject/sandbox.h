#pragma once
#include <lua.hpp>
#include <string>

namespace ka_ai_duka{
    namespace sandbox{
        extern const char* env_name;
        extern const char* varname_script_dir;
        bool Initialize(::lua_State* ls);
        bool DoFile(::lua_State* ls, std::string &basename);
        bool CallMain(::lua_State* ls);
    }
}