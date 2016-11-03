#include <spdlog/spdlog.h>
#include <boost/application.hpp>
#include <boost/application/auto_handler.hpp>
#include <example/setup/windows/setup/service_setup.hpp>
#include "ActeinService.h"

std::shared_ptr<spdlog::logger> ConfigLog()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("c:\\Code\\DevelopmentProjects\\CommercialProjects\\Actein\\actein-pc-client\\bin\\Win32\\Debug\\CommonLog.log"));

    auto combinedLogger = std::make_shared<spdlog::logger>(
        spdlog::COMMON_LOGGER_NAME, sinks.begin(), sinks.end()
        );

    spdlog::register_logger(combinedLogger);
    return combinedLogger;
}

int main(int argc, wchar_t ** argv)
{
    try
    {
        auto logger = ConfigLog();
        logger->info("Log configured");
        logger->flush_on(spdlog::level::info);

        try
        {
            boost_app::context appContext;
            boost_app::auto_handler<as::ActeinService> app(appContext);

            appContext.insert<boost_app::path>(std::make_shared<boost_app::path>());
            appContext.insert<boost_app::args>(std::make_shared<boost_app::args>(argc, argv));

            return boost_app::launch<boost_app::server>(app, appContext);
        }
        catch (const boost::system::system_error & se)
        {
            logger->error(se.what());
            return 1;
        }
    }
    catch (const spdlog::spdlog_ex & ex)
    {
        std::cerr << "Log init failed" << ex.what() << std::endl;
        return 1;
    }
}