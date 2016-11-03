#ifndef GAME_RUNNER_H__
#define GAME_RUNNER_H__

#include <string>
#include <mutex>
#include <gen/vr_game.pb.h>

namespace spdlog
{
    class logger;
}

namespace actein
{
    class GameRunner
    {
    public:
        GameRunner();
        bool Run(const vr_events::VrGame & game);
        bool Stop();
        bool IsGameRunning() const;
        
    private:
        bool RunSteamProcess(const std::wstring & commandLineStr);

    private:
        bool mGameRunning;
        std::wstring mSteamPath;
        mutable std::mutex mSync;
        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //GAME_RUNNER_H__
