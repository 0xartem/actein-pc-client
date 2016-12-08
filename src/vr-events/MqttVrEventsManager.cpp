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
        IVrEventsHandler * vrEventsHandler,
        mqtt_transport::IActionStatusObserver * actionObserver,
        const std::shared_ptr<VrBoothInfo> & vrBoothInfo
        ) : mIsRunning(false)
    {
        mVrEventsPublisher = std::make_unique<MqttVrEventsPublisher>(
            connection.GetPublisher(),
            vrBoothInfo,
            actionObserver
            );

        mVrEventsSubscriber = std::make_unique<MqttVrEventsSubscriber>(
            connection.GetSubcriber(),
            vrBoothInfo,
            vrEventsHandler,
            actionObserver
            );
    }

    void MqttVrEventsManager::Start()
    {
        std::unique_lock<std::mutex> locker(mSync);
        mIsRunning = true;
        mVrEventsSubscriber->SubscribeToGameOnEvent();
        mVrEventsSubscriber->SubscribeToGameOffEvent();
    }

    void MqttVrEventsManager::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);
        mVrEventsSubscriber->UnsubscribeFromGameOffEvent();
        mVrEventsSubscriber->UnsubscribeFromGameOnEvent();
        mIsRunning = false;
    }

    bool MqttVrEventsManager::IsRunning() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mIsRunning;
    }

    IVrEventsPublisher * MqttVrEventsManager::GetPublisher() const
    {
        return mVrEventsPublisher.get();
    }

    IVrEventsSubscriber * MqttVrEventsManager::GetSubscriber() const
    {
        return mVrEventsSubscriber.get();
    }

    mqtt_transport::IMessageHandler * MqttVrEventsManager::GetMessageHandler() const
    {
        return mVrEventsSubscriber.get();
    }
}