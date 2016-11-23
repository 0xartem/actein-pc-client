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
        const vr_events::VrGame * GetCurrentGame() const;
        
    private:
        std::wstring mSteamLoginStartVrCmd;
        std::wstring mSteamShutdownCmd;
        mutable std::mutex mSync;
        Settings & mSettings;
        std::unique_ptr<vr_events::VrGame> mCurrentGame;
        
        std::shared_ptr<spdlog::logger> mLogger;

        static const int STEAM_VR_ID = 250820;
    };
}

#endif //GAME_RUNNER_H__
