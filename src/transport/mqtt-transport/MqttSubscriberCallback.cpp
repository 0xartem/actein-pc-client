#include "MqttSubscriberCallback.h"
#include <spdlog/spdlog.h>

namespace mqtt_transport
{
    MqttSubscriberCallback::MqttSubscriberCallback(
        IMessageHandler * messageHandler,
        IConnectionObserver * connectionObserver)
        : mMessageHandler(messageHandler)
        , mConnectionObserver(connectionObserver)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    void MqttSubscriberCallback::connection_lost(const std::string& cause)
    {
        try
        {
            if (!cause.empty())
            {
                mLogger->error("MQTT connection lost: {}", cause);
                if (mConnectionObserver != nullptr)
                {
                    mConnectionObserver->OnConnectionLost();
                }
            }
            else
            {
                mLogger->info("MQTT client disconnected gracefully");
            }
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    void MqttSubscriberCallback::message_arrived(const std::string& topic, mqtt::message_ptr msg)
    {
        try
        {
            mLogger->info("Message arrived: {}", topic);
            if (mMessageHandler != nullptr)
            {
                mMessageHandler->HandleMessage(topic, msg);
            }
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    void MqttSubscriberCallback::delivery_complete(mqtt::idelivery_token_ptr tok)
    {
    }
}