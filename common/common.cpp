#include "common.h"
#include <Windows.h>
namespace ka_ai_duka{
    namespace common{
        const std::string ini_name("ka_ai_duka.ini");
        const char* key_script_path = "script_path";
        const char* key_enabled = "enabled";
        const char* key_exe_path = "exe_path";
        const char* sec_1p = "1p";
        const char* sec_2p = "2p";
        const char* sec_common = "common";
        const char* val_bool_true = "true";
        const char* val_bool_false = "false";
        bool FileExists(const std::string& file_path)
        {
            DWORD attr = ::GetFileAttributesA(file_path.c_str());
            return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
        }
        std::string ReadIniString(const char* sec, const char* key, const char* defualt_str, const char* filename)
        {
            char buf[0xff];
            int n = ::GetPrivateProfileStringA(sec, key, defualt_str, buf, sizeof(buf), filename);
            if(n < 0xff){
                return std::string(buf);
            }else{
                //TODO: 文字列が長い?
                return "";
            }
        }
        bool ReadIniBool(const char* sec, const char* key, bool default_bool, const char* filename)
        {
            std::string res = ReadIniString(sec, key, default_bool ? val_bool_true: val_bool_false, filename);
            return res.compare("true") == 0;
        }
        void WriteIniString(const char* sec, const char* key, const std::string &val, const char* filename)
        {
            ::WritePrivateProfileStringA(sec, key, val.c_str(), filename);
        }
        void WriteIniBool(const char* sec, const char* key, bool val, const char* filename)
        {
            WriteIniString(sec, key, val ? "true" : "false", filename);
        }
        void Config::Load(const std::string &file_path)
        {
            if(FileExists(file_path)){
                script_path_1P = ReadIniString(sec_1p, key_script_path, "", file_path.c_str());
                enable_1P = ReadIniBool(sec_1p, key_enabled, false, file_path.c_str());
                script_path_2P = ReadIniString(sec_2p, key_script_path, "", file_path.c_str());
                enable_2P = ReadIniBool(sec_2p, key_enabled, false, file_path.c_str());
                th09_exe_path = ReadIniString(sec_common, key_exe_path, "", file_path.c_str());
            }else{
                //TODO: I should notify error?
            }
            //TODO: path文字列の正規化。/区切りをバックスラッシュ区切りに変える
        }
        void Config::Save(const std::string &file_path)
        {
            WriteIniString(sec_1p, key_script_path, script_path_1P, file_path.c_str());
            WriteIniBool(sec_1p, key_enabled, enable_1P, file_path.c_str());
            WriteIniString(sec_2p, key_script_path, script_path_2P, file_path.c_str());
            WriteIniBool(sec_2p, key_enabled, enable_2P, file_path.c_str());
            WriteIniString(sec_common, key_exe_path, th09_exe_path, file_path.c_str());
        }

        //モジュールからファイルパスを取り出し、iniファイルのフルパスを得る。
        //トリッキーだが、プロセス間の通信が不要になる
        void Config::IniFilePath(HANDLE h_module, std::string &out)
        {
            char str[0x800];
            ::GetModuleFileNameA((HMODULE)h_module, str, sizeof(str)/sizeof(str[0]));
            //TODO: パスの文字列長チェック
            out.append(str, strrchr(str, '\\'));
            out.append("\\");
            out.append(ini_name);
        }
    }
}