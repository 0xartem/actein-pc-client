#ifndef VR_EVENTS_EXCEPTION_H__
#define VR_EVENTS_EXCEPTION_H__

#include <stdexcept>

namespace vr_events
{
    class VrEventsException : public std::runtime_error
    {
    public:
        explicit VrEventsException(const std::string & message)
            : std::runtime_error("vr_events::VrEventsException: " + message)
        {
        }
    };
}

#endif //VR_EVENTS_EXCEPTION_H__
