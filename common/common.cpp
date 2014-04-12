#include "common.h"
#include <Windows.h>
#include <stdexcept>
#include <sstream>
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
            if(n < 0xff - 1){
                return std::string(buf);
            }else{
                std::ostringstream os;
                os << "iniファイルの値が長過ぎます。" << std::endl;
                os << "セクション名: " << sec << std::endl;
                os << "キー名: " << key << std::endl;
                throw std::length_error(os.str().c_str());
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
        void NormalizePath(std::string &path)
        {
            std::string before("/");
            std::string after("\\");
            auto it = path.find(before);
            while(it != std::string::npos){
                path.replace(it, before.length(), after);
                it = path.find(before, it + after.length());
            }
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
                std::ostringstream os;
                os << "iniファイルが見つかりません。" << std::endl;
                os << "ファイルパス: " << file_path << std::endl;
                throw std::exception(os.str().c_str());
            }
            NormalizePath(script_path_1P);
            NormalizePath(script_path_2P);
            NormalizePath(th09_exe_path);
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
            int len = ::GetModuleFileNameA((HMODULE)h_module, str, sizeof(str)/sizeof(str[0]));
            if(len >= 0x800 - 1){
                std::ostringstream os;
                os << "iniファイルのパスが長過ぎます。" << std::endl;
                throw std::length_error(os.str().c_str());
            }
            out.append(str, strrchr(str, '\\'));
            out.append("\\");
            out.append(ini_name);
        }
    }
}