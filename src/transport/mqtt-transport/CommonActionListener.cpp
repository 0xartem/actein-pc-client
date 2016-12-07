#include <sstream>
#include <mqtt/token.h>
#include <spdlog/spdlog.h>
#include "IActionStatusObserver.h"
#include "CommonActionListener.h"

namespace mqtt_transport
{
    CommonActionListener::CommonActionListener(Action action, IActionStatusObserver * actionStatusObserver)
    {
        mAction = action;
        mActionStatusObserver = actionStatusObserver;
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    void CommonActionListener::on_success(const mqtt::itoken& asyncActionToken)
    {
        try
        {
            std::string resultMessage = BuildOnSuccessMessage(asyncActionToken);
            mLogger->info(resultMessage);

            if (mActionStatusObserver != nullptr)
            {
                mActionStatusObserver->OnActionSuccess(mAction, resultMessage);
            }
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    void CommonActionListener::on_failure(const mqtt::itoken& asyncActionToken, const char * message)
    {
        try
        {
            std::string pahoMessage;
            if (message != nullptr)
            {
                pahoMessage = message;
            }

            std::string resultMessage = BuildOnFailureMessage(asyncActionToken, pahoMessage);
            mLogger->info(resultMessage);
            
            if (mActionStatusObserver != nullptr)
            {
                mActionStatusObserver->OnActionFailure(mAction, resultMessage);
            }
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    std::string CommonActionListener::BuildOnSuccessMessage(const mqtt::itoken & asyncActionToken)
    {
        std::string topics = TopicsToString(asyncActionToken.get_topics());

        switch (mAction)
        {
        case Action::CONNECT:
            return "MQTT connection succeed";
        case Action::DISCONNECT:
            return "MQTT disconnection succeed";
        case Action::SUBSCRIBE:
            return "Subscription succeed. " + topics;
        case Action::UNSUBSCRIBE:
            return "Unsubscription succeed. " + topics;
        case Action::PUBLISH:
            return "Publication succeed. " + topics;
        default:
            throw std::logic_error("Unknown action type");
        }
    }

    std::string CommonActionListener::BuildOnFailureMessage(
        const mqtt::itoken & asyncActionToken,
        const std::string & message)
    {
        std::stringstream messageStrm;
        std::string topics = TopicsToString(asyncActionToken.get_topics());

        switch (mAction)
        {
        case Action::CONNECT:
            messageStrm << "MQTT connection failed";
            break;
        case Action::DISCONNECT:
            messageStrm << "MQTT disconnection failed";
            break;
        case Action::SUBSCRIBE:
            messageStrm << "Subscription failed. " + topics;
            break;
        case Action::UNSUBSCRIBE:
            messageStrm << "Unsubscription failed. " + topics;
            break;
        case Action::PUBLISH:
            messageStrm << "Publication failed. " + topics;
            break;
        default:
            throw std::logic_error("Unknown action type");
        }

        messageStrm << ". " << message;
        return messageStrm.str();
    }

    std::string CommonActionListener::TopicsToString(const std::vector<std::string> & topics)
    {
        std::ostringstream topicsStrm;
        if(!topics.empty())
        {
            topicsStrm << "Topics: ";
            std::for_each(
                topics.cbegin(),
                topics.cend(),
                [&topicsStrm](const std::string & topic) { topicsStrm << " " << topic << ";"; }
            );
        }
        return topicsStrm.str();
    }
}