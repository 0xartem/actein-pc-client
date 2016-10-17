#include <gtest/gtest.h>
#include "QualityOfService.h"

using namespace mqtt_transport;

namespace mqtt_transport_test
{
    TEST(QualityOfServiceTest, ConversionTest)
    {
        ASSERT_EQ(0, convertToMqttQosValues(QualityOfService::AT_MOST_ONCE_DELIVERY_GUARANTEE));
        ASSERT_EQ(1, convertToMqttQosValues(QualityOfService::AT_LEAST_ONCE_DELIVERY_GUARANTEE));
        ASSERT_EQ(2, convertToMqttQosValues(QualityOfService::EXACTLY_ONCE_DELIVERY_GUARANTEE));
    }
}