#ifndef I_SUBSCRIBER_H__
#define I_SUBSCRIBER_H__

#include <string>
#include <vector>

namespace mqtt
{
    class callback;
    class iaction_listener;
}

namespace mqtt_transport
{
    using TopicsVec = std::vector<std::string>;

    class ISubscriber
    {
    public:
        virtual void SetupCallback(mqtt::callback & callback) = 0;
        
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