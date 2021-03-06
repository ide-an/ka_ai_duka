#include "sandbox.h"
namespace ka_ai_duka{
    namespace sandbox{
        const char* env_name = "env";
        const char* varname_script_dir = "script_dir";
        bool Initialize(::lua_State* ls)
        {
            const char * src = 
                "env = {};\n"
                "local function validate_path(filename)\n"
                "  if type(filename) ~= 'string' then\n"
                "    return false\n"
                "  end\n"
                "  return string.match(filename, '^[%w_%-]+%.%w+$') ~= nil;\n"
                "end\n"
                "local function absolute_path(filename)\n"
                "  return script_dir..filename;\n"
                "end\n"
                "local function loadfile_safe(filename)\n"
                "  if not validate_path(filename) then\n"
                "    return nil, 'invalid filename';\n"
                "  end\n"
                "  filename = absolute_path(filename);\n"
                "  local fp = io.open(filename, 'rt');\n"
                "  local magic = fp:read(4);\n"
                "  if magic:byte(1) == 0x1B then\n"
                "    return nil, 'binary code prohibited';\n"
                "  end\n"
                "  local f, err = loadfile(filename);\n"
                "  if not f then\n"
                "    return nil, err;\n"
                "  end\n"
                "  setfenv(f, env);\n"
                "  return f;\n"
                "end\n"
                "\n"
                "local function dofile_safe(filename)\n"
                "  local f,err = loadfile_safe(filename);\n"
                "  if err then\n"
                "    error(err);\n"
                "  end\n"
                "  return f();\n"
                "end\n"
                "\n"
                "local function io_open_safe(filename, mode)\n"
                "  if not validate_path(filename) then\n"
                "    return nil, 'invalid filename';\n"
                "  end\n"
                "  if not mode then\n"
                "    mode = 'r';\n"
                "  end\n"
                "  filename = absolute_path(filename);\n"
                "  return io.open(filename, mode);\n"
                "end\n"
                "local function table_insert(tbl, ...)\n"
                "  local args = { ... }\n"
                "  local pos, val\n"
                "  if #args == 1 then\n"
                "    pos = #tbl + 1\n"
                "    val = args[1]\n"
                "  elseif #args == 2 then\n"
                "    pos = args[1]\n"
                "    val = args[2]\n"
                "  else\n"
                "    error(\"invalid arguments\")\n"
                "  end\n"
                "  for i=#tbl,pos,-1 do\n"
                "    tbl[i+1] = tbl[i]\n"
                "  end\n"
                "  tbl[pos] = val\n"
                "end\n"
                "local function table_remove(tbl, pos)\n"
                "  if not pos then pos = #tbl end\n"
                "  local len = #tbl\n"
                "  if pos < 1 or len < pos then return nil end\n"
                "  local ret = tbl[pos]\n"
                "  for i=pos+1,len do\n"
                "    tbl[i-1] = tbl[i]\n"
                "  end\n"
                "  tbl[len] = nil\n"
                "  return ret\n"
                "end\n"
                "local function select_(pos, ...)\n"
                "  local args = { ... }\n"
                "  if pos == \"#\" then\n"
                "    return #args\n"
                "  elseif 1 <= pos and pos <= #args then\n"
                "    return args[pos]\n"
                "  else\n"
                "    error(\"invalid arguments\")\n"
                "  end\n"
                "end\n"
                "\n"
                "local safe_lib = [[\n"
                "assert error ipairs next pairs pcall select tonumber tostring type unpack _VERSION xpcall\n"
                "coroutine.create coroutine.resume coroutine.running coroutine.status coroutine.wrap coroutine.yield\n"
                "string.byte string.char string.find string.format string.gmatch string.gsub string.len\n"
                "string.lower string.match string.rep string.reverse string.sub string.upper\n"
                "table.concat table.insert table.maxn table.remove table.sort\n"
                "math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.cosh math.deg math.exp\n"
                "math.floor math.fmod math.frexp math.huge math.ldexp math.log math.log10 math.max math.min math.modf\n"
                "math.pi math.pow math.rad math.random math.randomseed math.sin math.sinh math.sqrt math.tan math.tanh\n"
                "io.type\n"
                "os.clock os.date os.difftime os.time\n"
                "\n"
                "game_sides player_side round difficulty sendKeys hitTest saveSnapshot\n"
                "HitType ItemType ExAttackType CharacterType ChargeType\n"
                "]];\n"
                "for s in string.gmatch(safe_lib, '[a-zA-Z0-9_%.]+') do\n"
                "  local s1, s2 = string.match(s, '([a-zA-Z0-9_]+)%.([a-zA-Z0-9_]+)');\n"
                "  if not s2 then\n"
                "    env[s] = _G[s];\n"
                "  else\n"
                "    if not env[s1] then\n"
                "      env[s1] = {}\n"
                "    end\n"
                "    env[s1][s2] = _G[s1][s2];\n"
                "  end\n"
                "end\n"
                "env.loadfile = loadfile_safe;\n"
                "env.dofile = dofile_safe;\n"
                "env.io.open = io_open_safe\n"
                "env.table.insert = table_insert\n"
                "env.table.remove = table_remove\n"
                "env.select = select_\n"
                "env._G = env;\n";
            ::luaL_loadbuffer(ls, src, ::strlen(src), "sandbox");
            if(::lua_pcall(ls, 0, 0, 0)){
                //error reporting is caller's task!!!
                return false;
            }
            return true;
        }

        bool DoFile(::lua_State* ls, std::string &basename)
        {
            ::lua_getglobal(ls, env_name);
            ::lua_getfield(ls, -1, "dofile");
            ::lua_remove(ls, -2);
            ::lua_pushstring(ls, basename.c_str());
            return ::lua_pcall(ls, 1, 0, 0) == 0;
        }

        bool CallMain(::lua_State* ls)
        {
            ::lua_getglobal(ls, env_name);
            ::lua_getfield(ls, -1, "main");
            ::lua_remove(ls, -2);
            return ::lua_pcall(ls, 0, 0, 0) == 0;
        }
    }
}
