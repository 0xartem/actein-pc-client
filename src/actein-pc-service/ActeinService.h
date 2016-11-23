#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <memory>

namespace boost_app = boost::application;

namespace spdlog
{
    class logger;
}

namespace actein
{
    class Settings;
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
        void ConfigureLog();
        actein::Settings * ChooseSettings();

    private:
        ActeinService(const ActeinService &) = delete;
        ActeinService & operator=(const ActeinService &) = delete;

    private:
        boost_app::context & mContext;

        std::unique_ptr<as::CommandLineHelper> mCommandLineHelper;
        std::unique_ptr<actein::RegistrySettings> mRegistrySettings;
        std::unique_ptr<actein::ConnectionModel> mConnectionModel;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //ACTEIN_SERVICE_H__
