#ifndef I_SUBSCRIBER_H__
#define I_SUBSCRIBER_H__

#include <string>
#include <vector>
#include <memory>

namespace mqtt
{
    class iaction_listener;
}

using TopicsVec = std::vector<std::string>;

namespace mqtt_transport
{
    class MqttSubscriberCallback;

    class ISubscriber
    {
    public:
        virtual void SetupCallback(std::unique_ptr<MqttSubscriberCallback> callback) = 0;
        
        // throws mqtt::exception
        virtual void Subscribe(const std::string & topic, mqtt::iaction_listener & listener) = 0;
        
        // throws mqtt::exception
        virtual void Subscribe(const TopicsVec & topics, mqtt::iaction_listener & listener) = 0;
        
        // throws mqtt::exception
        virtual void Unsubscribe(const std::string & topic, mqtt::iaction_listener & listener) = 0;
        
        // throws mqtt::exception
        virtual void Unsubscribe(const TopicsVec & topics, mqtt::iaction_listener & listener) = 0;

        virtual ~ISubscriber() = default;
    };
}

#endif //I_SUBSCRIBER_H__