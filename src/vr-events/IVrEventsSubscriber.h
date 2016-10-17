#ifndef I_VR_EVENTS_SUBSCRIBER_H__
#define I_VR_EVENTS_SUBSCRIBER_H__

namespace mqtt_transport
{
    class IActionStatusObserver;
}


namespace vr_events
{
    class IVrEventsSubscriber
    {
    public:
        virtual void SubscribeToAll(mqtt_transport::IActionStatusObserver * actionObserver) = 0;
        virtual void UnsubscribeFromAll(mqtt_transport::IActionStatusObserver * actionObserver) = 0;

        virtual void SubscribeToGameOnEvent(mqtt_transport::IActionStatusObserver * actionObserver) = 0;
        virtual void UnsubscribeFromGameOnEvent(mqtt_transport::IActionStatusObserver * actionObserver) = 0;

        virtual void SubscribeToGameOffEvent(mqtt_transport::IActionStatusObserver * actionObserver) = 0;
        virtual void UnsubscribeFromGameOffEvent(mqtt_transport::IActionStatusObserver * actionObserver) = 0;

        virtual ~IVrEventsSubscriber() = default;
    };
}

#endif //I_VR_EVENTS_SUBSCRIBER_H__
