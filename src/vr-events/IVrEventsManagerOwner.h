#ifndef I_VR_EVENTS_MANAGER_OWNER_H__
#define I_VR_EVENTS_MANAGER_OWNER_H__

namespace vr_events
{
    class IVrEventsManager;

    class IVrEventsManagerOwner
    {
    public:
        virtual vr_events::IVrEventsManager * GetVrEventsManager() const = 0;
        virtual ~IVrEventsManagerOwner() = default;
    };
}
#endif //I_VR_EVENTS_MANAGER_OWNER_H__
