#ifndef CONNECTION_MODEL_H__
#define CONNECTION_MODEL_H__

#include <memory>
#include <mutex>
#include <map>
#include <gen/online_status_event.pb.h>
#include <IMessageHandler.h>
#include <IActionStatusObserver.h>
#include <IConnectionObserver.h>
#include <IEmbDeviceOnlineStatusHandler.h>
#include "IVrEventsManagerOwner.h"

namespace spdlog
{
    class logger;
}

namespace mqtt
{
    class iaction_listener;
}

namespace mqtt_transport
{
    class Connection;
    class LastWillManager;
    class IEmbDeviceOnlineStatusHandler;
}

namespace vr_events
{
    class IVrEventsManager;
}

namespace actein
{
    class Settings;
    class ScheduleVrEventsHandler;
    using MqttAction = mqtt_transport::Action;

    class ConnectionModel :
        public mqtt_transport::IActionStatusObserver,
        public mqtt_transport::IConnectionObserver,
        public mqtt_transport::IMessageHandler,
        public mqtt_transport::IEmbDeviceOnlineStatusHandler,
        public vr_events::IVrEventsManagerOwner
    {
    public:
        ConnectionModel(Settings & settings);
        ~ConnectionModel();
        void Start();
        void Stop();

        // vr_events::IVrEventsManagerOwner
        vr_events::IVrEventsManager * GetVrEventsManager() const override;

        // mqtt_transport::IActionStatusObserver
        void OnActionSuccess(MqttAction action, const std::string & message) override;
        void OnActionFailure(MqttAction action, const std::string & message) override;

        // mqtt_transport::IConnectionObserver
        void OnConnectionLost() override;

        // mqtt_transport::IMessageHandler
        void HandleMessage(const std::string & topic, mqtt::message_ptr message) override;

        // mqtt_transport::IEmbDeviceOnlineStatusHandler
        void OnEmbDeviceOnlineStatusChanged(mqtt_transport::OnlineStatus status) override;

    private:
        Settings & mSettings;
        mutable std::mutex mSync;
        std::map<std::string, mqtt_transport::IMessageHandler *> mMessageHandlers;

        std::unique_ptr<ScheduleVrEventsHandler> mVrEventsHandler;
        std::unique_ptr<vr_events::IVrEventsManager> mVrEventsManager;

        std::unique_ptr<mqtt_transport::LastWillManager> mLastWillManager;
        std::unique_ptr<mqtt_transport::Connection> mConnection;

        std::unique_ptr<mqtt::iaction_listener> mConnectListener;
        std::unique_ptr<mqtt::iaction_listener> mDisconnectListener;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //CONNECTION_MODEL_H__
