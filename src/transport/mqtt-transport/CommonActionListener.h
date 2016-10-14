#ifndef COMMON_ACTION_LISTENER_H__
#define COMMON_ACTION_LISTENER_H__

#include <mqtt/iaction_listener.h>
#include "Action.h"

namespace spdlog
{
    class logger;
}

namespace mqtt
{
    class itoken;
}

namespace mqtt_transport
{
    class ActionStatusObserver;

    class CommonActionListener : public mqtt::iaction_listener
    {
    public:
        CommonActionListener(Action action, ActionStatusObserver * actionStatusObserver);

        virtual void on_success(const mqtt::itoken & asyncActionToken) override;
        virtual void on_failure(const mqtt::itoken & asyncActionToken, const char * message) override;

    protected:
        std::string BuildOnSuccessMessage(const mqtt::itoken & asyncActionToken);
        std::string BuildOnFailureMessage(
            const mqtt::itoken & asyncActionToken,
            const std::string & message
        );
        std::string TopicsToString(const std::vector<std::string> & topics);

    private:
        Action mAction;
        ActionStatusObserver * mActionStatusObserver;
        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //COMMON_ACTION_LISTENER_H__
