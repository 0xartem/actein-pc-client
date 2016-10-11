#include "Connection.h"
#include "MqttBrokerEndPoint.h"
#include "MqttClientEndPoint.h"
#include "IConnectionPolicy.h"
#include "PreciseDeliveryConnectionPolicy.h"
#include <mqtt/async_client.h>

namespace transport
{
    Connection * Connection::createInstance(const std::string & brokerHost)
    {
        return new Connection(
            std::make_unique<MqttBrokerEndPoint>(brokerHost),
            std::make_unique<MqttClientEndPoint>(),
            std::make_unique<PreciseDeliveryConnectionPolicy>()
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
        mClient.reset(new mqtt::async_client(
            mBrokerEndPoint->getEndPointUri(),
            mClientEndPoint->getClientId())
            );
    }
}