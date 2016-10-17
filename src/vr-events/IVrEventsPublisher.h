#ifndef I_VR_EVENTS_PUBLISHER_H__
#define I_VR_EVENTS_PUBLISHER_H__

#include "gen/vr_game_status_event.pb.h"

namespace mqtt_transport
{
    class IActionStatusObserver;
}

namespace vr_events
{
    class VrGame;

    class IVrEventsPublisher
    {
    public:
            // throws VrEventsException
            virtual void PublishVrGameOnEvent(
                const VrGame & vrGame,
                mqtt_transport::IActionStatusObserver * actionObserver) = 0;

            // throws VrEventsException
            virtual void PublishVrGameOffEvent(mqtt_transport::IActionStatusObserver * actionObserver) = 0;

            // throws VrEventsException
            virtual void PublishVrGameStatusEvent(
                const VrGameStatus & status,
                mqtt_transport::IActionStatusObserver * actionObserver) = 0;

        virtual ~IVrEventsPublisher() = default;
    };
}

#endif //I_VR_EVENTS_PUBLISHER_H__
