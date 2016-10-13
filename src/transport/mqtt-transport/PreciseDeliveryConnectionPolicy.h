#ifndef PRICESE_DELIVERY_CONNECTION_POLCIY_H__
#define PRICESE_DELIVERY_CONNECTION_POLCIY_H__

#include "DefaultConnectionPolicy.h"
#include "QualityOfService.h"
#include "Topics.h"

namespace mqtt_transport
{
    class PreciseDeliveryConnectionPolicy : public DefaultConnectionPolicy
    {
    public:
        virtual int getQualityOfService() const override
        {
            return convertToMqttQosValues(QualityOfService::EXACTLY_ONCE_DELIVERY_GUARANTEE);
        }

        virtual bool shouldRetainMessages() const override
        {
            return true;
        }

        virtual bool isPersistentSession() const override
        {
            return false;
        }

        virtual bool shouldUseLastWill() const override
        {
            return true;
        }

        virtual std::string getLastWillTopic() const override
        {
            return Topics::PC_ONLINE_STATUS;
        }

        virtual std::string getLastWillPayload() const override
        {
            return "offline";
        }
    };
}

#endif //PRICESE_DELIVERY_CONNECTION_POLCIY_H__