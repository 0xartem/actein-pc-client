#ifndef MQTT_PUBLISHER_H__
#define MQTT_PUBLISHER_H__

#include "IPublisher.h"
#include "IConnectionPolicy.h"

namespace mqtt
{
    class iasync_client;
}

namespace mqtt_transport
{
    class MqttPublisher : public IPublisher
    {
    public:
        MqttPublisher(mqtt::iasync_client & client, IConnectionPolicy & connectionPolicy);

        void Publish(
            const std::string & topic,
            const google::protobuf::MessageLite & message,
            mqtt::iaction_listener & listener) override;

    private:
        mqtt::iasync_client & mClient;
        IConnectionPolicy & mConnectionPolicy;
    };
}

#endif //MQTT_PUBLISHER_H__
