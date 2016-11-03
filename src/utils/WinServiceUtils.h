#ifndef WIN_SERVICE_UTILS_H__
#define WIN_SERVICE_UTILS_H__

#include <windows.h>

namespace utils
{
    DWORD GetSessionIdOfUser(PCWSTR pszUserName, PCWSTR pszDomain);
    BOOL CreateInteractiveProcess(
        DWORD dwSessionId,
        PWSTR pszCommandLine,
        BOOL fWait,
        DWORD dwTimeout,
        DWORD *pExitCode
    );
}

#endif //WIN_SERVICE_UTILS_H__
