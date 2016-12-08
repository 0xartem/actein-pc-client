#include "ConnectOptionsBuilder.h"

namespace mqtt_transport
{
    mqtt::connect_options_ptr ConnectOptionsBuilder::BuildConnectOptions(IConnectionPolicy & connectionPolicy)
    {
        mqtt::connect_options_ptr connectOptions = std::make_shared<mqtt::connect_options>();
        connectOptions->set_clean_session(!connectionPolicy.isPersistentSession());

        if (connectionPolicy.shouldUseLastWill())
        {
            std::string lastWillPayload = connectionPolicy.getLastWillPayload();
            connectOptions->set_will(
                connectionPolicy.getLastWillTopic(),
                (void *) lastWillPayload.data(),
                lastWillPayload.size(),
                connectionPolicy.getQualityOfService(),
                connectionPolicy.shouldRetainMessages()
            );
        }

        connectOptions->set_keep_alive_interval(connectionPolicy.getKeepAliveInterval());
        connectOptions->set_connection_timeout(connectionPolicy.getConnectionTimeout());

        //connectOptions->set_automatic_reconnect(connectionPolicy.isAutomaticReconnect());
        /*if (connectionPolicy.isAutomaticReconnect())
        {
            connectOptions->set_min_reconnect_interval(connectionPolicy.getMinReconnectInterval());
            connectOptions->set_max_reconnect_interval(connectionPolicy.getMaxReconnectInterval());
        }*/

        return connectOptions;
    }
}