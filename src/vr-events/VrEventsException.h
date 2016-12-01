#ifndef VR_EVENTS_EXCEPTION_H__
#define VR_EVENTS_EXCEPTION_H__

#include <stdexcept>
#include "gen/vr_game_error.pb.h"

namespace vr_events
{
    class VrEventsException : public std::runtime_error
    {
    public:
        explicit VrEventsException(const std::string & message)
            : std::runtime_error(message)
            , mErrorCode(VrGameErrorCode::FAIL)
        {
        }

        VrEventsException(VrGameErrorCode errorCode, const std::string & message)
            : std::runtime_error(message)
            , mErrorCode(errorCode)
        {
        }

        VrGameErrorCode GetErrorCode() const
        {
            return mErrorCode;
        }

    private:
        VrGameErrorCode mErrorCode;
    };
}

#endif //VR_EVENTS_EXCEPTION_H__
