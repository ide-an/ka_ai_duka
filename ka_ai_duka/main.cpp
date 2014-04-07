#include <Windows.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>
#include "inject.h"
#include "../common/common.h"

const std::string dll_name("inject.dll");

void BuildDllPath(std::string& out)
{
    char str[0x800];
    ::GetModuleFileNameA(NULL, str, sizeof(str)/sizeof(str[0]));
    out.append(str, strrchr(str, '\\'));
    out.append("\\");
    out.append(dll_name);
}

int main(int argc, char** argv){

    ka_ai_duka::common::Config conf;
    std::string ini_path;
    ka_ai_duka::common::Config::IniFilePath(NULL, ini_path);
    conf.Load(ini_path);
    //TODO: validation
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ::ZeroMemory(&pi, sizeof(pi));
    char* current_dir = new char[conf.Th09ExePath().size()+1];
    ::strcpy(current_dir, conf.Th09ExePath().c_str());
    ::PathRemoveFileSpecA(current_dir);
    if(!::CreateProcess(conf.Th09ExePath().c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, NULL, current_dir, &si, &pi)){
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