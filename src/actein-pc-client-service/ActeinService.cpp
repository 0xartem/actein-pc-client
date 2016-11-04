#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include <mqtt/exception.h>
#include <mqtt/iaction_listener.h>
#include <Connection.h>
#include <IVrEventsManager.h>
#include <VrEventsException.h>
#include <ConnectionModel.h>
#include <WinServiceUtils.h>
#include "ActeinService.h"

namespace as
{
    ActeinService::ActeinService(boost_app::context & context)
        : mContext(context)
        , mTestMode(false)
        , mBoothId(0)
        , mTestGameId(0)
    {
        try
        {
            auto execPath = mContext.find<boost_app::path>()->executable_path().string();
            ::SetCurrentDirectory(execPath.c_str());

            ConfigureLog();

            po::options_description opts = BuildServiceOptions();
            ParseCommandLineArgs(opts);
        }
        catch (const spdlog::spdlog_ex & ex)
        {
            std::cerr << "Log init failed" << ex.what() << std::endl;
        }
        // Need to handle exceptions here because constructor is being called by boost_app code
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    ActeinService::~ActeinService()
    {
        spdlog::drop_all();
    }

    int ActeinService::operator()()
    {
        try
        {
            auto status = mContext.find<boost_app::status>();

            mConnectionModel = std::make_unique<actein::ConnectionModel>(mBrokerHost, mBoothId);
            mConnectionModel->Start();

            mWorker = std::thread(&ActeinService::OnStart, this);

            mContext.find<boost_app::wait_for_termination_request>()->wait();

            return 0;
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error(ex.what());
        }
        catch (const mqtt::persistence_exception& ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
        return 0;
    }

    void ActeinService::OnStart()
    {
        try
        {
            if (mTestMode)
            {
                vr_events::VrGame game;
                game.set_steam_game_id(mTestGameId);
                mTestGameRunner.Run(game);
            }

            //std::wstring runDllLockScreen = L"rundll32.exe user32.dll,LockWorkStation";
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    bool ActeinService::stop()
    {
        try
        {
            if (mWorker.joinable())
            {
                mWorker.join();
            }

            mConnectionModel->Stop();
            
            if (mTestMode && mTestGameRunner.IsGameRunning())
            {
                mTestGameRunner.Stop();
            }
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error(ex.what());
        }
        catch (const mqtt::persistence_exception& ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
        return true;
    }

    bool ActeinService::resume()
    {
        return true;
    }

    bool ActeinService::pause()
    {
        return true;
    }

    po::options_description ActeinService::BuildServiceOptions()
    {
        po::options_description serviceOptions("service options");
        serviceOptions.add_options()
            ("help", "produce a help message")
            ("test", "test mode - start/stop game using service console")
            ("broker",
                po::value<std::string>(&mBrokerHost)->default_value("test.mosquitto.org"),
                "MQTT broker address")
            ("booth",
                po::value<int>(&mBoothId)->default_value(1),
                "booth id number")
            ("test_game_id",
                po::value<google::protobuf::int64>(&mTestGameId)->default_value(392190),
                "test game id to start/stop - default is Selfie Tennis");
        
        return serviceOptions;
    }

    void ActeinService::ParseCommandLineArgs(const po::options_description & opts)
    {
        auto args = mContext.find<boost_app::args>();
        po::variables_map vm;
        po::store(po::parse_command_line(args->argc(), args->argv(), opts), vm);

        if (vm.find("help") != vm.end())
        {
            for (const auto & opt : opts.options())
            {
                mLogger->info("{} {} {}", opt->format_name(), opt->format_parameter(), opt->description());
            }
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
        it = vm.find("test_game_id");
        if (it != vm.end())
        {
            mTestGameId = it->second.as<google::protobuf::int64>();
        }
    }

    void ActeinService::ConfigureLog()
    {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("CommonLog", "log", 1048576 * 5, 3));

        mLogger = std::make_shared<spdlog::logger>(
            spdlog::COMMON_LOGGER_NAME, sinks.begin(), sinks.end()
            );

        spdlog::register_logger(mLogger);

        mLogger->info("Log configured");
        mLogger->flush_on(spdlog::level::info);
    }
}