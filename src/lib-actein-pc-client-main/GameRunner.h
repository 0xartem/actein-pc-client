#ifndef GAME_RUNNER_H__
#define GAME_RUNNER_H__

#include <string>
#include <mutex>

namespace spdlog
{
    class logger;
}

namespace vr_events
{
    class VrGame;
}

namespace actein
{
    class Settings;

    class GameRunner
    {
    public:
        explicit GameRunner(Settings & settings);
        void Run(const vr_events::VrGame & game);
        void Stop();
        bool IsGameRunning() const;
        const vr_events::VrGame * GetCurrentGame() const;
        
    private:
        bool mGameRunning;
        Settings & mSettings;
        std::unique_ptr<vr_events::VrGame> mCurrentGame;
        
        mutable std::mutex mSync;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //GAME_RUNNER_H__
