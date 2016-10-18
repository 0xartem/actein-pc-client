#include "MqttVrEventsManager.h"
#include "Connection.h"
#include "IConnectionObserver.h"
#include "gen/vr_booth_info.pb.h"
#include "MqttVrEventsPublisher.h"
#include "MqttVrEventsSubscriber.h"

namespace vr_events
{
    MqttVrEventsManager::MqttVrEventsManager(
        mqtt_transport::Connection & connection,
        const std::shared_ptr<VrBoothInfo> & vrBoothInfo)
        : mConnection(connection)
        , mVrBoothInfo(vrBoothInfo)
        , mIsRunning(false)
    {
    }

    void MqttVrEventsManager::Start(
        IVrEventsHandler * vrEventsHandler,
        mqtt_transport::IConnectionObserver * connectionObserver,
        mqtt_transport::IActionStatusObserver * actionObserver)
    {
        mVrEventsPublisher = std::make_unique<MqttVrEventsPublisher>(
            mConnection.GetPublisher(),
            mVrBoothInfo,
            actionObserver
            );

        mVrEventsSubscriber = std::make_unique<MqttVrEventsSubscriber>(
            mConnection.GetSubcriber(),
            mVrBoothInfo,
            vrEventsHandler,
            connectionObserver,
            actionObserver
            );

        mIsRunning = true;
    }

    void MqttVrEventsManager::Stop()
    {
        mIsRunning = false;
        mVrEventsSubscriber.reset();
        mVrEventsPublisher.reset();
    }

    bool MqttVrEventsManager::IsRunning() const
    {
        return mIsRunning;
    }

    IVrEventsPublisher & MqttVrEventsManager::GetPublisher() const
    {
        return *mVrEventsPublisher;
    }

    IVrEventsSubscriber & MqttVrEventsManager::GetSubscriber() const
    {
        return *mVrEventsSubscriber;
    }
}