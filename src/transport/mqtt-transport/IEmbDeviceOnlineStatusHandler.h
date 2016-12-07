#ifndef I_EMB_DEVICE_STATUS_HANDLER_H__
#define I_EMB_DEVICE_STATUS_HANDLER_H__

#include <string>

namespace mqtt_transport
{
    class IEmbDeviceOnlineStatusHandler
    {
    public:
        virtual void OnEmbDeviceOnlineStatusChanged(OnlineStatus status) = 0;
        virtual ~IEmbDeviceOnlineStatusHandler() = default;
    };
}

#endif //I_EMB_DEVICE_STATUS_HANDLER_H__
