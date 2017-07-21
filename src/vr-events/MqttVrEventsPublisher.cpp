#include <mqtt/exception.h>
#include <SpdLocalLog.h>
#include "gen/vr_booth_info.pb.h"
#include "gen/vr_game_on_event.pb.h"
#include "gen/vr_game_off_event.pb.h"
#include "IPublisher.h"
#include "CommonActionListener.h"
#include "MqttVrEventsPublisher.h"
#include "VrEventsException.h"
#include "VrTopicBuilder.h"

namespace vr_events
{
    MqttVrEventsPublisher::MqttVrEventsPublisher(
        mqtt_transport::IPublisher & publisher,
        const std::shared_ptr<VrBoothInfo> & vrBoothInfo,
        mqtt_transport::IActionStatusObserver * actionObserver)
        : mPublisher(publisher)
        , mVrBoothInfo(vrBoothInfo)
        , mPublishListener(new mqtt_transport::CommonActionListener(
            mqtt_transport::Action::PUBLISH, actionObserver
        ))
    {
        mLogger = spdlog::get(COMMON_LOGGER_NAME);
    }

    void MqttVrEventsPublisher::PublishVrGameOnEvent(std::unique_ptr<VrGame> vrGame)
    {
        try
        {
            VrGameOnEvent event;
            event.set_allocated_game(vrGame.get());
            vrGame.release();

            event.mutable_vr_booth_info()->set_id(mVrBoothInfo->id());

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameOn().SetBoothId(mVrBoothInfo->id()).Build();

            mPublisher.Publish(topic, event, *mPublishListener, false);
        }
        catch (const mqtt::exception & ex)
        {
            LOG_ERROR_WITH_ERROR_CODE(mLogger, ex.what(), "Mqtt Paho error code", ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not publish vr game turn on event.");
        }
    }

    void MqttVrEventsPublisher::PublishVrGameOffEvent()
    {
        try
        {
            VrGameOffEvent event;
            event.mutable_vr_booth_info()->set_id(mVrBoothInfo->id());

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameOff().SetBoothId(mVrBoothInfo->id()).Build();

            mPublisher.Publish(topic, event, *mPublishListener, false);
        }
        catch (const mqtt::exception & ex)
        {
            LOG_ERROR_WITH_ERROR_CODE(mLogger, ex.what(), "Mqtt Paho error code", ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not publish vr game turn off event.");
        }
    }

    void MqttVrEventsPublisher::PublishVrGameStatusEvent(const VrGameStatus & status)
    {
        PublishVrGameStatusEvent(status, std::unique_ptr<VrGameError>());
    }

    void MqttVrEventsPublisher::PublishVrGameStatusEvent(
        const VrGameStatus & status,
        std::unique_ptr<VrGameError> error)
    {
        try
        {
            VrGameStatusEvent event;
            event.set_status(status);

            if (error.get() != nullptr)
            {
                event.set_allocated_error(error.get());
                error.release();
            }

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameStatus().SetBoothId(mVrBoothInfo->id()).Build();

            mPublisher.Publish(topic, event, *mPublishListener);
        }
        catch (const mqtt::exception & ex)
        {
            LOG_ERROR_WITH_ERROR_CODE(mLogger, ex.what(), "Mqtt Paho error code", ex.get_reason_code());
            throw VrEventsException(NETWORK_ERROR, "Can not publish vr game status event.");
        }
    }
}