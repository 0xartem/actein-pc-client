#ifndef COMMAND_LINE_HELPER_H__
#define COMMAND_LINE_HELPER_H__

#include <memory>
#include <boost/program_options.hpp>
#include <boost/application.hpp>
#include <Settings.h>

namespace boost_app = boost::application;
namespace po = boost::program_options;

namespace spdlog
{
    class logger;
}

namespace as
{
    class CommandLineHelper : public actein::Settings
    {
    public:
        explicit CommandLineHelper(const std::shared_ptr<boost_app::args> & args);
        
        po::options_description BuildServiceOptions();

        bool IsNoRegistry() const
        {
            return mNoRegistry;
        }

    private:
        void ParseCommandLineArgs(const po::options_description & opts);

    private:
        std::shared_ptr<boost_app::args> mArgs;
        std::shared_ptr<spdlog::logger> mLogger;

        bool mNoRegistry;

        static const std::string DEFAULT_STEAM_ACCOUNT_NAME;
        static const std::string DEFAULT_BROKER_HOST;
        static const int DEFAULT_BOOTH_ID = 1;
    };
}

#endif //COMMAND_LINE_HELPER_H__
