#ifndef CONNECTION_H__
#define CONNECTION_H__

#include <string>
#include <memory>
#include "IPublisher.h"
#include "ISubscriber.h"
#include "MqttBrokerEndPoint.h"
#include "MqttClientEndPoint.h"
#include "IConnectionPolicy.h"

namespace mqtt
{
    class iasync_client;
    class iaction_listener;
}

namespace mqtt_transport
{
    class Connection
    {
    public:
        static std::unique_ptr<Connection> CreateInstance(
            const std::string & brokerHost,
            std::unique_ptr<IConnectionPolicy> connectionPolicy
        );
        static std::unique_ptr<Connection> CreateInstance(
            std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
            std::unique_ptr<MqttClientEndPoint> clientEndPoint,
            std::unique_ptr<IConnectionPolicy> connectionPolicy
        );

        void Connect(mqtt::iaction_listener & listener); // throws mqtt::exception
        void Reconnect(mqtt::iaction_listener & listener); // throws mqtt::exception
        void Disconnect(mqtt::iaction_listener & listener); // throws mqtt::exception
        void WaitPendingTokens(); // throws mqtt::exception

        mqtt::iasync_client & GetClient() const;
        IPublisher & GetPublisher() const;
        ISubscriber & GetSubcriber() const;
        MqttBrokerEndPoint & GetBrokerEndPoint() const;
        MqttClientEndPoint & GetClientEndPoint() const;

        ~Connection();

    private:
        Connection(
            std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
            std::unique_ptr<MqttClientEndPoint> clientEndPoint,
            std::unique_ptr<IConnectionPolicy> connectionPolicy);

        Connection(const Connection &) = delete;
        Connection & operator=(const Connection &) = delete;

        template<typename TokenType>
        void WaitPendingTokens(const std::vector<TokenType> & tokens);

    private:
        std::unique_ptr<MqttBrokerEndPoint> mBrokerEndPoint;
        std::unique_ptr<MqttClientEndPoint> mClientEndPoint;
        std::unique_ptr<IPublisher> mPublisher;
        std::unique_ptr<ISubscriber> mSubscriber;
        std::unique_ptr<IConnectionPolicy> mConnectionPolicy;

        std::unique_ptr<mqtt::iasync_client> mClient;
    };
}

#endif // CONNECTION_H__
