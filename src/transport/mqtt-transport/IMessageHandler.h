#ifndef I_MESSAGE_HANDLER_H__
#define I_MESSAGE_HANDLER_H__

#include <string>
#include <mqtt/message.h>

namespace mqtt_transport
{
    class IMessageHandler
    {
    public:
        virtual void HandleMessage(const std::string & topic, mqtt::message_ptr message) = 0;
        virtual ~IMessageHandler() = default;
    };
}

#endif //I_MESSAGE_HANDLER_H__
