#ifndef CONNECTION_MODEL_H__
#define CONNECTION_MODEL_H__

#include <memory>
#include <mutex>
#include "IActionStatusObserver.h"
#include "IConnectionObserver.h"
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
}

namespace vr_events
{
    class IVrEventsManager;
}

namespace actein
{
    class Settings;
    class ScheduleVrEventsHandler;

    class ConnectionModel :
        public mqtt_transport::IActionStatusObserver,
        public mqtt_transport::IConnectionObserver,
        public vr_events::IVrEventsManagerOwner
    {
    public:
        ConnectionModel(Settings & settings);
        ~ConnectionModel();
        void Start();
        void Stop();

        // vr_events::IVrEventsManagerOwner
        vr_events::IVrEventsManager * GetVrEventsManager() const;

        // mqtt_transport::IActionStatusObserver
        void OnActionSuccess(mqtt_transport::Action action, const std::string & message) override;
        void OnActionFailure(mqtt_transport::Action action, const std::string & message) override;

        // mqtt_transport::IConnectionObserver
        void OnConnectionLost() override;

    private:
        mutable std::mutex mSync;
        std::unique_ptr<ScheduleVrEventsHandler> mVrEventsHandler;
        std::unique_ptr<vr_events::IVrEventsManager> mVrEventsManager;
        std::unique_ptr<mqtt_transport::Connection> mConnection;
        std::unique_ptr<mqtt::iaction_listener> mConnectListener;
        std::unique_ptr<mqtt::iaction_listener> mDisconnectListener;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //CONNECTION_MODEL_H__
