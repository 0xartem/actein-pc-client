#ifndef MQTT_VR_EVENTS_PUBLISHER_H__
#define MQTT_VR_EVENTS_PUBLISHER_H__

#include "IVrEventsPublisher.h"

namespace spdlog
{
    class logger;
}

namespace mqtt_transport
{
    class IPublisher;
    class IActionStatusObserver;
    class CommonActionListener;
}

namespace vr_events
{
    class VrBoothInfo;

    class MqttVrEventsPublisher : public IVrEventsPublisher
    {
    public:
        MqttVrEventsPublisher(
            mqtt_transport::IPublisher & publisher,
            const std::shared_ptr<VrBoothInfo> & vrBoothInfo,
            mqtt_transport::IActionStatusObserver * actionObserver
        );

        void PublishVrGameOnEvent(std::unique_ptr<VrGame> vrGame) override;
        void PublishVrGameOffEvent() override;
        void PublishVrGameStatusEvent(const VrGameStatus & status) override;
        void PublishVrGameStatusEvent(
            const VrGameStatus & status,
            std::unique_ptr<VrGameError> error) override;

    private:
        mqtt_transport::IPublisher & mPublisher;
        std::shared_ptr<VrBoothInfo> mVrBoothInfo;
        std::unique_ptr<mqtt_transport::CommonActionListener> mPublishListener;
        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //MQTT_VR_EVENTS_PUBLISHER_H__
