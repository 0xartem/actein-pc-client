#include "Topics.h"

namespace mqtt_transport
{
    const std::string Topics::FACTORY = u8"factory";
    const std::string Topics::FACTORY_ALL = u8"factory/#";

    const std::string Topics::BOOTH_ID = u8"boothId";

    const std::string Topics::PC_ONLINE_STATUS = u8"factory/booths/" + BOOTH_ID + "/pc/status";
    const std::string Topics::EMB_DEVICE_ONLINE_STATUS = u8"factory/booths/" + BOOTH_ID + "/embDevice/status";
}