#include "MqttBrokerEndPoint.h"

namespace mqtt_transport
{
    MqttBrokerEndPoint::MqttBrokerEndPoint(
        const std::string & brokerHost,
        int port /* = STANDARD_MQTT_PORT */,
        bool tlsEndpoint /* = false */
        )
    {
        mBrokerHost = brokerHost;
        mPort = port;
        mTlsEndpoint = tlsEndpoint;
    }

    std::string MqttBrokerEndPoint::getEndPointUri() const
    {
        std::string host = mBrokerHost + ":" + std::to_string(mPort);
        if (mTlsEndpoint)
        {
            return "ssl://" + host;
        }
        return "tcp://" + host;
    }

    std::string MqttBrokerEndPoint::getBrokerHost() const
    {
        return mBrokerHost;
    }

    int MqttBrokerEndPoint::getPort() const
    {
        return mPort;
    }

    bool MqttBrokerEndPoint::isTlsEndPoint() const
    {
        return mTlsEndpoint;
    }
}