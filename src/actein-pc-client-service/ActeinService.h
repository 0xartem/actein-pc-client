#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <thread>
#include <boost/application.hpp>
#include <GameRunner.h>

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
        void onStart();

    private:
        ActeinService(const ActeinService &) = delete;
        ActeinService & operator=(const ActeinService &) = delete;

    private:
        boost_app::context & mContext;
        std::thread mWorker;

        std::unique_ptr<actein::ConnectionModel> mConnectionModel;
        actein::GameRunner mTestGameRunner;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //ACTEIN_SERVICE_H__
