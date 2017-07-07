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
        void WaitForSteamProcessToStart(const std::string & processName);
        void WaitForSteamToExit();
        
    private:
        std::wstring mSteamLoginStartVrCmd;
        std::wstring mSteamShutdownCmd;
        mutable std::mutex mSync;
        Settings & mSettings;
        std::unique_ptr<vr_events::VrGame> mCurrentGame;
        
        std::shared_ptr<spdlog::logger> mLogger;

        static const int STEAM_VR_ID = 250820;
        static const int MAX_START_STEAM_PROC_WAIT_TIME = 20000;
        static const int STEAM_PROC_STARTUP_WAIT_TIME = 8000;
        static const int MAX_EXIT_STEAM_WAIT_TIME = 8000;
        static const int WAIT_STEP = 1000;
    };
}

#endif //GAME_RUNNER_H__
