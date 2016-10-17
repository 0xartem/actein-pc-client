#ifndef I_CONNECTION_OBSERVER_H__
#define I_CONNECTION_OBSERVER_H__

namespace mqtt_transport
{
    class IConnectionObserver
    {
    public:
        virtual void OnConnectionLost() = 0;
        virtual ~IConnectionObserver() = default;
    };
}

#endif //I_CONNECTION_OBSERVER_H__
