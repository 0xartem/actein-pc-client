#include <sstream>
#include <SpdLocalLog.h>
#include <gen/vr_game.pb.h>
#include <gen/vr_game_error.pb.h>
#include <VrEventsException.h>
#include <StringUtils.h>
#include <WinServiceUtils.h>
#include <WinProcessUtils.h>
#include "Settings.h"
#include "GameRunner.h"

namespace actein
{
    GameRunner::GameRunner(Settings & settings)
        : mSettings(settings)
    {
        mLogger = spdlog::get(COMMON_LOGGER_NAME);

        std::wostringstream steamLoginStartVrStrm;
        steamLoginStartVrStrm << utils::string2wstring(mSettings.GetSteamExePath())
            << " -login " << utils::string2wstring(mSettings.GetSteamAccountName())
            << " " << utils::string2wstring(mSettings.GetSteamAccountPassword());
            //<< " -applaunch " << std::to_wstring(STEAM_VR_ID)
            //<< " -nosteamvr -nodashboard";
        mSteamLoginStartVrCmd = steamLoginStartVrStrm.str();

        std::wstringstream steamShutdownCmdStrm;
        steamShutdownCmdStrm << utils::string2wstring(mSettings.GetSteamExePath())
            << L" -shutdown " << " -login "
            << utils::string2wstring(mSettings.GetSteamAccountName()) << " "
            << utils::string2wstring(mSettings.GetSteamAccountPassword());
        mSteamShutdownCmd = steamShutdownCmdStrm.str();
    }

    void GameRunner::Run(const vr_events::VrGame & game)
    {
        // check if it works
        if (utils::IsProcessRunning(utils::string2wstring(mSettings.GetSteamExeName())))
        {
            Stop();
        }

        std::unique_lock<std::mutex> locker(mSync);

        // Shutdown steam if it's running
        //utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamExeName()));

        // Kill process to restart without the dashboard
        // utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamVrExeName()));

        utils::RunInteractiveProcess(mSteamLoginStartVrCmd);
        WaitForSteamProcessToStart(mSettings.GetSteamExeName());

        std::wostringstream steamRunGameCmdLine;
        steamRunGameCmdLine << utils::string2wstring(mSettings.GetSteamExePath())
            << L" steam://launch/" << std::to_wstring(game.steam_game_id()) << L"/VR";
        
        utils::RunInteractiveProcess(steamRunGameCmdLine.str());
        WaitForSteamProcessToStart(mSettings.GetSteamVrExeName());
        
        mCurrentGame = std::make_unique<vr_events::VrGame>(game);
    }

    const vr_events::VrGame * GameRunner::GetCurrentGame() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mCurrentGame.get();
    }

    void GameRunner::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);

        // Discard current game
        mCurrentGame.reset();

        utils::RunInteractiveProcess(mSteamShutdownCmd);
        WaitForSteamToExit();// remove this, just use previous line to wait
        utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamExeName()));
    }

    void GameRunner::WaitForSteamProcessToStart(const std::string & processName)
    {
        // wait until Steam or SteamVR process is started,
        // then wait a bit more to initialize
        std::wstring steamVrExeWstr = utils::string2wstring(processName);
        int leftToWait = MAX_START_STEAM_PROC_WAIT_TIME;
        do
        {
            ::Sleep(WAIT_STEP);
            leftToWait -= WAIT_STEP;
        }
        while (!utils::IsProcessRunning(steamVrExeWstr) && leftToWait > 0);

        if (utils::IsProcessRunning(steamVrExeWstr))
        {
            ::Sleep(STEAM_PROC_STARTUP_WAIT_TIME);
        }
        else
        {
            throw vr_events::VrEventsException(
                vr_events::VrGameErrorCode::CANNOT_START_STEAM_VR,
                "Can not start Steam VR."
            );
        }
    }

    void GameRunner::WaitForSteamToExit()
    {
        std::wstring steamExeWstr = utils::string2wstring(mSettings.GetSteamExeName());
        int leftToWait = MAX_EXIT_STEAM_WAIT_TIME;
        do
        {
            ::Sleep(WAIT_STEP);
            leftToWait -= WAIT_STEP;
        }
        while (utils::IsProcessRunning(steamExeWstr) && leftToWait > 0);
    }
}