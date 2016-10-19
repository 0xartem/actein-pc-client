#include "MqttPublisher.h"
#include <mqtt/async_client.h>
#include <google/protobuf/message_lite.h>

namespace mqtt_transport
{
    MqttPublisher::MqttPublisher(
        mqtt::iasync_client & client,
        IConnectionPolicy & connectionPolicy)
        : mClient(client)
        , mConnectionPolicy(connectionPolicy)
    {
    }

    void MqttPublisher::Publish(
        const std::string & topic,
        const google::protobuf::MessageLite & message,
        mqtt::iaction_listener & listener)
    {
        std::string messageData = message.SerializeAsString();
        //TODO: check empty string behavior
        mqtt::idelivery_token_ptr token = mClient.publish(
            topic,
            messageData.data(),
            messageData.size(),
            mConnectionPolicy.getQualityOfService(),
            mConnectionPolicy.shouldRetainMessages()
        );

        token->set_action_callback(listener);
    }

    void MqttPublisher::Publish(
        const std::string & topic,
        const google::protobuf::MessageLite & message,
        mqtt::iaction_listener & listener,
        bool retained)
    {
        std::string messageData = message.SerializeAsString();
        //TODO: check empty string behavior
        mqtt::idelivery_token_ptr token = mClient.publish(
            topic,
            messageData.data(),
            messageData.size(),
            mConnectionPolicy.getQualityOfService(),
            retained
        );

        token->set_action_callback(listener);
    }
}