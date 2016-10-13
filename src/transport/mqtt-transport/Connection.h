#ifndef CONNECTION_H__
#define CONNECTION_H__

#include <string>
#include <memory>

namespace mqtt
{
    class iasync_client;
}

namespace mqtt_transport
{
    class MqttBrokerEndPoint;
    class MqttClientEndPoint;
    class IConnectionPolicy;

    class Connection
    {
    public:
        static Connection * createInstance(const std::string & brokerHost);

    private:
        Connection(
            std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
            std::unique_ptr<MqttClientEndPoint> clientEndPoint,
            std::unique_ptr<IConnectionPolicy> connectionPolicy
            );

    private:
        std::unique_ptr<MqttBrokerEndPoint> mBrokerEndPoint;
        std::unique_ptr<MqttClientEndPoint> mClientEndPoint;
        std::unique_ptr<IConnectionPolicy> mConnectionPolicy;

        std::unique_ptr<mqtt::iasync_client> mClient;
    };
}

#endif // CONNECTION_H__
