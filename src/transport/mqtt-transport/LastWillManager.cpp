#include <spdlog/spdlog.h>
#include "gen/online_status_event.pb.h"
#include "Connection.h"
#include "CommonActionListener.h"
#include "MqttSubscriber.h"
#include "MqttPublisher.h"
#include "LastWillManager.h"
#include "IEmbDeviceOnlineStatusHandler.h"
#include "TopicBuilder.h"

namespace mqtt_transport
{
    LastWillManager::LastWillManager(
        mqtt_transport::Connection & connection,
        IActionStatusObserver * actionObserver,
        IEmbDeviceOnlineStatusHandler * embDeviceStatusHandler,
        int boothId)
        : mPublisher(connection.GetPublisher())
        , mSubscriber(connection.GetSubcriber())
        , mEmbDeviceStatusHandler(embDeviceStatusHandler)
        , mPublishListener(new CommonActionListener(
            Action::PUBLISH, actionObserver
        ))
        , mSubscribeListener(new CommonActionListener(
            Action::SUBSCRIBE, actionObserver
        ))
        , mUnsubscribeListener(new CommonActionListener(
            Action::UNSUBSCRIBE, actionObserver
        ))
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
        mPcLastWillTopic = TopicBuilder::BuildPcLastWill(boothId);
        mEmbDeviceLastWillTopic = TopicBuilder::BuildEmbDeviceLastWill(boothId);
    }

    void LastWillManager::Start()
    {
        std::unique_lock<std::mutex> locker(mSync);
        mIsRunning = true;
        this->SubscribeToEmbDeviceLastWill();
        this->PublishPcOnlineStatus(true);
    }

    void LastWillManager::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);
        this->UnsubscribeFromEmbDeviceLastWill();
        this->PublishPcOnlineStatus(false);
        mIsRunning = false;
    }

    bool LastWillManager::IsRunning() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mIsRunning;
    }

    void LastWillManager::PublishPcOnlineStatus(bool online)
    {
        OnlineStatusEvent event;
        event.set_status(online ? OnlineStatus::ONLINE : OnlineStatus::OFFLINE);
        mPublisher.Publish(mPcLastWillTopic, event, *mPublishListener);
    }

    void LastWillManager::SubscribeToEmbDeviceLastWill()
    {
        mSubscriber.Subscribe(mEmbDeviceLastWillTopic, *mSubscribeListener);
    }

    void LastWillManager::UnsubscribeFromEmbDeviceLastWill()
    {
        mSubscriber.Unsubscribe(mEmbDeviceLastWillTopic, *mUnsubscribeListener);
    }

    void LastWillManager::HandleMessage(const std::string & topic, mqtt::message_ptr message)
    {
        if (topic == mEmbDeviceLastWillTopic)
        {
            ProcessOnlineStatusEvent(message);
        }
    }

    void LastWillManager::ProcessOnlineStatusEvent(mqtt::message_ptr message)
    {
        auto event = std::make_shared<OnlineStatusEvent>();
        if (!event->ParseFromString(message->get_payload()))
        {
            mLogger->error("Can not parse online status event");
            return;
        }

        if (mEmbDeviceStatusHandler != nullptr)
        {
            mEmbDeviceStatusHandler->OnEmbDeviceOnlineStatusChanged(event->status());
        }
    }
}