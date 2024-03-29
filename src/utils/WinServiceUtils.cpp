#include "WinServiceUtils.h"
#include "Win32Exception.h"
#include <wtsapi32.h>
#include <userenv.h>

#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "userenv.lib")

namespace utils
{
    DWORD GetSessionIdOfUser(PCWSTR pszUserName, PCWSTR pszDomain) noexcept
    {
        DWORD dwSessionId = 0xFFFFFFFF;

        if (pszUserName == NULL)
        {
            // If the user name is not provided, try to get the session attached 
            // to the physical console. The physical console is the monitor, 
            // keyboard, and mouse.
            dwSessionId = WTSGetActiveConsoleSessionId();
        }
        else
        {
            // If the user name is provided, get the session of the provided user. 
            // The same user could have more than one session, this sample just 
            // retrieves the first one found. You can add more sophisticated 
            // checks by requesting different types of information from 
            // WTSQuerySessionInformation.

            PWTS_SESSION_INFO *pSessionsBuffer = NULL;
            DWORD dwSessionCount = 0;

            // Enumerate the sessions on the current server.
            if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1,
                pSessionsBuffer, &dwSessionCount))
            {
                for (DWORD i = 0; (dwSessionId == -1) && (i < dwSessionCount); i++)
                {
                    DWORD sid = pSessionsBuffer[i]->SessionId;

                    // Get the user name from the session ID.
                    PWSTR pszSessionUserName = NULL;
                    DWORD dwSize;
                    if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sid,
                        WTSUserName, &pszSessionUserName, &dwSize))
                    {
                        // Compare with the provided user name (case insensitive).
                        if (_wcsicmp(pszUserName, pszSessionUserName) == 0)
                        {
                            // Get the domain from the session ID.
                            PWSTR pszSessionDomain = NULL;
                            if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                                sid, WTSDomainName, &pszSessionDomain, &dwSize))
                            {
                                // Compare with the provided domain (case insensitive).
                                if (_wcsicmp(pszDomain, pszSessionDomain) == 0)
                                {
                                    // The session of the provided user is found.
                                    dwSessionId = sid;
                                }
                                WTSFreeMemory(pszSessionDomain);
                            }
                        }
                        WTSFreeMemory(pszSessionUserName);
                    }
                }

                WTSFreeMemory(pSessionsBuffer);
                pSessionsBuffer = NULL;
                dwSessionCount = 0;

                // Cannot find the session of the provided user.
                if (dwSessionId == 0xFFFFFFFF)
                {
                    SetLastError(ERROR_INVALID_PARAMETER);
                }
            }
        }

        return dwSessionId;
    }

    // TODO: get rid if 'goto', implement handle guard
    BOOL CreateInteractiveProcess(
        DWORD dwSessionId,
        PWSTR pszCommandLine,
        BOOL fWait,
        DWORD dwTimeout,
        DWORD *pExitCode) noexcept
    {
        DWORD dwError = ERROR_SUCCESS;
        HANDLE hToken = NULL;
        LPVOID lpvEnv = NULL;
        wchar_t szUserProfileDir[MAX_PATH];
        DWORD cchUserProfileDir = ARRAYSIZE(szUserProfileDir);
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        DWORD dwWaitResult;

        // Obtain the primary access token of the logged-on user specified by the 
        // session ID.
        if (!WTSQueryUserToken(dwSessionId, &hToken))
        {
            dwError = GetLastError();
            goto Cleanup;
        }

        // Run the command line in the session that we found by using the default 
        // values for working directory and desktop.

        // This creates the default environment block for the user.
        if (!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE))
        {
            dwError = GetLastError();
            goto Cleanup;
        }

        // Retrieve the path to the root directory of the user's profile.
        if (!GetUserProfileDirectory(hToken, szUserProfileDir,
            &cchUserProfileDir))
        {
            dwError = GetLastError();
            goto Cleanup;
        }

        // Specify that the process runs in the interactive desktop.
        si.lpDesktop = L"winsta0\\default";

        // Launch the process.
        if (!CreateProcessAsUser(hToken, NULL, pszCommandLine, NULL, NULL, FALSE,
            CREATE_UNICODE_ENVIRONMENT, lpvEnv, szUserProfileDir, &si, &pi))
        {
            dwError = GetLastError();
            goto Cleanup;
        }

        if (fWait)
        {
            // Wait for the exit of the process.
            dwWaitResult = WaitForSingleObject(pi.hProcess, dwTimeout);
            if (dwWaitResult == WAIT_OBJECT_0)
            {
                // If the process exits before timeout, get the exit code.
                GetExitCodeProcess(pi.hProcess, pExitCode);
            }
            else if (dwWaitResult == WAIT_TIMEOUT)
            {
                // If it times out, terminate the process.
                TerminateProcess(pi.hProcess, IDTIMEOUT);
                *pExitCode = IDTIMEOUT;
            }
            else
            {
                dwError = GetLastError();
                goto Cleanup;
            }
        }
        else
        {
            *pExitCode = IDASYNC;
        }

    Cleanup:

        // Centralized cleanup for all allocated resources.
        if (hToken)
        {
            CloseHandle(hToken);
            hToken = NULL;
        }
        if (lpvEnv)
        {
            DestroyEnvironmentBlock(lpvEnv);
            lpvEnv = NULL;
        }
        if (pi.hProcess)
        {
            CloseHandle(pi.hProcess);
            pi.hProcess = NULL;
        }
        if (pi.hThread)
        {
            CloseHandle(pi.hThread);
            pi.hThread = NULL;
        }

        // Set the last error if something failed in the function.
        if (dwError != ERROR_SUCCESS)
        {
            SetLastError(dwError);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    void RunInteractiveProcess(
        const std::wstring & commandLineStr,
        bool wait,
        DWORD timeout) noexcept(false)
    {
        // Get the ID of the session attached to the physical console.
        DWORD dwSessionId = utils::GetSessionIdOfUser(nullptr, nullptr);
        if (dwSessionId == 0xFFFFFFFF)
        {
            throw Win32Exception("Can not get current user id", ::GetLastError());
        }

        DWORD dwExitCode = 0;
        if (!utils::CreateInteractiveProcess(
            dwSessionId,
            const_cast<wchar_t *>(commandLineStr.c_str()),
            wait ? TRUE : FALSE,
            timeout,
            &dwExitCode))
        {
            throw Win32Exception("Can not start interactive process", ::GetLastError());
        }
    }
}