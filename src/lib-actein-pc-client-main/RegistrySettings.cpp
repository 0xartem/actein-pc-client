#include "RegistrySettings.h"
#include "WinRegistry.h"
#include "RegKeyGuard.h"
#include "Win32Exception.h"

namespace actein
{
    using WinReg = utils::WinRegistry;

    const std::string RegistrySettings::REGISTRY_ACTEIN_KEY = "Software\\Actein\\Actein PC Client";
    const std::string RegistrySettings::REG_VALUE_STEAM_ACCOUNT_NAME = "SteamAccountName";
    const std::string RegistrySettings::REG_VALUE_STEAM_ACCOUNT_PASSSWORD = "SteamAccountPassword";
    const std::string RegistrySettings::REG_VALUE_MQTT_BROKER_ADDRESS = "MqttBrokerAddress";
    const std::string RegistrySettings::REG_VALUE_BOOTH_ID = "BoothId";

    RegistrySettings::RegistrySettings()
    {
        utils::RegKeyGuard keyGuard(WinReg::ReadKey(HKEY_LOCAL_MACHINE, REGISTRY_ACTEIN_KEY));
        mSteamAccountName = WinReg::ReadStringValue(keyGuard.Get(), REG_VALUE_STEAM_ACCOUNT_NAME);
        mSteamAccountPassword = WinReg::ReadStringValue(keyGuard.Get(), REG_VALUE_STEAM_ACCOUNT_PASSSWORD);
        mBrokerHost = WinReg::ReadStringValue(keyGuard.Get(), REG_VALUE_MQTT_BROKER_ADDRESS);
        mBoothId = WinReg::ReadIntegerValue(keyGuard.Get(), REG_VALUE_BOOTH_ID);
        LONG status = keyGuard.Close();
        if (status != ERROR_SUCCESS)
        {
            throw utils::Win32Exception("Can not close registry key: " + REGISTRY_ACTEIN_KEY, status);
        }
    }
}