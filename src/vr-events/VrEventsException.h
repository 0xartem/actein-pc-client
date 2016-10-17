#ifndef VR_EVENTS_EXCEPTION_H__
#define VR_EVENTS_EXCEPTION_H__

#include <stdexcept>

namespace vr_events
{
    class VrEventsException : public std::runtime_error
    {
    public:
        explicit VrEventsException(
            const std::string & message,
            const std::string & mqttMessage = std::string())
            : std::runtime_error(message + " " + mqttMessage)
        {
        }
    };
}

#endif //VR_EVENTS_EXCEPTION_H__
