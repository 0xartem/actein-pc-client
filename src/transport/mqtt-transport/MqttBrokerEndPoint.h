#ifndef MQTT_BROKER_END_POINT_H__
#define MQTT_BROKER_END_POINT_H__

#include <string>

namespace mqtt_transport
{
    class MqttBrokerEndPoint
    {
    public:
        explicit MqttBrokerEndPoint(
            const std::string & brokerHost,
            int port = STANDARD_MQTT_PORT,
            bool tlsEndpoint = false
            );

        std::string getEndPointUri() const;
        std::string getBrokerHost() const;
        int getPort() const;
        bool isTlsEndPoint() const;

    public:
        static const int STANDARD_MQTT_PORT = 1883;
        static const int STANDARD_MQTT_SSL_PORT = 8883;

    private:
        std::string mBrokerHost;
        int mPort;
        bool mTlsEndpoint;
    };
}

#endif //MQTT_BROKER_END_POINT_H__
