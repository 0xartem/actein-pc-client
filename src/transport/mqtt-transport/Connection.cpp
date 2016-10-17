#include "Connection.h"
#include "MqttBrokerEndPoint.h"
#include "MqttClientEndPoint.h"
#include "IConnectionPolicy.h"
#include "PreciseDeliveryConnectionPolicy.h"
#include "MqttPublisher.h"
#include "MqttSubscriber.h"
#include "ConnectOptionsBuilder.h"
#include <mqtt/async_client.h>

namespace mqtt_transport
{
    std::unique_ptr<Connection> Connection::CreateInstance(const std::string & brokerHost)
    {
        return std::unique_ptr<Connection>(new Connection(
            std::make_unique<MqttBrokerEndPoint>(brokerHost),
            std::make_unique<MqttClientEndPoint>(),
            std::make_unique<PreciseDeliveryConnectionPolicy>()
            ));
    }

    std::unique_ptr<Connection> Connection::CreateInstance(
        std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
        std::unique_ptr<MqttClientEndPoint> clientEndPoint,
        std::unique_ptr<IConnectionPolicy> connectionPolicy)
    {
        return std::unique_ptr<Connection>(new Connection(
            std::move(brokerEndPoint),
            std::move(clientEndPoint),
            std::move(connectionPolicy))
            );
    }

    Connection::Connection(
        std::unique_ptr<MqttBrokerEndPoint> brokerEndPoint,
        std::unique_ptr<MqttClientEndPoint> clientEndPoint,
        std::unique_ptr<IConnectionPolicy> connectionPolicy
        )
        : mBrokerEndPoint(std::move(brokerEndPoint))
        , mClientEndPoint(std::move(clientEndPoint))
        , mConnectionPolicy(std::move(connectionPolicy))
    {
        mClient = std::make_unique<mqtt::async_client>(
            mBrokerEndPoint->getEndPointUri(),
            mClientEndPoint->getClientId()
            );

        mPublisher = std::make_unique<MqttPublisher>(*mClient, *mConnectionPolicy);
        mSubscriber = std::make_unique<MqttSubscriber>(*mClient, *mConnectionPolicy);
    }

    // throws mqtt::exception
    void Connection::Connect(mqtt::iaction_listener & listener)
    {
        mqtt::connect_options_ptr connectOptions =
            ConnectOptionsBuilder::BuildConnectOptions(*mConnectionPolicy);
        mqtt::itoken_ptr token = mClient->connect(*connectOptions);
        token->set_action_callback(listener);
    }

    // throws mqtt::exception
    void Connection::Disconnect(mqtt::iaction_listener & listener)
    {
        mqtt::itoken_ptr token = mClient->disconnect();
        token->set_action_callback(listener);
    }

    mqtt::iasync_client & Connection::GetClient() const
    {
        return *mClient;
    }

    IPublisher & Connection::GetPublisher() const
    {
        return *mPublisher;
    }

    ISubscriber & Connection::GetSubcriber() const
    {
        return *mSubscriber;
    }

    MqttBrokerEndPoint & Connection::GetBrokerEndPoint() const
    {
        return *mBrokerEndPoint;
    }

    MqttClientEndPoint & Connection::GetClientEndPoint() const
    {
        return *mClientEndPoint;
    }
}