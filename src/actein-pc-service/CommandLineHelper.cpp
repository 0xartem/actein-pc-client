#include <spdlog/spdlog.h>
#include "CommandLineHelper.h"

namespace as
{
    const std::string CommandLineHelper::DEFAULT_STEAM_ACCOUNT_NAME = "actein";
    const std::string CommandLineHelper::DEFAULT_BROKER_HOST = "iot.eclipse.org";

    CommandLineHelper::CommandLineHelper(const std::shared_ptr<boost_app::args> & args)
        : mArgs(args)
        , mNoRegistry(false)
        , mTestMode(false)
        , mTestGameId(DEFAULT_TEST_GAME_ID)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);

        po::options_description opts = BuildServiceOptions();
        ParseCommandLineArgs(opts);
    }

    po::options_description CommandLineHelper::BuildServiceOptions()
    {
        po::options_description serviceOptions("service options");
        serviceOptions.add_options()
            ("help", "produce a help message")
            ("test", "test mode - start/stop game using service console")
            ("noreg", "use command line settings instead of registry settings")
            ("broker",
                po::value<std::string>(&mBrokerHost)->default_value(DEFAULT_BROKER_HOST),
                "MQTT broker address")
            ("booth",
                po::value<int>(&mBoothId)->default_value(DEFAULT_BOOTH_ID),
                "booth id number")
            ("steam_account",
                po::value<std::string>(&mSteamAccountName)->default_value(DEFAULT_STEAM_ACCOUNT_NAME),
                "steam account name")
            ("steam_pwd",
                po::value<std::string>(&mSteamAccountPassword),
                "steam account password")
            ("test_game_id",
                po::value<__int64>(&mTestGameId)->default_value(DEFAULT_TEST_GAME_ID),
                "test game id to start/stop - default is Selfie Tennis");

        return serviceOptions;
    }

    void CommandLineHelper::ParseCommandLineArgs(const po::options_description & opts)
    {
        po::variables_map vm;
        po::store(po::parse_command_line(mArgs->argc(), mArgs->argv(), opts), vm);

        if (vm.find("help") != vm.end())
        {
            for (const auto & opt : opts.options())
            {
                mLogger->info("{} {} {}", opt->format_name(), opt->format_parameter(), opt->description());
            }
        }
        if (vm.find("noreg") != vm.end())
        {
            mNoRegistry = true;
        }
        if (vm.find("test") != vm.end())
        {
            mTestMode = true;
        }
        auto it = vm.find("broker");
        if (it != vm.end())
        {
            mBrokerHost = it->second.as<std::string>();
        }
        it = vm.find("booth");
        if (it != vm.end())
        {
            mBoothId = it->second.as<int>();
        }
        it = vm.find("steam_account");
        if (it != vm.end())
        {
            mSteamAccountName = it->second.as<std::string>();
        }
        it = vm.find("steam_pwd");
        if (it != vm.end())
        {
            mSteamAccountPassword = it->second.as<std::string>();
        }
        it = vm.find("test_game_id");
        if (it != vm.end())
        {
            mTestGameId = it->second.as<__int64>();
        }
    }
}