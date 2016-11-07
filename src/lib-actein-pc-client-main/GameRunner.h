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
        void Run(const vr_events::VrGame & game);
        void Stop();
        bool IsGameRunning() const;
        const vr_events::VrGame * GetCurrentGame() const;
        
    private:
        const std::wstring mSteamPath;
        mutable std::mutex mSync;
        
        bool mGameRunning;
        std::unique_ptr<vr_events::VrGame> mCurrentGame;
        
        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //GAME_RUNNER_H__
