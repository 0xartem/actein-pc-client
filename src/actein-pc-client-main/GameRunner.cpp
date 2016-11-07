#include <sstream>
#include <spdlog/spdlog.h>
#include <WinServiceUtils.h>
#include "GameRunner.h"

namespace actein
{
    GameRunner::GameRunner()
        : mSteamPath(L"\"C:\\Program Files (x86)\\Steam\\Steam.exe\"")
        , mGameRunning(false)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    void GameRunner::Run(const vr_events::VrGame & game)
    {
        std::unique_lock<std::mutex> locker(mSync);

        std::wstringstream commandLineStrm;
        commandLineStrm << mSteamPath << L" -applaunch "
            << std::to_wstring(game.steam_game_id()) << " -login actein RimRam66!";

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
        commandLineStrm << mSteamPath << " -shutdown -login actein RimRam66!";

        utils::RunInteractiveProcess(commandLineStrm.str());
    }
}