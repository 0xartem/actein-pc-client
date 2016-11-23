#include "RegistrySettings.h"
#include "WinRegistry.h"
#include "RegKeyGuard.h"
#include "Win32Exception.h"

namespace actein
{
    using WinReg = utils::WinRegistry;

    const std::string RegistrySettings::REG_KEY_VALVE_STEAM = "Software\\Valve\\Steam";
    const std::string RegistrySettings::REG_VALUE_STEAM_INSTALL_PATH = "InstallPath";
    const std::string RegistrySettings::REG_KEY_ACTEIN_CLIENT = "Software\\Actein\\Actein PC Client";
    const std::string RegistrySettings::REG_VALUE_STEAM_ACCOUNT_NAME = "SteamAccountName";
    const std::string RegistrySettings::REG_VALUE_STEAM_ACCOUNT_PASSSWORD = "SteamAccountPassword";
    const std::string RegistrySettings::REG_VALUE_MQTT_BROKER_ADDRESS = "MqttBrokerAddress";
    const std::string RegistrySettings::REG_VALUE_BOOTH_ID = "BoothId";

    RegistrySettings::RegistrySettings()
    {
        utils::RegKeyGuard acteinKeyGd(WinReg::ReadKey(HKEY_LOCAL_MACHINE, REG_KEY_ACTEIN_CLIENT));
        mSteamAccountName = WinReg::ReadStringValue(acteinKeyGd.get(), REG_VALUE_STEAM_ACCOUNT_NAME);
        mSteamAccountPassword = WinReg::ReadStringValue(acteinKeyGd.get(), REG_VALUE_STEAM_ACCOUNT_PASSSWORD);
        mBrokerHost = WinReg::ReadStringValue(acteinKeyGd.get(), REG_VALUE_MQTT_BROKER_ADDRESS);
        mBoothId = WinReg::ReadIntegerValue(acteinKeyGd.get(), REG_VALUE_BOOTH_ID);
        LONG status = acteinKeyGd.close();
        if (status != ERROR_SUCCESS)
        {
            throw utils::Win32Exception("Can not close registry key: " + REG_KEY_ACTEIN_CLIENT, status);
        }

        utils::RegKeyGuard steamKeyGd(WinReg::ReadKey(HKEY_LOCAL_MACHINE, REG_KEY_VALVE_STEAM));
        mSteamPath = WinReg::ReadStringValue(steamKeyGd.get(), REG_VALUE_STEAM_INSTALL_PATH);
        status = steamKeyGd.close();
        if (status != ERROR_SUCCESS)
        {
            throw utils::Win32Exception("Can not close registry key: " + REG_KEY_VALVE_STEAM, status);
        }
    }
}