#ifndef SCHEDULE_VR_EVENTS_HANDLER_H__
#define SCHEDULE_VR_EVENTS_HANDLER_H__

#include <vector>
#include <mutex>
#include <future>
#include <IVrEventsHandler.h>
#include "IGameStatusObserver.h"

namespace spdlog
{
    class logger;
}

namespace utils
{
    class ThreadTimer;
}

namespace vr_events
{
    class IVrEventsManagerOwner;
}

namespace actein
{
    class Settings;
    class GameRunner;
    class VrTutorialRunner;

    class ScheduleVrEventsHandler :
        public vr_events::IVrEventsHandler,
        public IGameStatusObserver
    {
    public:
        explicit ScheduleVrEventsHandler(
            Settings & settings,
            vr_events::IVrEventsManagerOwner & vrEventsManagerOwner
        );
        ~ScheduleVrEventsHandler();

        void OnStart();

        // vr_events::IVrEventsHandler
        void HandleVrGameOnEvent(const std::shared_ptr<vr_events::VrGameOnEvent> & event) override;
        void HandleVrGameOffEvent(const std::shared_ptr<vr_events::VrGameOffEvent> & event) override;
        void HandleVrGameStatusEvent(const std::shared_ptr<vr_events::VrGameStatusEvent> & event) override;

        // IGameStatusObserver
        vr_events::VrGameStatus GetGameStatus() const override;
        void OnGameStatusChanged(const vr_events::VrGameStatus & status) override;
        void OnGameStatusError(const std::string & error) override;

    private:
        bool IsGameRunning() const;
        bool ContinueStarting();
        void StartGameRoutine(std::shared_ptr<vr_events::VrGameOnEvent> event);
        void StopGameRoutine();

    private:
        mutable std::mutex mStatusSync;
        vr_events::VrGameStatus mStatus;
        mutable std::mutex mFuturesSync;
        std::vector<std::future<void>> mFuturesVec;

        std::unique_ptr<VrTutorialRunner> mTutorialRunner;
        std::unique_ptr<GameRunner> mGameRunner;
        std::unique_ptr<utils::ThreadTimer> mGameStopTimer;
        std::shared_ptr<spdlog::logger> mLogger;

        vr_events::IVrEventsManagerOwner & mVrEventsManagerOwner;
    };
}

#endif //SCHEDULE_VR_EVENTS_HANDLER_H__
