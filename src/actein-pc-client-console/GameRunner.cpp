#include "GameRunner.h"

#include <boost/process.hpp> 

GameRunner::GameRunner()
    : mSteamPath("\"C:\\Program Files (x86)\\Steam\\Steam.exe\"")
{
}

void GameRunner::Run(const vr_events::VrGame & game)
{
    std::vector<std::string> args;
    
    args.push_back("-applaunch");

    args.push_back(std::to_string(game.steam_game_id()));

    //args.push_back("-silent");
    args.push_back("-login");
    args.push_back("actein");
    args.push_back("RimRam66!");

    //args.push_back("-tcp");

    boost::process::context ctx;
    ctx.stdout_behavior = boost::process::silence_stream();

    boost::process::child steam = boost::process::launch(mSteamPath, args, ctx);
    boost::process::status status = steam.wait();
}

void GameRunner::Stop()
{
    std::vector<std::string> args;
    args.push_back("-shutdown");

    boost::process::context ctx;
    ctx.stdout_behavior = boost::process::silence_stream();

    boost::process::child steam = boost::process::launch(mSteamPath, args, ctx);
    boost::process::status status = steam.wait();
}