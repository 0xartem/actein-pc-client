#include <windows.h>
#include "Win32Exception.h"
#include "WinUtils.h"

namespace utils
{
    bool IsWindows64bit()
    {
        BOOL isWow64 = FALSE;
        if (::IsWow64Process(::GetCurrentProcess(), &isWow64) == FALSE)
        {
            throw Win32Exception("Can not determine if the system is 32-bit or 64-bit", ::GetLastError());
        }
        return isWow64 == TRUE;
    }
}