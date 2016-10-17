#ifndef CONNECT_OPTIONS_BUILDER_H__
#define CONNECT_OPTIONS_BUILDER_H__

#include <mqtt/connect_options.h>
#include "IConnectionPolicy.h"

namespace mqtt_transport
{
    class ConnectOptionsBuilder
    {
    public:
        static mqtt::connect_options_ptr BuildConnectOptions(IConnectionPolicy & connectionPolicy);
    };
}

#endif //CONNECT_OPTIONS_BUILDER_H__
