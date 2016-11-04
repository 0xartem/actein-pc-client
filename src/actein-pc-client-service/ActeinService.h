#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <thread>
#include <boost/application.hpp>
#include <boost/program_options.hpp>
#include <GameRunner.h>

namespace boost_app = boost::application;
namespace po = boost::program_options;

namespace spdlog
{
    class logger;
}

namespace actein
{
    class ConnectionModel;
}

namespace as
{
    class ActeinService
    {
    public:
        ActeinService(boost_app::context & context);
        ~ActeinService();

        int operator()();
        bool stop();
        bool resume();
        bool pause();

    private:
        void OnStart();
        void ConfigureLog();

        po::options_description BuildServiceOptions();
        void ParseCommandLineArgs(const po::options_description & opts);

    private:
        ActeinService(const ActeinService &) = delete;
        ActeinService & operator=(const ActeinService &) = delete;

    private:
        int mBoothId;
        std::string mBrokerHost;
        std::thread mWorker;
        boost_app::context & mContext;

        std::unique_ptr<actein::ConnectionModel> mConnectionModel;
        std::shared_ptr<spdlog::logger> mLogger;

        bool mTestMode;
        google::protobuf::int64 mTestGameId;
        actein::GameRunner mTestGameRunner;
    };
}

#endif //ACTEIN_SERVICE_H__
