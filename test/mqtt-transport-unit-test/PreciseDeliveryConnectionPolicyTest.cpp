#include <gtest/gtest.h>
#include <ArrayUtils.h>
#include <PreciseDeliveryConnectionPolicy.h>

namespace mqtt_transport_test
{
    TEST(PreciseDeliveryConnectionPolicyTest, DefaultValues)
    {
        mqtt_transport::PreciseDeliveryConnectionPolicy policy(1);

        ASSERT_FALSE(policy.isPersistentSession());
        ASSERT_EQ(policy.getKeepAliveInterval(), 30);
        ASSERT_EQ(policy.getConnectionTimeout(), 15);

        ASSERT_EQ(policy.getQualityOfService(), 2);
        ASSERT_TRUE(policy.shouldRetainMessages());

        ASSERT_TRUE(policy.shouldUseLastWill());
        ASSERT_STREQ(policy.getLastWillTopic().c_str(), u8"factory/booths/1/pc/status");
        mqtt_transport::OnlineStatusEvent event;
        event.set_status(mqtt_transport::OnlineStatus::OFFLINE);
        ASSERT_TRUE(test_utils::ArraysMatch<std::string>(event.SerializeAsString(), policy.getLastWillPayload()));
    }
}