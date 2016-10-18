#include <mqtt/exception.h>
#include <spdlog/spdlog.h>
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
        , mCommonListener(new mqtt_transport::CommonActionListener(
            mqtt_transport::Action::PUBLISH, actionObserver
        ))
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
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

            mPublisher.Publish(topic, event, *mCommonListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException("Can not publish vr game turn on event.");
        }
    }

    void MqttVrEventsPublisher::PublishVrGameOffEvent()
    {
        try
        {
            VrGameOffEvent event;
            event.mutable_vr_booth_info()->set_id(mVrBoothInfo->id());

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameOn().SetBoothId(mVrBoothInfo->id()).Build();

            mPublisher.Publish(topic, event, *mCommonListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException("Can not publish vr game turn off event.");
        }
    }

    void MqttVrEventsPublisher::PublishVrGameStatusEvent(const VrGameStatus & status)
    {
        try
        {
            VrGameStatusEvent event;
            event.set_status(status);

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameStatus().SetBoothId(mVrBoothInfo->id()).Build();

            mPublisher.Publish(topic, event, *mCommonListener);
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
            throw VrEventsException("Can not publish vr game status event.");
        }
    }
}