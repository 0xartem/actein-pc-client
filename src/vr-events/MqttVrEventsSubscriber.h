#ifndef MQTT_VR_EVENTS_SUBSCRIBER_H__
#define MQTT_VR_EVENTS_SUBSCRIBER_H__

#include <string>
#include <memory>
#include "IVrEventsSubscriber.h"
#include "IMessageHandler.h"

namespace spdlog
{
    class logger;
}

namespace mqtt_transport
{
    class ISubscriber;
    class IActionStatusObserver;
    class IConnectionObserver;
    class CommonActionListener;
}

namespace vr_events
{
    class IVrEventsHandler;
    class VrBoothInfo;

    class MqttVrEventsSubscriber : public IVrEventsSubscriber, public mqtt_transport::IMessageHandler
    {
    public:
        MqttVrEventsSubscriber(
            mqtt_transport::ISubscriber & subscriber,
            const std::shared_ptr<VrBoothInfo> & vrBoothInfo,
            IVrEventsHandler * vrEventsHandler,
            mqtt_transport::IActionStatusObserver * actionObserver
        );

        // IVrEventsSubscriber
        void SubscribeToAll() override;
        void UnsubscribeFromAll() override;

        void SubscribeToGameOnEvent() override;
        void UnsubscribeFromGameOnEvent() override;

        void SubscribeToGameOffEvent() override;
        void UnsubscribeFromGameOffEvent() override;

        // mqtt_transport::IMessageHandler
        void HandleMessage(const std::string & topic, mqtt::message_ptr message) override;

    private:
        void ProcessGameOnEvent(mqtt::message_ptr message);
        void ProcessGameOffEvent(mqtt::message_ptr message);
        void ProcessVrStatusEvent(mqtt::message_ptr message);

    private:
        std::string mAllVrEventsTopic;
        std::string mGameStatusVrTopic;
        std::string mGameOnVrTopic;
        std::string mGameOffVrTopic;

        IVrEventsHandler * mVrEventsHandler;
        mqtt_transport::ISubscriber & mSubscriber;

        std::unique_ptr<mqtt_transport::CommonActionListener> mSubscribeListener;
        std::unique_ptr<mqtt_transport::CommonActionListener> mUnsubscribeListener;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //MQTT_VR_EVENTS_SUBSCRIBER_H__
