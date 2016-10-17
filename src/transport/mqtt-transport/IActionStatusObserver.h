#ifndef ACTION_STATUS_OBSERVER_H__
#define ACTION_STATUS_OBSERVER_H__

#include <string>
#include "Action.h"

namespace mqtt_transport
{
    class IActionStatusObserver
    {
    public:
        virtual void OnActionSuccess(Action action, const std::string & message) = 0;
        virtual void OnActionFailure(Action action, const std::string & message) = 0;
        virtual ~IActionStatusObserver() = default;
    };
}

#endif //ACTION_STATUS_OBSERVER_H__

