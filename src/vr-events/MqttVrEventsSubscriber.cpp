#include <mqtt/exception.h>
#include <spdlog/spdlog.h>
#include "MqttVrEventsSubscriber.h"
#include "MqttSubscriberCallback.h"
#include "ISubscriber.h"
#include "CommonActionListener.h"
#include "gen/vr_booth_info.pb.h"
#include "gen/vr_game_on_event.pb.h"
#include "gen/vr_game_off_event.pb.h"
#include "gen/vr_game_status_event.pb.h"
#include "VrEventsException.h"
#include "VrTopicBuilder.h"
#include "IVrEventsHandler.h"

using MqttCallback = mqtt_transport::MqttSubscriberCallback;

namespace vr_events
{
    MqttVrEventsSubscriber::MqttVrEventsSubscriber(
        mqtt_transport::ISubscriber & subscriber,
        const std::shared_ptr<VrBoothInfo> & vrBoothInfo,
        IVrEventsHandler * vrEventsHandler,
        mqtt_transport::IConnectionObserver * connectionObserver,
        mqtt_transport::IActionStatusObserver * actionObserver)
        : mSubscriber(subscriber)
        , mVrBoothInfo(vrBoothInfo)
        , mVrEventsHandler(vrEventsHandler)
        , mSubscribeListener(new mqtt_transport::CommonActionListener(
            mqtt_transport::Action::SUBSCRIBE, actionObserver
        ))
        , mUnsubscribeListener(new mqtt_transport::CommonActionListener(
            mqtt_transport::Action::UNSUBSCRIBE, actionObserver
        ))
    {
        std::unique_ptr<MqttCallback> callback = std::make_unique<MqttCallback>(
            this, connectionObserver
            );
        mSubscriber.SetupCallback(std::move(callback));

        VrTopicBuilder topicBuilder;
        mAllVrEventsTopic = topicBuilder.SetToAll().SetBoothId(mVrBoothInfo->id()).Build();
        mGameStatusVrTopic = topicBuilder.SetToGameStatus().SetBoothId(mVrBoothInfo->id()).Build();
        mGameOnVrTopic = topicBuilder.SetToGameOn().SetBoothId(mVrBoothInfo->id()).Build();
        mGameOffVrTopic = topicBuilder.SetToGameOff().SetBoothId(mVrBoothInfo->id()).Build();
    }

    void MqttVrEventsSubscriber::SubscribeToAll()
    {
        try
        {
            mSubscriber.Subscribe(mAllVrEventsTopic, *mSubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not subscribe to all vr events");
        }
    }

    void MqttVrEventsSubscriber::UnsubscribeFromAll()
    {
        try
        {
            mSubscriber.Unsubscribe(mAllVrEventsTopic, *mUnsubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not unsubscribe from all vr events");
        }
    }

    void MqttVrEventsSubscriber::SubscribeToGameOnEvent()
    {
        try
        {
            mSubscriber.Subscribe(mGameOnVrTopic, *mSubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not subscribe to game on vr event");
        }
    }

    void MqttVrEventsSubscriber::UnsubscribeFromGameOnEvent()
    {
        try
        {
            mSubscriber.Unsubscribe(mGameOnVrTopic, *mUnsubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not unsubscribe from game off vr event");
        }
    }

    void MqttVrEventsSubscriber::SubscribeToGameOffEvent()
    {
        try
        {
            mSubscriber.Subscribe(mGameOffVrTopic, *mSubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not subscribe to game off vr event");
        }
    }

    void MqttVrEventsSubscriber::UnsubscribeFromGameOffEvent()
    {
        try
        {
            mSubscriber.Unsubscribe(mGameOffVrTopic, *mUnsubscribeListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not unsubscribe from game off vr event");
        }
    }

    void MqttVrEventsSubscriber::HandleMessage(const std::string & topic, mqtt::message_ptr message)
    {
        if (topic == mGameOnVrTopic)
        {
            ProcessGameOnEvent(message);
        }
        else if (topic == mGameOffVrTopic)
        {
            ProcessGameOffEvent(message);
        }
        else if (topic == mGameStatusVrTopic)
        {
            ProcessVrStatusEvent(message);
        }
        else
        {
            throw std::logic_error("Unknown vr event message type");
        }
    }

    void MqttVrEventsSubscriber::ProcessGameOnEvent(mqtt::message_ptr message)
    {
        auto event = std::make_shared<VrGameOnEvent>();
        if (!event->ParseFromString(message->get_payload()))
        {
            mLogger->error("Can not parse vr game on event");
        }
        if (mVrEventsHandler != nullptr)
        {
            mVrEventsHandler->HandleVrGameOnEvent(event);
        }
    }

    void MqttVrEventsSubscriber::ProcessGameOffEvent(mqtt::message_ptr message)
    {
        auto event = std::make_shared<VrGameOffEvent>();
        if (!event->ParseFromString(message->get_payload()))
        {
            mLogger->error("Can not parse vr game off event");
        }
        if (mVrEventsHandler != nullptr)
        {
            mVrEventsHandler->HandleVrGameOffEvent(event);
        }
    }

    void MqttVrEventsSubscriber::ProcessVrStatusEvent(mqtt::message_ptr message)
    {
        auto event = std::make_shared<VrGameStatusEvent>();
        if (!event->ParseFromString(message->get_payload()))
        {
            mLogger->error("Can not parse vr game status event");
        }
        if (mVrEventsHandler != nullptr)
        {
            mVrEventsHandler->HandleVrGameStatusEvent(event);
        }
    }
}