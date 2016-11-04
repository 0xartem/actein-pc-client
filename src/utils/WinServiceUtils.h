#ifndef WIN_SERVICE_UTILS_H__
#define WIN_SERVICE_UTILS_H__

#include <windows.h>
#include <string>

namespace utils
{
    // no exception guarantee
    DWORD GetSessionIdOfUser(PCWSTR pszUserName, PCWSTR pszDomain);
    // no exception guarantee
    BOOL CreateInteractiveProcess(
        DWORD dwSessionId,
        PWSTR pszCommandLine,
        BOOL fWait,
        DWORD dwTimeout,
        DWORD *pExitCode
    );

    // throws Win32Exception
    void RunInteractiveProcess(const std::wstring & commandLineStr);
}

#endif //WIN_SERVICE_UTILS_H__
