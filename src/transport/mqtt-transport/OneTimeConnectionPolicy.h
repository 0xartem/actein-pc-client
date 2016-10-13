#ifndef ONE_TIME_CONNECTION_POLCIY_H__
#define ONE_TIME_CONNECTION_POLCIY_H__

#include "DefaultConnectionPolicy.h"
#include "QualityOfService.h"

namespace mqtt_transport
{
    class OneTimeConnectionPolicy : public DefaultConnectionPolicy
    {
    public:
        virtual int getQualityOfService() const override
        {
            return convertToMqttQosValues(QualityOfService::EXACTLY_ONCE_DELIVERY_GUARANTEE);
        }

        virtual bool shouldRetainMessages() const override
        {
            return false;
        }

        virtual bool isPersistentSession() const override
        {
            return false;
        }

        virtual bool shouldUseLastWill() const override
        {
            return false;
        }

        virtual std::string getLastWillTopic() const override
        {
            return std::string();
        }

        virtual std::string getLastWillPayload() const override
        {
            return std::string();
        }
    };
}

#endif //ONE_TIME_CONNECTION_POLCIY_H__
