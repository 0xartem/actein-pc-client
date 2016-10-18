#ifndef VR_TOPIC_BUILDER_H__
#define VR_TOPIC_BUILDER_H__

#include <string>
#include <algorithm>
#include "VrTopics.h"
#include "Topics.h"

using TrTopics = mqtt_transport::Topics;

namespace vr_events
{
    class VrTopicBuilder
    {
    public:
        VrTopicBuilder & SetToAll()
        {
            mTopic = VrTopics::VR_PC_GAME_ALL;
            return *this;
        }

        VrTopicBuilder & SetToGameOn()
        {
            mTopic = VrTopics::VR_PC_TURN_GAME_ON;
            return *this;
        }

        VrTopicBuilder & SetToGameOff()
        {
            mTopic = VrTopics::VR_PC_TURN_GAME_OFF;
            return *this;
        }

        VrTopicBuilder & SetToGameStatus()
        {
            mTopic = VrTopics::VR_PC_GAME_STATUS;
            return *this;
        }

        VrTopicBuilder & SetAllBooths()
        {
            SetBoothIdOrWildcard("+");
            return *this;
        }

        VrTopicBuilder & SetBoothId(int boothId)
        {
            SetBoothIdOrWildcard(std::to_string(boothId));
            return *this;
        }

        std::string Build()
        {
            return mTopic;
        }

    private:
        bool SetBoothIdOrWildcard(const std::string & value)
        {
            size_t boothIdIdx = mTopic.find(TrTopics::BOOTH_ID);
            if (boothIdIdx == std::string::npos)
            {
                return false;
            }
            mTopic.replace(boothIdIdx, TrTopics::BOOTH_ID.size(), value);
            return true;
        }

    private:
        std::string mTopic;
    };
}

#endif //VR_TOPIC_BUILDER_H__
