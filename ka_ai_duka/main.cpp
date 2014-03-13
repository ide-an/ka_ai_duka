#include <Windows.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>
#include "inject.h"

const std::string dll_name("inject.dll");

typedef struct{
    std::string th09_exe_path;
    bool enable_1P;
    bool enable_2P;
    std::string script_path_1P;
    std::string script_path_2P;
} Config;

void LoadConfig(Config& conf, char* filepath){
    conf.th09_exe_path = "C:\\Users\\ide\\toho\\tmp-kaeiduka\\“Œ•û‰Ô‰f’Ë\\th09.exe";
    //TODO: implement
}

void BuildDllPath(std::string& out)
{
    char str[0x800];
    ::GetModuleFileNameA(NULL, str, sizeof(str)/sizeof(str[0]));
    out.append(str, strrchr(str, '\\'));
    out.append("\\");
    out.append(dll_name);
}

int main(int argc, char** argv){
    Config conf;
    LoadConfig(conf, "hoge.ini");
    //TODO: validation
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ::ZeroMemory(&pi, sizeof(pi));
    char* current_dir = new char[conf.th09_exe_path.size()+1];
    ::strcpy(current_dir, conf.th09_exe_path.c_str());
    ::PathRemoveFileSpecA(current_dir);
    if(!::CreateProcess(conf.th09_exe_path.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, NULL, current_dir, &si, &pi)){
        char message[0x800];
        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), message, sizeof(message), NULL);
        std::cerr << "‰Ô‰f’Ë‚Ì‹N“®‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        std::cerr << message << std::endl;
        return 1;
    }
    delete[] current_dir;
    std::string dll_path;
    BuildDllPath(dll_path);
    std::cerr << "DLL path:" << dll_path << std::endl;
    if(!ka_ai_duka::InjectDll(pi.hProcess, dll_path)){
        std::cerr << "DLL injection‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        return 1;
    }
    ::ResumeThread(pi.hThread);
    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);
    return 0;
}