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
            }
            else
            {
                mLogger->info("MQTT client disconnected gracefully");
            }
            if (mConnectionObserver != nullptr)
            {
                mConnectionObserver->OnConnectionLost();
            }
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    void MqttSubscriberCallback::connect_complete(const std::string& cause)
    {
        try
        {
            if (cause == "connect onSuccess called")
            {
                mLogger->info("MQTT client connected successfully");
                if (mConnectionObserver != nullptr)
                {
                    mConnectionObserver->OnConnected();
                }
            }
            else if (cause == "automatic reconnect")
            {
                mLogger->info("MQTT client reconnected successfully");
                if (mConnectionObserver != nullptr)
                {
                    mConnectionObserver->OnConnected();
                }
            }
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
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