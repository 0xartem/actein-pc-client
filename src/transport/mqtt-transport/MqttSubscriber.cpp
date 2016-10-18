#include "MqttSubscriber.h"
#include "MqttSubscriberCallback.h"
#include <mqtt/async_client.h>

namespace mqtt_transport
{
    MqttSubscriber::MqttSubscriber(
        mqtt::iasync_client & client,
        IConnectionPolicy & connectionPolicy)
        : mClient(client)
        , mConnectionPolicy(connectionPolicy)
    {
    }

    void MqttSubscriber::SetupCallback(std::unique_ptr<MqttSubscriberCallback> callback)
    {
        mCallback = std::move(callback);
        mClient.set_callback(*mCallback);
    }

    void MqttSubscriber::Subscribe(const std::string & topic, mqtt::iaction_listener & listener)
    {
        mqtt::itoken_ptr token = mClient.subscribe(topic, mConnectionPolicy.getQualityOfService());
        token->set_action_callback(listener);
    }

    void MqttSubscriber::Subscribe(const TopicsVec & topics, mqtt::iaction_listener & listener)
    {
        std::vector<int> qosVec(topics.size(), mConnectionPolicy.getQualityOfService());
        mqtt::itoken_ptr token = mClient.subscribe(topics, qosVec);
        token->set_action_callback(listener);
    }

    void MqttSubscriber::Unsubscribe(const std::string & topic, mqtt::iaction_listener & listener)
    {
        mqtt::itoken_ptr token = mClient.unsubscribe(topic);
        token->set_action_callback(listener);
    }

    void MqttSubscriber::Unsubscribe(const TopicsVec & topics, mqtt::iaction_listener & listener)
    {
        mqtt::itoken_ptr token = mClient.unsubscribe(topics);
        token->set_action_callback(listener);
    }
}