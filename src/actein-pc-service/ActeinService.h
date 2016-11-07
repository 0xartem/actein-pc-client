#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <thread>
#include <GameRunner.h>
#include "CommandLineHelper.h"

namespace boost_app = boost::application;

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

    private:
        ActeinService(const ActeinService &) = delete;
        ActeinService & operator=(const ActeinService &) = delete;

    private:
        boost_app::context & mContext;

        std::thread mWorker;
        std::unique_ptr<as::CommandLineHelper> mCommandLineHelper;
        std::unique_ptr<actein::ConnectionModel> mConnectionModel;

        std::shared_ptr<spdlog::logger> mLogger;

        actein::GameRunner mTestGameRunner;
    };
}

#endif //ACTEIN_SERVICE_H__
