#ifndef I_CONNECTION_POLICY_H__
#define I_CONNECTION_POLICY_H__

#include <string>
#include <vector>

namespace mqtt_transport
{
    class IConnectionPolicy
    {
    public:
        virtual int getQualityOfService() const = 0;

        virtual bool shouldRetainMessages() const = 0;
        virtual bool isPersistentSession() const = 0;

        virtual bool shouldUseLastWill() const = 0;
        virtual std::string getLastWillTopic() const = 0;
        virtual std::string getLastWillPayload() const = 0;

        virtual int getKeepAliveInterval() const = 0;
        virtual int getConnectionTimeout() const = 0;

        virtual ~IConnectionPolicy() = default;
    };
}

#endif //I_CONNECTION_POLICY_H__
