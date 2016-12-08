#ifndef MQTT_VR_EVENTS_MANAGER_H__
#define MQTT_VR_EVENTS_MANAGER_H__

#include <memory>
#include <mutex>
#include "IVrEventsManager.h"

namespace mqtt_transport
{
    class Connection;
}

namespace vr_events
{
    class VrBoothInfo;
    class MqttVrEventsPublisher;
    class MqttVrEventsSubscriber;

    class MqttVrEventsManager : public IVrEventsManager
    {
    public:
        MqttVrEventsManager(
            mqtt_transport::Connection & connection,
            IVrEventsHandler * vrEventsHandler,
            mqtt_transport::IActionStatusObserver * actionObserver,
            const std::shared_ptr<VrBoothInfo> & vrBoothInfo
        );

        void Start() override;
        void Stop() override;
        bool IsRunning() const override;

        IVrEventsPublisher * GetPublisher() const override;
        IVrEventsSubscriber * GetSubscriber() const override;
        mqtt_transport::IMessageHandler * GetMessageHandler() const override;

    private:
        bool mIsRunning;
        mutable std::mutex mSync;

        std::unique_ptr<MqttVrEventsPublisher> mVrEventsPublisher;
        std::unique_ptr<MqttVrEventsSubscriber> mVrEventsSubscriber;
    };
}

#endif //MQTT_VR_EVENTS_MANAGER_H__
