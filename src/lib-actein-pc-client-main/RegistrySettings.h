#ifndef REGISTRY_SETTINGS_H__
#define REGISTRY_SETTINGS_H__

#include <string>
#include "Settings.h"

namespace actein
{
    class RegistrySettings : public Settings
    {
    public:
        RegistrySettings();

    private:
        static const std::string REGISTRY_ACTEIN_KEY;
        static const std::string REG_VALUE_STEAM_ACCOUNT_NAME;
        static const std::string REG_VALUE_STEAM_ACCOUNT_PASSSWORD;
        static const std::string REG_VALUE_MQTT_BROKER_ADDRESS;
        static const std::string REG_VALUE_BOOTH_ID;
    };
}

#endif //REGISTRY_SETTINGS_H__
