#ifndef VR_TOPIC_BUILDER_H__
#define VR_TOPIC_BUILDER_H__

#include <string>
#include <algorithm>
#include "VrTopics.h"
#include "Topics.h"

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

        VrTopicBuilder & setBoothId(int boothId)
        {
            SetBoothIdOrWildcard(std::to_string(boothId));
            return *this;
        }

        std::string Build()
        {
            return mTopic;
        }

    private:
        void SetBoothIdOrWildcard(const std::string & value)
        {
            std::replace(mTopic.begin(), mTopic.end(), mqtt_transport::Topics::BOOTH_ID, value);
        }

    private:
        std::string mTopic;
    };
}

#endif //VR_TOPIC_BUILDER_H__
