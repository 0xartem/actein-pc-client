#include <gtest/gtest.h>
#include "CommonActionListener.h"

namespace mqtt_transport_test
{
    TEST(CommonActionListenerTest, ConnectActionTest)
    {
        mqtt_transport::CommonActionListener listener(mqtt_transport::Action::CONNECT, nullptr);
    }
}