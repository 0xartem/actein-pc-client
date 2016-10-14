#ifndef ACTION_H__
#define ACTION_H__

namespace mqtt_transport
{
    enum class Action
    {
        CONNECT,
        DISCONNECT,
        SUBSCRIBE,
        UNSUBSCRIBE,
        PUBLISH
    };
}

#endif //ACTION_H__
