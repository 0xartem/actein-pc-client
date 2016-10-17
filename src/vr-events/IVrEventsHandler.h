#ifndef I_VR_EVENTS_HANDLER_H__
#define I_VR_EVENTS_HANDLER_H__

#include <memory>

namespace vr_events
{
    class VrGameOnEvent;
    class VrGameOffEvent;
    class VrGameStatusEvent;

    class IVrEventsHandler
    {
    public:
        virtual void HandleVrGameOnEvent(std::shared_ptr<VrGameOnEvent> event) = 0;
        virtual void HandleVrOffEvent(std::shared_ptr<VrGameOffEvent> event) = 0;
        virtual void HandleVrGameStatusEvent(std::shared_ptr<VrGameOnEvent> event) = 0;
        virtual ~IVrEventsHandler() = default;
    };
}

#endif //I_VR_EVENTS_HANDLER_H__
