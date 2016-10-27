#ifndef GAME_RUNNER_H__
#define GAME_RUNNER_H__

#include <string>
#include <gen/vr_game.pb.h>

class GameRunner
{
public:
    GameRunner();
    explicit GameRunner(const vr_events::VrGame & game);
    void Run();
    void Stop();

private:
    vr_events::VrGame mGame;
    std::string mSteamPath;
};

#endif //GAME_RUNNER_H__
