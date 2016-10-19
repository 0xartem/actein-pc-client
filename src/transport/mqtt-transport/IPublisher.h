#ifndef I_PUBLISHER_H__
#define I_PUBLISHER_H__

#include <string>

namespace google
{
    namespace protobuf
    {
        class MessageLite;
    }
}

namespace mqtt
{
    class iaction_listener;
}

namespace mqtt_transport
{
    class IPublisher
    {
    public:
        // throws mqtt::exception
        virtual void Publish(
            const std::string & topic,
            const google::protobuf::MessageLite & message,
            mqtt::iaction_listener & listener) = 0;

        // throws mqtt::exception
        virtual void Publish(
            const std::string & topic,
            const google::protobuf::MessageLite & message,
            mqtt::iaction_listener & listener,
            bool retained) = 0;

        virtual ~IPublisher() = default;
    };
}

#endif //I_PUBLISHER_H__