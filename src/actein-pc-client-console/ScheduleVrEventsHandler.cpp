#include <spdlog/spdlog.h>
#include <IVrEventsManagerOwner.h>
#include <IVrEventsManager.h>
#include <IVrEventsPublisher.h>
#include <gen/vr_game_on_event.pb.h>
#include <gen/vr_game_off_event.pb.h>
#include <gen/vr_game_status_event.pb.h>
#include <ThreadTimer.h>
#include "ScheduleVrEventsHandler.h"

using ThTimer = utils::ThreadTimer;

ScheduleVrEventsHandler::ScheduleVrEventsHandler(vr_events::IVrEventsManagerOwner * vrEventsManagerOwner)
    : mVrEventsManagerOwner(vrEventsManagerOwner)
{
    mGameStopTimer = std::make_unique<ThTimer>(
        ThTimer::UsageType::DurationUsage,
        false,
        std::bind(&ScheduleVrEventsHandler::StopGameRoutine, this)
        );

    mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
}

void ScheduleVrEventsHandler::HandleVrGameOnEvent(const std::shared_ptr<vr_events::VrGameOnEvent> & event)
{
    mLogger->info("VR game on event received. Game {}", event->game().game_name());

    mGameRunner.Run(event->game());

    //mGameStopTimer->SetDuration(std::chrono::seconds(event->game().game_duration_seconds()));
    //mGameStopTimer->Start();

    SendStatusEvent(vr_events::VrGameStatus::GAME_ON);
}

void ScheduleVrEventsHandler::HandleVrGameOffEvent(const std::shared_ptr<vr_events::VrGameOffEvent> & event)
{
    mLogger->info("VR game off event received.");
    StopGameRoutine();
}

void ScheduleVrEventsHandler::HandleVrGameStatusEvent(const std::shared_ptr<vr_events::VrGameStatusEvent> & event)
{
    mLogger->info("VR game status event received");
}

void ScheduleVrEventsHandler::StopGameRoutine()
{
    mGameRunner.Stop();
    SendStatusEvent(vr_events::VrGameStatus::GAME_OFF);
}

void ScheduleVrEventsHandler::SendStatusEvent(const vr_events::VrGameStatus & status)
{
    vr_events::IVrEventsManager * vrManager = mVrEventsManagerOwner->GetVrEventsManager();
    if (vrManager != nullptr)
    {
        vrManager->GetPublisher()->PublishVrGameStatusEvent(status);
    }
}
