#include <Windows.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>
#include "inject.h"
#include "../common/common.h"
#include "../common/errorhandle.h"

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
    try{
        ka_ai_duka::common::Config::IniFilePath(NULL, ini_path);
        conf.Load(ini_path);
    }catch(const std::exception &e){
        std::ostringstream os;
        os << e.what() << std::endl;
        ka_ai_duka::ReportError(os);
        return 1;
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ::ZeroMemory(&pi, sizeof(pi));
    char* current_dir = new char[conf.Th09ExePath().size()+1];
    ::strcpy(current_dir, conf.Th09ExePath().c_str());
    ::PathRemoveFileSpecA(current_dir);
    if(!::CreateProcess(conf.Th09ExePath().c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, NULL, current_dir, &si, &pi)){
        std::ostringstream os;
        os << "‰Ô‰f’Ë‚Ì‹N“®‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        ka_ai_duka::ReportError(os);
        return 1;
    }
    delete[] current_dir;
    std::string dll_path;
    BuildDllPath(dll_path);
    std::cerr << "DLL path:" << dll_path << std::endl;
    if(!ka_ai_duka::InjectDll(pi.hProcess, dll_path)){
        std::ostringstream os;
        os << "DLL injection‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        ka_ai_duka::ReportError(os);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        return 1;
    }
    ::ResumeThread(pi.hThread);
    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);
    return 0;
}