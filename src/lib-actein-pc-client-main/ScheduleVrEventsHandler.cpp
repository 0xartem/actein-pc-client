#include <spdlog/spdlog.h>
#include <IVrEventsManagerOwner.h>
#include <IVrEventsManager.h>
#include <IVrEventsPublisher.h>
#include <gen/vr_game_on_event.pb.h>
#include <gen/vr_game_off_event.pb.h>
#include <gen/vr_game_status_event.pb.h>
#include <VrEventsException.h>
#include <ThreadTimer.h>
#include "Settings.h"
#include "GameRunner.h"
#include "VrTutorialRunner.h"
#include "ScheduleVrEventsHandler.h"

namespace actein
{
    using ThTimer = utils::ThreadTimer;

    ScheduleVrEventsHandler::ScheduleVrEventsHandler(
        Settings & settings,
        vr_events::IVrEventsManagerOwner & vrEventsManagerOwner)
        : mVrEventsManagerOwner(vrEventsManagerOwner)
        , mStatus(vr_events::VrGameStatus::UNKNOWN)
    {
        mTutorialRunner = std::make_unique<VrTutorialRunner>(settings);
        mGameRunner = std::make_unique<GameRunner>(settings);

        mGameStopTimer = std::make_unique<ThTimer>(
            ThTimer::UsageType::DurationUsage,
            false,
            std::bind(&ScheduleVrEventsHandler::StopGameRoutine, this)
            );

        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    void ScheduleVrEventsHandler::OnStart()
    {
        OnGameStatusChanged(vr_events::VrGameStatus::GAME_OFF);
    }

    ScheduleVrEventsHandler::~ScheduleVrEventsHandler()
    {
        mGameStopTimer->Stop();
    }

    void ScheduleVrEventsHandler::HandleVrGameOnEvent(const std::shared_ptr<vr_events::VrGameOnEvent> & event)
    {
        mLogger->info("VR game on event received. Game {}", event->game().game_name());

        if (ContinueStarting())
        {
            std::unique_lock<std::mutex> locker(mFuturesSync);
            mFuturesVec.push_back(std::async(
                std::launch::async,
                &ScheduleVrEventsHandler::StartGameRoutine,
                this, event
            ));
        }
    }

    void ScheduleVrEventsHandler::HandleVrGameOffEvent(const std::shared_ptr<vr_events::VrGameOffEvent> & event)
    {
        mLogger->info("VR game off event received.");
        if (mGameStopTimer->IsRunning())
        {
            mGameStopTimer->Stop();
        }

        std::unique_lock<std::mutex> locker(mFuturesSync);
        mFuturesVec.push_back(std::async(
            std::launch::async,
            &ScheduleVrEventsHandler::StopGameRoutine,
            this
        ));
    }

    void ScheduleVrEventsHandler::HandleVrGameStatusEvent(const std::shared_ptr<vr_events::VrGameStatusEvent> & event)
    {
        mLogger->info("VR game status event received");
    }

    vr_events::VrGameStatus ScheduleVrEventsHandler::GetGameStatus() const
    {
        std::unique_lock<std::mutex> locker(mStatusSync);
        return mStatus;
    }

    void ScheduleVrEventsHandler::OnGameStatusChanged(vr_events::VrGameStatus status)
    {
        std::unique_lock<std::mutex> locker(mStatusSync);
        mStatus = status;
        locker.unlock();
        vr_events::IVrEventsManager * vrManager = mVrEventsManagerOwner.GetVrEventsManager();
        if (vrManager != nullptr)
        {
            vrManager->GetPublisher()->PublishVrGameStatusEvent(status);
        }
    }

    void ScheduleVrEventsHandler::OnGameStatusError(
        vr_events::VrGameErrorCode errorCode,
        const std::string & error)
    {
        try
        {
            std::unique_lock<std::mutex> locker(mStatusSync);
            vr_events::VrGameStatus curStatus = mStatus;
            locker.unlock();
            vr_events::IVrEventsManager * vrManager = mVrEventsManagerOwner.GetVrEventsManager();
            if (vrManager != nullptr)
            {
                std::unique_ptr<vr_events::VrGameError> vrError = std::make_unique<vr_events::VrGameError>();
                vrError->set_error_code(errorCode);
                vrError->set_error_message(error);
                vrManager->GetPublisher()->PublishVrGameStatusEvent(curStatus, std::move(vrError));
            }
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error("{}; VR game error code: {}", ex.what(), ex.GetErrorCode());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    void ScheduleVrEventsHandler::OnGameStatusError(const std::string & error)
    {
        OnGameStatusError(vr_events::VrGameErrorCode::FAIL, error);
    }

    bool ScheduleVrEventsHandler::IsGameRunning() const
    {
        std::unique_lock<std::mutex> locker(mStatusSync);
        return mStatus == vr_events::VrGameStatus::STARTING_TUTORIAL
            || mStatus == vr_events::VrGameStatus::TUTORIAL_ON
            || mStatus == vr_events::VrGameStatus::STARTING_GAME
            || mStatus == vr_events::VrGameStatus::GAME_ON;
    }

    void ScheduleVrEventsHandler::StartGameRoutine(std::shared_ptr<vr_events::VrGameOnEvent> event)
    {
        try
        {
            mGameStopTimer->SetDuration(std::chrono::seconds(event->game().game_duration_seconds()));
            mGameStopTimer->Start();
            mGameStopTimer->RethrowExceptionIfAny();

            if (event->game().run_tutorial())
            {
                OnGameStatusChanged(vr_events::VrGameStatus::STARTING_TUTORIAL);
                auto fut = mTutorialRunner->RunAsync();
                mTutorialRunner->WaitForTutorialStart();
                OnGameStatusChanged(vr_events::VrGameStatus::TUTORIAL_ON);
                fut.get();

                if (!IsGameRunning())
                    return;
            }

            OnGameStatusChanged(vr_events::VrGameStatus::STARTING_GAME);
            mGameRunner->Run(event->game());
            OnGameStatusChanged(vr_events::VrGameStatus::GAME_ON);
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error("{}; VR game error code: {}", ex.what(), ex.GetErrorCode());
            OnGameStatusError(ex.GetErrorCode(), ex.what());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
            OnGameStatusError(ex.what());
        }
    }

    void ScheduleVrEventsHandler::StopGameRoutine()
    {
        try
        {
            if (IsGameRunning())
            {
                vr_events::VrGameStatus curStatus = GetGameStatus();
                if (curStatus == vr_events::VrGameStatus::TUTORIAL_ON ||
                    curStatus == vr_events::VrGameStatus::STARTING_TUTORIAL)
                {
                    OnGameStatusChanged(vr_events::VrGameStatus::STOPPING_GAME);
                    mTutorialRunner->Stop();
                }
                else
                {
                    OnGameStatusChanged(vr_events::VrGameStatus::STOPPING_GAME);
                    mGameRunner->Stop();
                }
                OnGameStatusChanged(vr_events::VrGameStatus::GAME_OFF);
            }
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error("{}; VR game error code: {}", ex.what(), ex.GetErrorCode());
            OnGameStatusError(ex.GetErrorCode(), ex.what());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
            OnGameStatusError(ex.what());
        }
    }

    bool ScheduleVrEventsHandler::ContinueStarting()
    {
        if (IsGameRunning())
        {
            mLogger->info("VR game is already running");
            return false;
        }
        if (mGameStopTimer->IsRunning())
        {
            mGameStopTimer->Stop();
        }
        return true;
    }
}
