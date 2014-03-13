#pragma once
#include<string>
#include<Windows.h>

namespace ka_ai_duka{
    bool InjectDll(HANDLE process, const std::string& dll_path);
}