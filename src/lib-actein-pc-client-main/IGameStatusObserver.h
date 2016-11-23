#ifndef I_GAME_STATUS_OBSERVER_H__
#define I_GAME_STATUS_OBSERVER_H__

#include <gen/vr_game_status_event.pb.h>

namespace actein
{
    class IGameStatusObserver
    {
    public:
        virtual vr_events::VrGameStatus GetGameStatus() const = 0;
        virtual void OnGameStatusChanged(const vr_events::VrGameStatus & status) = 0;
        virtual void OnGameStatusError(const std::string & error) = 0;
        virtual ~IGameStatusObserver() = default;
    };
}

#endif //I_GAME_STATUS_OBSERVER_H__
