#ifndef WIN_PROCESS_UTILS_H__
#define WIN_PROCESS_UTILS_H__

#include <string>

namespace utils
{
    bool IsProcessRunning(const std::wstring & processName);
    void KillIfProcessRunning(const std::wstring & processName);
}

#endif //WIN_PROCESS_UTILS_H__
