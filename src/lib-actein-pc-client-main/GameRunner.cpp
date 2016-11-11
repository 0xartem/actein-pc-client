#include <sstream>
#include <spdlog/spdlog.h>
#include <gen/vr_game.pb.h>
#include <StringUtils.h>
#include <WinServiceUtils.h>
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

        std::wstringstream commandLineStrm;
        commandLineStrm << utils::string2wstring(mSettings.GetSteamExePath())
            << L" -applaunch " << std::to_wstring(game.steam_game_id()) << " -login "
            << utils::string2wstring(mSettings.GetSteamAccountName()) << " "
            << utils::string2wstring(mSettings.GetSteamAccountPassword());

        utils::RunInteractiveProcess(commandLineStrm.str());

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

        std::wstringstream commandLineStrm;
        commandLineStrm << utils::string2wstring(mSettings.GetSteamExePath())
            << L" -shutdown " << " -login "
            << utils::string2wstring(mSettings.GetSteamAccountName()) << " "
            << utils::string2wstring(mSettings.GetSteamAccountPassword());

        utils::RunInteractiveProcess(commandLineStrm.str());
    }
}