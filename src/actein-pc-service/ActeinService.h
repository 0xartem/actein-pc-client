#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <thread>

namespace boost_app = boost::application;

namespace spdlog
{
    class logger;
}

namespace actein
{
    class GameRunner;
    class RegistrySettings;
    class ConnectionModel;
}

namespace as
{
    class CommandLineHelper;

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
        std::unique_ptr<actein::RegistrySettings> mRegistrySettings;
        std::unique_ptr<actein::ConnectionModel> mConnectionModel;
        std::unique_ptr<actein::GameRunner> mTestGameRunner;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //ACTEIN_SERVICE_H__
