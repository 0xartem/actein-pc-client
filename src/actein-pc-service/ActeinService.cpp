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
#include <RegistrySettings.h>
#include "CommandLineHelper.h"
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

            try
            {
                ConfigureLog();
            }
            catch (const spdlog::spdlog_ex & ex)
            {
                std::cerr << "Log init failed" << ex.what() << std::endl;
            }

            mCommandLineHelper = std::make_unique<as::CommandLineHelper>(mContext.find<boost_app::args>());
            mRegistrySettings = std::make_unique<actein::RegistrySettings>();
        }
        // Need to write information to the log here and re-throw the exception
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
            throw;
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
            actein::Settings * settings = ChooseSettings();

            mLogger->info("MQTT Broker: {}", settings->GetBrokerHost());
            mLogger->info("Booth Id: {}", settings->GetBoothId());
            mLogger->info("Steam Account: {}", settings->GetSteamAccountName());

            mConnectionModel = std::make_unique<actein::ConnectionModel>(*settings);
            mConnectionModel->Start();

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

    bool ActeinService::stop()
    {
        try
        {
            mConnectionModel->Stop();
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

    actein::Settings * ActeinService::ChooseSettings()
    {
        actein::Settings * settings = mRegistrySettings.get();
        if (mCommandLineHelper->IsNoRegistry())
        {
            settings = mCommandLineHelper.get();
        }
        return settings;
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