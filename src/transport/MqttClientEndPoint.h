#ifndef MQTT_CLIENT_END_POINT_H__
#define MQTT_CLIENT_END_POINT_H__

#include <string>
#include <UUIDGenerator.h>

namespace transport
{
    class MqttClientEndPoint
    {
    public:
        MqttClientEndPoint()
        {
            utils::UUIDGenerator uuidGenerator;
            mClientId = uuidGenerator.GenerateUUID();
        }

        explicit MqttClientEndPoint(const std::string & clientId)
        {
            mClientId = clientId;
        }

        std::string getClientId() const
        {
            return mClientId;
        }

    private:
        std::string mClientId;
    };
}

#endif //MQTT_CLIENT_END_POINT_H__
