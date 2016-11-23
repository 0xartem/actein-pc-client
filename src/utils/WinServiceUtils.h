#ifndef WIN_SERVICE_UTILS_H__
#define WIN_SERVICE_UTILS_H__

#include <windows.h>
#include <string>

namespace utils
{
    DWORD GetSessionIdOfUser(PCWSTR pszUserName, PCWSTR pszDomain) noexcept;
    BOOL CreateInteractiveProcess(
        DWORD dwSessionId,
        PWSTR pszCommandLine,
        BOOL fWait,
        DWORD dwTimeout,
        DWORD *pExitCode
    ) noexcept;

    void RunInteractiveProcess(
        const std::wstring & commandLineStr,
        bool wait = false,
        DWORD timeout = 0
    ) noexcept(false);
}

#endif //WIN_SERVICE_UTILS_H__
