#ifndef CONNECTION_H__
#define CONNECTION_H__

#include <string>
#include <memory>
#include "IPublisher.h"
#include "ISubscriber.h"

namespace mqtt
{
    class iasync_client;
    class iaction_listener;
}

namespace mqtt_transport
{
    class MqttBrokerEndPoint;
    class MqttClientEndPoint;
    class MqttPublisher;
    class MqttSubscriber;
    class IConnectionPolicy;

    class Connection
    {
    public:
        static std::unique_ptr<Connection> CreateInstance(const std::string & brokerHost);
        static std::unique_ptr<Connection> CreateInstance(
            std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
            std::unique_ptr<MqttClientEndPoint> clientEndPoint,
            std::unique_ptr<IConnectionPolicy> connectionPolicy);

        // throws mqtt::exception
        void Connect(mqtt::iaction_listener & listener);
        // throws mqtt::exception
        void Disconnect(mqtt::iaction_listener & listener);

        mqtt::iasync_client & GetClient() const;
        IPublisher & GetPublisher() const;
        ISubscriber & GetSubcriber() const;
        MqttBrokerEndPoint & GetBrokerEndPoint() const;
        MqttClientEndPoint & GetClientEndPoint() const;

    private:
        Connection(
            std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
            std::unique_ptr<MqttClientEndPoint> clientEndPoint,
            std::unique_ptr<IConnectionPolicy> connectionPolicy);

        Connection(const Connection &) = delete;
        Connection & operator=(const Connection &) = delete;

    private:
        std::unique_ptr<MqttBrokerEndPoint> mBrokerEndPoint;
        std::unique_ptr<MqttClientEndPoint> mClientEndPoint;
        std::unique_ptr<MqttPublisher> mPublisher;
        std::unique_ptr<MqttSubscriber> mSubscriber;
        std::unique_ptr<IConnectionPolicy> mConnectionPolicy;

        std::unique_ptr<mqtt::iasync_client> mClient;
    };
}

#endif // CONNECTION_H__
