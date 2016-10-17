#include <mqtt/exception.h>
#include "gen/vr_booth_info.pb.h"
#include "IPublisher.h"
#include "CommonActionListener.h"
#include "MqttVrEventsPublisher.h"
#include "VrEventsException.h"
#include "VrTopicBuilder.h"

using CommonListener = mqtt_transport::CommonActionListener;

namespace vr_events
{
    MqttVrEventsPublisher::MqttVrEventsPublisher(
        mqtt_transport::IPublisher & publisher,
        const VrBoothInfo & vrBoothInfo)
        : mPublisher(publisher)
        , mVrBoothInfo(vrBoothInfo)
    {
    }

    void MqttVrEventsPublisher::PublishVrGameStatusEvent(
        const VrGameStatus & status,
        mqtt_transport::IActionStatusObserver * actionObserver)
    {
        try
        {
            VrGameStatusEvent event;
            event.set_status(status);

            VrTopicBuilder topicBuilder;
            std::string topic = topicBuilder.SetToGameStatus()
                .setBoothId(mVrBoothInfo.id())
                .Build();

            std::unique_ptr<CommonListener> listener = std::make_unique<CommonListener>(
                mqtt_transport::Action::PUBLISH,
                actionObserver
                );

            mPublisher.Publish(
                topic, event,
                *listener//!!!
                );
            //  to be continued...
        }
        catch (const mqtt::exception & ex)
        {
            throw VrEventsException("Can not publish vr game status event.");
        }
    }
}