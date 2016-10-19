#ifndef I_VR_EVENTS_MANAGER_H__
#define I_VR_EVENTS_MANAGER_H__

namespace mqtt_transport
{
    class IConnectionObserver;
    class IActionStatusObserver;
}

namespace vr_events
{
    class IVrEventsHandler;
    class IVrEventsPublisher;
    class IVrEventsSubscriber;

    class IVrEventsManager
    {
    public:
        // throws VrEventsException
        virtual void Start(
            IVrEventsHandler * vrEventsHandler,
            mqtt_transport::IConnectionObserver * connectionObserver,
            mqtt_transport::IActionStatusObserver * actionObserver) = 0;

        // throws VrEventsException
        virtual void Stop() = 0;
        virtual bool IsRunning() const = 0;

        virtual IVrEventsPublisher * GetPublisher() const = 0;
        virtual IVrEventsSubscriber * GetSubscriber() const = 0;

        virtual ~IVrEventsManager() = default;
    };
}

#endif //I_VR_EVENTS_MANAGER_H__
