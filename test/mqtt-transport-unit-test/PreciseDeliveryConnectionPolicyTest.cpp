#include <gtest/gtest.h>
#include <ArrayUtils.h>
#include <PreciseDeliveryConnectionPolicy.h>

namespace mqtt_transport_test
{
    TEST(PreciseDeliveryConnectionPolicyTest, DefaultValues)
    {
        mqtt_transport::PreciseDeliveryConnectionPolicy policy;

        ASSERT_FALSE(policy.isPersistentSession());
        ASSERT_EQ(policy.getKeepAliveInterval(), 60);
        ASSERT_EQ(policy.getConnectionTimeout(), 30);

        ASSERT_EQ(policy.getQualityOfService(), 2);
        ASSERT_TRUE(policy.shouldRetainMessages());

        ASSERT_TRUE(policy.shouldUseLastWill());
        ASSERT_STREQ(policy.getLastWillTopic().c_str(), mqtt_transport::Topics::PC_ONLINE_STATUS.c_str());
        ASSERT_TRUE(test_utils::ArraysMatch<std::string>("offline", policy.getLastWillPayload()));
    }
}