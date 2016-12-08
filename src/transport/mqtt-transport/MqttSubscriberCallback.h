#ifndef MQTT_SUBSCRIBER_CALLBACK_H__
#define MQTT_SUBSCRIBER_CALLBACK_H__

#include <mqtt/callback.h>
#include <mqtt/delivery_token.h>
#include "IMessageHandler.h"
#include "IConnectionObserver.h"

namespace spdlog
{
    class logger;
}

namespace mqtt_transport
{
    class MqttSubscriberCallback : public mqtt::callback
    {
    public:
        MqttSubscriberCallback(IMessageHandler * messageHandler, IConnectionObserver * connectionObserver);
        void connection_lost(const std::string& cause) override;
        void message_arrived(const std::string& topic, mqtt::message_ptr msg) override;
        void delivery_complete(mqtt::idelivery_token_ptr tok) override;
        void connect_complete(const std::string& cause) override;

    private:
        IMessageHandler * mMessageHandler;
        IConnectionObserver * mConnectionObserver;
        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //MQTT_SUBSCRIBER_CALLBACK_H__
