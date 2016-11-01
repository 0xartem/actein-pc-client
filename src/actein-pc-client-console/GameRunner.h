#ifndef GAME_RUNNER_H__
#define GAME_RUNNER_H__

#include <string>
#include <gen/vr_game.pb.h>

class GameRunner
{
public:
    GameRunner();
    void Run(const vr_events::VrGame & game);
    void Stop();

private:
    std::string mSteamPath;
};

#endif //GAME_RUNNER_H__
