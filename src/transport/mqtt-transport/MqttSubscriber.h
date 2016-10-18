#ifndef MQTT_SUBSCRIBER_H__
#define MQTT_SUBSCRIBER_H__

#include "ISubscriber.h"
#include "IConnectionPolicy.h"

namespace mqtt
{
    class iasync_client;
}

namespace mqtt_transport
{
    class MqttSubscriber : public ISubscriber
    {
    public:
        MqttSubscriber(mqtt::iasync_client & client, IConnectionPolicy & connectionPolicy);
        void SetupCallback(std::unique_ptr<MqttSubscriberCallback> callback) override;
        void Subscribe(const std::string & topic, mqtt::iaction_listener & listener) override;
        void Subscribe(const TopicsVec & topics, mqtt::iaction_listener & listener) override;
        void Unsubscribe(const std::string & topic, mqtt::iaction_listener & listener) override;
        void Unsubscribe(const TopicsVec & topics, mqtt::iaction_listener & listener) override;

    private:
        mqtt::iasync_client & mClient;
        IConnectionPolicy & mConnectionPolicy;
        std::unique_ptr<MqttSubscriberCallback> mCallback;
    };
}

#endif //MQTT_SUBSCRIBER_H__
