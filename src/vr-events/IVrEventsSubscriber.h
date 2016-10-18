#ifndef I_VR_EVENTS_SUBSCRIBER_H__
#define I_VR_EVENTS_SUBSCRIBER_H__

namespace vr_events
{
    class IVrEventsSubscriber
    {
    public:
        virtual void SubscribeToAll() = 0;
        virtual void UnsubscribeFromAll() = 0;

        virtual void SubscribeToGameOnEvent() = 0;
        virtual void UnsubscribeFromGameOnEvent() = 0;

        virtual void SubscribeToGameOffEvent() = 0;
        virtual void UnsubscribeFromGameOffEvent() = 0;

        virtual ~IVrEventsSubscriber() = default;
    };
}

#endif //I_VR_EVENTS_SUBSCRIBER_H__
