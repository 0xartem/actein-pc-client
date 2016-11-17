#include <sstream>
#include <spdlog/spdlog.h>
#include <gen/vr_game.pb.h>
#include <StringUtils.h>
#include <WinServiceUtils.h>
//#include "SteamClient.h"
#include "Settings.h"
#include "GameRunner.h"

namespace actein
{
    GameRunner::GameRunner(Settings & settings)
        : mGameRunning(false)
        , mSettings(settings)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    void GameRunner::Run(const vr_events::VrGame & game)
    {
        std::unique_lock<std::mutex> locker(mSync);

        std::wostringstream steamLoginCmdLine;
        steamLoginCmdLine << utils::string2wstring(mSettings.GetSteamExePath())
            << " -login " << utils::string2wstring(mSettings.GetSteamAccountName())
            << " " << utils::string2wstring(mSettings.GetSteamAccountPassword());

        utils::RunInteractiveProcess(steamLoginCmdLine.str());

        // TODO: wait until Steam logs in; when we will have ui application:
        // use Steam API to check if user is logged in to Steam
        ::Sleep(10000);
        /*SteamClient steamClient;
        if (steamClient.WaitForSteamUserToLogin(std::chrono::milliseconds(15000)))
        {
            throw std::runtime_error("Can not login to the Steam");
        }
        steamClient.Shutdown();*/

        std::wostringstream steamRunGameCmdLine;
        steamRunGameCmdLine << utils::string2wstring(mSettings.GetSteamExePath())
            << L" steam://launch/" << std::to_wstring(game.steam_game_id()) << L"/VR";
        
        utils::RunInteractiveProcess(steamRunGameCmdLine.str());
        
        // TODO: this feature block headset's screen also, so not it's disabled
        //std::wstring runDllLockScreen = L"rundll32.exe user32.dll,LockWorkStation";
        //utils::RunInteractiveProcess(runDllLockScreen);

        mCurrentGame = std::make_unique<vr_events::VrGame>(game);
        mGameRunning = true;
    }

    bool GameRunner::IsGameRunning() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mGameRunning && mCurrentGame.get() != nullptr;
    }

    const vr_events::VrGame * GameRunner::GetCurrentGame() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mCurrentGame.get();
    }

    void GameRunner::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);

        mGameRunning = false;
        // Discard current game
        mCurrentGame.reset();

        std::wstringstream steamCommandLine;
        steamCommandLine << utils::string2wstring(mSettings.GetSteamExePath())
            << L" -shutdown " << " -login "
            << utils::string2wstring(mSettings.GetSteamAccountName()) << " "
            << utils::string2wstring(mSettings.GetSteamAccountPassword());

        utils::RunInteractiveProcess(steamCommandLine.str());
    }
}