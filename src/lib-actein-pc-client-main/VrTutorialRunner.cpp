#include <StringUtils.h>
#include <WinServiceUtils.h>
#include <WinProcessUtils.h>
#include "Settings.h"
#include "VrTutorialRunner.h"

namespace actein
{
    VrTutorialRunner::VrTutorialRunner(Settings & settings)
        : mSettings(settings)
    {
        mTutorialRunCmd = utils::string2wstring(mSettings.GetVrTutorialExePath())
            + L" -popupwindow -tutorial";
    }

    std::future<void> VrTutorialRunner::RunAsync(bool wait)
    {
        return std::async(std::launch::async, &VrTutorialRunner::Run, this, wait);
    }

    void VrTutorialRunner::WaitForTutorialStart()
    {
        do
        {
            ::Sleep(1000);
        }
        while (!utils::IsProcessRunning(utils::string2wstring(mSettings.GetVrTutorialExeName())));
    }

    void VrTutorialRunner::Run(bool wait)
    {
        std::unique_lock<std::mutex> locker(mSync);
        if (!utils::IsProcessRunning(utils::string2wstring(mSettings.GetSteamVrDashboardExeName())))
        {
            utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamVrExeName()));
        }

        if (wait)
        {
            locker.unlock();
        }

        utils::RunInteractiveProcess(mTutorialRunCmd, wait, INFINITE);
    }

    void VrTutorialRunner::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);
        utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetVrTutorialExeName()));
        // Run process again after killing to avoid ERROR_ACCESS_DENIED in the next start
        DWORD maxWaitTimeout = 5000;
        utils::RunInteractiveProcess(mTutorialRunCmd, true, maxWaitTimeout);
    }
}