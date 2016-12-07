#ifndef PRICESE_DELIVERY_CONNECTION_POLCIY_H__
#define PRICESE_DELIVERY_CONNECTION_POLCIY_H__

#include "gen/online_status_event.pb.h"
#include "DefaultConnectionPolicy.h"
#include "QualityOfService.h"
#include "Topics.h"
#include "TopicBuilder.h"

namespace mqtt_transport
{
    class PreciseDeliveryConnectionPolicy : public DefaultConnectionPolicy
    {
    public:
        explicit PreciseDeliveryConnectionPolicy(int boothId)
        {
            mLastWillTopic = TopicBuilder::BuildPcLastWill(boothId);
        }

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
            return mLastWillTopic;
        }

        virtual std::string getLastWillPayload() const override
        {
            OnlineStatusEvent event;
            event.set_status(OnlineStatus::OFFLINE);
            return event.SerializeAsString();
        }

    private:
        std::string mLastWillTopic;
    };
}

#endif //PRICESE_DELIVERY_CONNECTION_POLCIY_H__