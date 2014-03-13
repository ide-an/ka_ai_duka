#include "inject.h"
#include <iostream>

namespace ka_ai_duka{
    bool InjectDll(HANDLE process, const std::string& dll_path){
        size_t arg_size = ::strlen(dll_path.c_str()) + 1;
        void* arg_addr = ::VirtualAllocEx(process, NULL, arg_size, MEM_COMMIT, PAGE_READWRITE);
        if(!arg_addr){
            std::cerr << "VirtualAllocEx failed" << std::endl;
            return false;
        }
        std::cerr << arg_size << std::endl;
        std::cerr << dll_path.c_str() << std::endl;
        DWORD written;
        if(!::WriteProcessMemory(process, arg_addr, dll_path.c_str(), arg_size, &written)){
            std::cerr << "WriteProcessMemory failed" << std::endl;
            return false;
        }
        std::cerr << "written:" << written << std::endl;
        PTHREAD_START_ROUTINE thread_routine = reinterpret_cast<PTHREAD_START_ROUTINE>(
            ::GetProcAddress(::GetModuleHandleA("kernel32"), "LoadLibraryA"));
        if(!thread_routine){
            std::cerr << "GetProcAddress failed" << std::endl;
            return false;
        }
        HANDLE thread = ::CreateRemoteThread(process, NULL, 0, thread_routine, arg_addr, 0, NULL);
        if(!thread){
            std::cerr << "CreateRemoteThread failed" << std::endl;
            return false;
        }
        ::WaitForSingleObject(thread, INFINITE);
        ::CloseHandle(thread);
        ::VirtualFreeEx(process, arg_addr, arg_size, MEM_RELEASE);
        return true;
    }
}