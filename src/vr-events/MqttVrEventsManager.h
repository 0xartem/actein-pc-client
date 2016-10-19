#ifndef MQTT_VR_EVENTS_MANAGER_H__
#define MQTT_VR_EVENTS_MANAGER_H__

#include <memory>
#include "IVrEventsManager.h"

namespace mqtt_transport
{
    class Connection;
}

namespace vr_events
{
    class VrBoothInfo;

    class MqttVrEventsManager : public IVrEventsManager
    {
    public:
        MqttVrEventsManager(
            mqtt_transport::Connection & connection,
            const std::shared_ptr<VrBoothInfo> & vrBoothInfo
        );

        void Start(
            IVrEventsHandler * vrEventsHandler,
            mqtt_transport::IConnectionObserver * connectionObserver,
            mqtt_transport::IActionStatusObserver * actionObserver) override;

        void Stop() override;
        bool IsRunning() const override;

        IVrEventsPublisher * GetPublisher() const override;
        IVrEventsSubscriber * GetSubscriber() const override;

    private:
        bool mIsRunning;

        std::unique_ptr<IVrEventsPublisher> mVrEventsPublisher;
        std::unique_ptr<IVrEventsSubscriber> mVrEventsSubscriber;

        mqtt_transport::Connection & mConnection;
        std::shared_ptr<VrBoothInfo> mVrBoothInfo;
    };
}

#endif //MQTT_VR_EVENTS_MANAGER_H__
