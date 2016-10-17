#ifndef MQTT_VR_EVENTS_PUBLISHER_H__
#define MQTT_VR_EVENTS_PUBLISHER_H__

#include "IVrEventsPublisher.h"

namespace mqtt_transport
{
    class IPublisher;
}

namespace vr_events
{
    class VrBoothInfo;

    class MqttVrEventsPublisher : public IVrEventsPublisher
    {
    public:
        MqttVrEventsPublisher(mqtt_transport::IPublisher & publisher, const VrBoothInfo & vrBoothInfo);

        void PublishVrGameOnEvent(
            const VrGame & vrGame,
            mqtt_transport::IActionStatusObserver * actionObserver) override;

        void PublishVrGameOffEvent(mqtt_transport::IActionStatusObserver * actionObserver) override;

        void PublishVrGameStatusEvent(
            const VrGameStatus & status,
            mqtt_transport::IActionStatusObserver * actionObserver) override;

    private:
        mqtt_transport::IPublisher & mPublisher;
        const VrBoothInfo & mVrBoothInfo;
    };
}

#endif //MQTT_VR_EVENTS_PUBLISHER_H__
