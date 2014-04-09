#pragma once
#include <string>
#include <Windows.h>
namespace ka_ai_duka{
    namespace common{
        class Config{
        private:
            std::string script_path_1P;
            std::string script_path_2P;
            bool enable_1P;
            bool enable_2P;
            std::string th09_exe_path;
        public:
            bool Enable1P() const
            {
                return enable_1P;
            }
            bool Enable2P() const
            {
                return enable_2P;
            }
            void SetEnable1P(bool b)
            {
                enable_1P = b;
            }
            void SetEnable2P(bool b)
            {
                enable_2P = b;
            }
            const std::string& ScriptPath1P() const
            {
                return script_path_1P;
            }
            const std::string& ScriptPath2P() const
            {
                return script_path_2P;
            }
    
            void SetScriptPath1P(const std::string& str)
            {
                script_path_1P = str;
            }
            void SetScriptPath2P(const std::string& str)
            {
                script_path_2P = str;
            }
            const std::string& Th09ExePath() const
            {
                return th09_exe_path;
            }
            void Load(const std::string &filepath);
            void Save(const std::string &filepath);
            static void IniFilePath(HANDLE h_module, std::string &out);
        };
    }
}