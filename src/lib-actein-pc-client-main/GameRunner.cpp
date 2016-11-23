#include <sstream>
#include <spdlog/spdlog.h>
#include <gen/vr_game.pb.h>
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

        // wait until SteamVR process is started,
        // then wait a bit more for SteamVR to initialize
        do
        {
            ::Sleep(1000);
        }
        while (!utils::IsProcessRunning(utils::string2wstring(mSettings.GetSteamVrExeName())));
        ::Sleep(5000);

        std::wostringstream steamRunGameCmdLine;
        steamRunGameCmdLine << utils::string2wstring(mSettings.GetSteamExePath())
            << L" steam://launch/" << std::to_wstring(game.steam_game_id()) << L"/VR";
        
        utils::RunInteractiveProcess(steamRunGameCmdLine.str());
        
        // TODO: this feature blocks headset's screen also, so now it's disabled
        //std::wstring runDllLockScreen = L"rundll32.exe user32.dll,LockWorkStation";
        //utils::RunInteractiveProcess(runDllLockScreen);

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
}