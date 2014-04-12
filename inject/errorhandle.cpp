#include "errorhandle.h"
#include <Windows.h>
namespace ka_ai_duka{
    void ReportError(std::ostringstream &os)
    {
        ::MessageBoxA(NULL, os.str().c_str(), "Error", MB_OK);
    }
}