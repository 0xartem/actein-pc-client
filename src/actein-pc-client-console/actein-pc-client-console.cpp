#include <iostream>
#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include <mqtt/exception.h>
#include <mqtt/iaction_listener.h>
#include <Connection.h>
#include <IVrEventsManager.h>
#include <VrEventsException.h>
#include "ConnectionModel.h"

std::shared_ptr<spdlog::logger> ConfigLog()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("CommonLog.log"));

    auto combinedLogger = std::make_shared<spdlog::logger>(
        spdlog::COMMON_LOGGER_NAME, sinks.begin(), sinks.end()
        );

    spdlog::register_logger(combinedLogger);
    return combinedLogger;
}

int main()
{
    try
    {
        auto logger = ConfigLog();
        logger->info("Log configured");
        logger->flush_on(spdlog::level::info);
        try
        {
            ConnectionModel model("iot.eclipse.org", 1);
            model.Start();

            std::cout << "Press ESC and ENTER to stop..." << std::endl;
            while (std::cin.get() != 'e') {}

            model.Stop();

            std::cout << "Press any key to exit..." << std::endl;
            std::cin.get();
            return 0;
        }
        catch (const vr_events::VrEventsException & ex)
        {
            logger->error(ex.what());
            return 1;
        }
        catch (const mqtt::persistence_exception& ex)
        {
            logger->error(ex.what());
            return 1;
        }
        catch (const mqtt::exception & ex)
        {
            logger->error(ex.what());
            return 1;
        }
        catch (const std::exception & ex)
        {
            logger->error(ex.what());
            return 1;
        }

        spdlog::drop_all();
    }
    catch (const spdlog::spdlog_ex & ex)
    {
        std::cout << "Log init failed" << ex.what() << std::endl;
        return 1;
    }
}
