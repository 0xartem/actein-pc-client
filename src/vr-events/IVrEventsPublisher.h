#ifndef I_VR_EVENTS_PUBLISHER_H__
#define I_VR_EVENTS_PUBLISHER_H__

#include "gen/vr_game_status_event.pb.h"

namespace vr_events
{
    class VrGame;

    class IVrEventsPublisher
    {
    public:
            // throws VrEventsException
            virtual void PublishVrGameOnEvent(std::unique_ptr<VrGame> vrGame) = 0;

            // throws VrEventsException
            virtual void PublishVrGameOffEvent() = 0;

            // throws VrEventsException
            virtual void PublishVrGameStatusEvent(const VrGameStatus & status) = 0;
            
            // throws VrEventsException
            virtual void PublishVrGameStatusEvent(
                const VrGameStatus & status,
                std::unique_ptr<VrGameError> error) = 0;

        virtual ~IVrEventsPublisher() = default;
    };
}

#endif //I_VR_EVENTS_PUBLISHER_H__
