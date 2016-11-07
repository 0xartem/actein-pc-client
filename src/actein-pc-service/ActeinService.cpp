#include <spdlog/spdlog.h>
#include <boost/application.hpp>
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
    {
        try
        {
            auto execPath = mContext.find<boost_app::path>()->executable_path().string();
            ::SetCurrentDirectory(execPath.c_str());

            ConfigureLog();

            mCommandLineHelper = std::make_unique<as::CommandLineHelper>(mContext.find<boost_app::args>());
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
            mConnectionModel = std::make_unique<actein::ConnectionModel>(
                mCommandLineHelper->GetBrokerHost(),
                mCommandLineHelper->GetBoothId()
                );

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
            if (mCommandLineHelper->IsTestMode())
            {
                vr_events::VrGame game;
                game.set_steam_game_id(mCommandLineHelper->GetTestGameId());
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
            
            if (mCommandLineHelper->IsTestMode() && mTestGameRunner.IsGameRunning())
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