#ifndef TOPICS_H__
#define TOPICS_H__

#include <string>

namespace mqtt_transport
{
    class Topics
    {
    public:
        static const std::string FACTORY;
        static const std::string FACTORY_ALL;

        static const std::string BOOTH_ID;

        static const std::string PC_ONLINE_STATUS;
        static const std::string EMB_DEVICE_ONLINE_STATUS;
    };
}

#endif //TOPICS_H__
