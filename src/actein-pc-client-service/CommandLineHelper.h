#ifndef COMMAND_LINE_HELPER_H__
#define COMMAND_LINE_HELPER_H__

#include <memory>
#include <boost/program_options.hpp>
#include <boost/application.hpp>

namespace boost_app = boost::application;
namespace po = boost::program_options;

namespace spdlog
{
    class logger;
}

namespace as
{
    class CommandLineHelper
    {
    public:
        explicit CommandLineHelper(const std::shared_ptr<boost_app::args> & args);
        
        po::options_description BuildServiceOptions();

        int GetBoothId() const
        {
            return mBoothId;
        }

        std::string GetBrokerHost() const
        {
            return mBrokerHost;
        }

        bool IsTestMode() const
        {
            return mTestMode;
        }

        __int64 GetTestGameId() const
        {
            return mTestGameId;
        }

    private:
        void ParseCommandLineArgs(const po::options_description & opts);

    private:
        std::string mBrokerHost;
        int mBoothId;
        bool mTestMode;
        __int64 mTestGameId;

        std::shared_ptr<boost_app::args> mArgs;
        std::shared_ptr<spdlog::logger> mLogger;

        static const std::string DEFAULT_BROKER_HOST;
        static const int DEFAULT_BOOTH_ID = 1;
        static const bool DEFAULT_TEST_MODE = false;
        static const __int64 DEFAULT_TEST_GAME_ID = 392190; //Selfie tennis id

    };
}

#endif //COMMAND_LINE_HELPER_H__
