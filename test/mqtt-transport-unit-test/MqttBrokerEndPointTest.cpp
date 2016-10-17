#include <gtest/gtest.h>
#include "MqttBrokerEndPoint.h"

namespace mqtt_transport_test
{
    TEST(MqttBrokerEndPointTest, TCP_Standard_MQTT_Port)
    {
        mqtt_transport::MqttBrokerEndPoint brokerEndPoint("123.456.789.012");
        ASSERT_FALSE(brokerEndPoint.isTlsEndPoint());
        ASSERT_EQ(1883, brokerEndPoint.getPort());
        ASSERT_STREQ("123.456.789.012", brokerEndPoint.getBrokerHost().c_str());
        ASSERT_STREQ("tcp://123.456.789.012:1883", brokerEndPoint.getEndPointUri().c_str());
    }

    TEST(MqttBrokerEndPointTest, SSL_Standard_MQTT_SSLPort)
    {
        mqtt_transport::MqttBrokerEndPoint brokerEndPoint(
            "iot.eclipse.org",
            mqtt_transport::MqttBrokerEndPoint::STANDARD_MQTT_SSL_PORT,
            true
        );

        ASSERT_TRUE(brokerEndPoint.isTlsEndPoint());
        ASSERT_EQ(8883, brokerEndPoint.getPort());
        ASSERT_STREQ("iot.eclipse.org", brokerEndPoint.getBrokerHost().c_str());
        ASSERT_STREQ("ssl://iot.eclipse.org:8883", brokerEndPoint.getEndPointUri().c_str());
    }
}