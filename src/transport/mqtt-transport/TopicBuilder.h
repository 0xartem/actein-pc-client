#ifndef TOPIC_BUILDER_H__
#define TOPIC_BUILDER_H__

#include <string>
#include "Topics.h"

namespace mqtt_transport
{
    class TopicBuilder
    {
    public:
        static std::string BuildEmbDeviceLastWill(int boothId)
        {
            std::string topic = Topics::EMB_DEVICE_ONLINE_STATUS;
            SetBoothId(topic, boothId);
            return topic;
        }

        static std::string BuildPcLastWill(int boothId)
        {
            std::string topic = Topics::PC_ONLINE_STATUS;
            SetBoothId(topic, boothId);
            return topic;
        }

    private:
        static void SetBoothId(std::string & topic, int boothId)
        {
            size_t boothIdIdx = topic.find(Topics::BOOTH_ID);
            if (boothIdIdx != std::string::npos)
            {
                topic.replace(boothIdIdx, Topics::BOOTH_ID.size(), std::to_string(boothId));
            }
        }
    };
}

#endif //TOPIC_BUILDER_H__
