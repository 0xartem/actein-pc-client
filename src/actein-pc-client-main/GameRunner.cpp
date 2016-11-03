#include <sstream>
#include <boost/process.hpp> 
#include <spdlog/spdlog.h>
#include <WinServiceUtils.h>
#include "GameRunner.h"

namespace actein
{
    GameRunner::GameRunner()
        : mSteamPath(L"\"C:\\Program Files (x86)\\Steam\\Steam.exe\"")
        , mGameRunning(false)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    bool GameRunner::Run(const vr_events::VrGame & game)
    {
        std::unique_lock<std::mutex> locker(mSync);

        std::vector<std::string> args;
        
        args.push_back("-applaunch");
        args.push_back(std::to_string(game.steam_game_id()));
        args.push_back("-login");
        args.push_back("actein");
        args.push_back("RimRam66!");
        //args.push_back("-tcp");
        //args.push_back("-silent");

        /*std::wstringstream commandLineStrm;
        commandLineStrm << mSteamPath << L" -applaunch 250820 -login actein RimRam66!";
        bool result = RunSteamProcess(commandLineStrm.str());*/

        std::wstringstream commandLineStrm2;
        commandLineStrm2 << mSteamPath << L" -applaunch " << std::to_wstring(game.steam_game_id()) << " -login actein RimRam66!";
        bool result = RunSteamProcess(commandLineStrm2.str());
        if (result)
        {
            mGameRunning = true;
        }
        return result;

        /*boost::process::context ctx;
        ctx.stdout_behavior = boost::process::silence_stream();

        boost::process::child steam = boost::process::launch(mSteamPath, args, ctx);
        boost::process::status status = steam.wait();*/
    }

    bool GameRunner::IsGameRunning() const
    {
        std::unique_lock<std::mutex> locker(mSync);
        return mGameRunning;
    }

    bool GameRunner::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);

        std::vector<std::string> args;
        args.push_back("-shutdown");

        std::wstringstream commandLineStrm;
        commandLineStrm << mSteamPath << " -shutdown";

        bool result = RunSteamProcess(commandLineStrm.str());
        if (result)
        {
            mGameRunning = false;
        }
        return result;

        /*boost::process::context ctx;
        ctx.stdout_behavior = boost::process::silence_stream();

        boost::process::child steam = boost::process::launch(mSteamPath, args, ctx);
        boost::process::status status = steam.wait();*/
    }

    bool GameRunner::RunSteamProcess(const std::wstring & commandLineStr)
    {
        // Get the ID of the session attached to the physical console.
        DWORD dwSessionId = utils::GetSessionIdOfUser(nullptr, nullptr);
        if (dwSessionId == 0xFFFFFFFF)
        {
            mLogger->error("Can not get current user id, Windows error code: {}", ::GetLastError());
            return false;
        }

        DWORD dwExitCode = 0;
        if (!utils::CreateInteractiveProcess(
            dwSessionId,
            const_cast<wchar_t *>(commandLineStr.c_str()),
            FALSE, 0,
            &dwExitCode))
        {
            mLogger->error(
                "Can not start steam process, Windows error code: {}; Steam exit code: {}",
                ::GetLastError(),
                dwExitCode
            );
            return false;
        }
        return true;
    }
}