#ifndef SCHEDULE_VR_EVENTS_HANDLER_H__
#define SCHEDULE_VR_EVENTS_HANDLER_H__

#include <memory>
#include <IVrEventsHandler.h>
#include "GameRunner.h"

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

class ScheduleVrEventsHandler : public vr_events::IVrEventsHandler
{
public:
    explicit ScheduleVrEventsHandler(vr_events::IVrEventsManagerOwner * vrEventsManagerOwner);
    
    // vr_events::IVrEventsHandler
    void HandleVrGameOnEvent(const std::shared_ptr<vr_events::VrGameOnEvent> & event) override;
    void HandleVrGameOffEvent(const std::shared_ptr<vr_events::VrGameOffEvent> & event) override;
    void HandleVrGameStatusEvent(const std::shared_ptr<vr_events::VrGameStatusEvent> & event) override;

private:
    void StopGameRoutine();
    void SendStatusEvent(const vr_events::VrGameStatus & status);

private:
    GameRunner mGameRunner;
    std::unique_ptr<utils::ThreadTimer> mGameStopTimer;
    std::shared_ptr<spdlog::logger> mLogger;
    vr_events::IVrEventsManagerOwner * mVrEventsManagerOwner;
};

#endif //SCHEDULE_VR_EVENTS_HANDLER_H__
