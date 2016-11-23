#include <windows.h>
#include <tlhelp32.h>
#include "StringUtils.h"
#include "HandleGuard.h"
#include "Win32Exception.h"
#include "WinProcessUtils.h"

namespace utils
{
    bool FindProcessByName(const std::wstring & processName, HandleGuard & handleGuard)
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshotHandle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (snapshotHandle == INVALID_HANDLE_VALUE)
        {
            throw Win32Exception("Can not get snapshot CreateToolhelp32Snapshot", ::GetLastError());
        }
        HandleGuard snapshotGuard(snapshotHandle);

        if (::Process32First(snapshotHandle, &entry) == TRUE)
        {
            while (::Process32Next(snapshotHandle, &entry) == TRUE)
            {
                if (processName == entry.szExeFile)
                {
                    handleGuard.reset(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID));
                    return true;
                }
            }
        }
        return false;
    }

    bool IsProcessRunning(const std::wstring & processName)
    {
        HandleGuard processGuard;
        return FindProcessByName(processName, processGuard);
    }

    void KillIfProcessRunning(const std::wstring & processName)
    {
        HandleGuard processGuard;
        if (FindProcessByName(processName, processGuard))
        {
            if (::TerminateProcess(processGuard.get(), 0) == FALSE)
            {
                throw Win32Exception(
                    "Can not terminate process: " + utils::wstring2string(processName),
                    ::GetLastError()
                );
            }
        }
    }
}