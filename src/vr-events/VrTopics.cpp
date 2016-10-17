#include "VrTopics.h"
#include "Topics.h"

using TrTopics = mqtt_transport::Topics;

namespace vr_events
{
    const std::string VrTopics::VR_PC_GAME_ALL = "factory/booths/" + TrTopics::BOOTH_ID + "/pc/vr/game/#";
    const std::string VrTopics::VR_PC_TURN_GAME_ON = "factory/booths/" + TrTopics::BOOTH_ID + "/pc/vr/game/on";
    const std::string VrTopics::VR_PC_TURN_GAME_OFF = "factory/booths/" + TrTopics::BOOTH_ID + "/pc/vr/game/off";
    const std::string VrTopics::VR_PC_GAME_STATUS = "factory/booths/" + TrTopics::BOOTH_ID + "/pc/vr/game/status";
}