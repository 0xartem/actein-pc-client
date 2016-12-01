#include <sstream>
#include <spdlog/spdlog.h>
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
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);

        std::wostringstream steamLoginStartVrStrm;
        steamLoginStartVrStrm << utils::string2wstring(mSettings.GetSteamExePath())
            << " -login " << utils::string2wstring(mSettings.GetSteamAccountName())
            << " " << utils::string2wstring(mSettings.GetSteamAccountPassword())
            << " -applaunch " << std::to_wstring(STEAM_VR_ID)
            << " -nosteamvr -nodashboard";
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
        std::unique_lock<std::mutex> locker(mSync);

        // Kill process to restart without the dashboard
        utils::KillIfProcessRunning(utils::string2wstring(mSettings.GetSteamVrExeName()));

        utils::RunInteractiveProcess(mSteamLoginStartVrCmd);
        WaitForSteamVrToStart();

        std::wostringstream steamRunGameCmdLine;
        steamRunGameCmdLine << utils::string2wstring(mSettings.GetSteamExePath())
            << L" steam://launch/" << std::to_wstring(game.steam_game_id()) << L"/VR";
        
        utils::RunInteractiveProcess(steamRunGameCmdLine.str());
        
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
    }

    void GameRunner::WaitForSteamVrToStart()
    {
        // wait until SteamVR process is started,
        // then wait a bit more for SteamVR to initialize
        std::wstring steamVrExeWstr = utils::string2wstring(mSettings.GetSteamVrExeName());
        int leftToWait = MAX_START_STEAM_VR_WAIT_TIME;
        do
        {
            ::Sleep(WAIT_STEP);
            leftToWait -= WAIT_STEP;
        }
        while (!utils::IsProcessRunning(steamVrExeWstr) && leftToWait > 0);

        if (utils::IsProcessRunning(steamVrExeWstr))
        {
            ::Sleep(STEAM_VR_STARTUP_WAIT_TIME);
        }
        else
        {
            throw vr_events::VrEventsException(
                vr_events::VrGameErrorCode::CANNOT_START_STEAM_VR,
                "Can not start Steam VR."
            );
        }
    }
}