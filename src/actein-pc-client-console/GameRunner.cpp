#include "GameRunner.h"

#include <boost/process.hpp> 

GameRunner::GameRunner(const vr_events::VrGame & game)
    : mSteamPath("\"C:\\Program Files (x86)\\Steam\\Steam.exe\"")
    , mGame(game)
{
}

void GameRunner::Run()
{
    std::vector<std::string> args;
    args.push_back("-applaunch");
    args.push_back(std::to_string(mGame.steam_game_id()));

    boost::process::context ctx;
    ctx.stdout_behavior = boost::process::silence_stream();

    boost::process::child steam = boost::process::launch(mSteamPath, args, ctx);
    boost::process::status status = steam.wait();
}