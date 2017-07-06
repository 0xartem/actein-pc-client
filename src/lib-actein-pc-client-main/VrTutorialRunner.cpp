#include <StringUtils.h>
#include <WinServiceUtils.h>
#include <WinProcessUtils.h>
#include <gen/vr_game_error.pb.h>
#include <VrEventsException.h>
#include "Settings.h"
#include "VrTutorialRunner.h"

namespace actein
{
    VrTutorialRunner::VrTutorialRunner(Settings & settings)
        : mSettings(settings)
    {
        mTutorialRunCmd = utils::string2wstring(mSettings.GetVrTutorialExePath())
            + L" -popupwindow";
    }

    std::future<void> VrTutorialRunner::RunAsync()
    {
        return std::async(std::launch::async, &VrTutorialRunner::Run, this);
    }

    void VrTutorialRunner::WaitForTutorialStart(int waitTimeout /* = MAX_START_WAIT_TIME */)
    {
        std::wstring vrTutorialExeWstr = utils::string2wstring(mSettings.GetVrTutorialExeName());
        int leftToWait = waitTimeout;
        do
        {
            ::Sleep(WAIT_STEP);
            leftToWait -= WAIT_STEP;
        }
        while (!utils::IsProcessRunning(vrTutorialExeWstr) && leftToWait > 0);

        if (!utils::IsProcessRunning(vrTutorialExeWstr))
        {
            throw vr_events::VrEventsException(
                vr_events::VrGameErrorCode::CANNOT_START_STEAM_VR_TUTORIAL,
                "Can not start Steam VR Tutorial."
            );
        }
    }

    void VrTutorialRunner::Run()
    {
        std::unique_lock<std::mutex> locker(mSync);
        if (!utils::IsProcessRunning(utils::string2wstring(mSettings.GetSteamVrDashboardExeName())))
        {
            utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamVrExeName()));
        }
        locker.unlock();

        utils::RunInteractiveProcess(mTutorialRunCmd, true, INFINITE);
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