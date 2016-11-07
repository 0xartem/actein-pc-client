#pragma once

#include <public/steam/steam_api.h>

class DlcInstallationHandler
{
public:
    DlcInstallationHandler();
private:
    STEAM_CALLBACK(DlcInstallationHandler, OnDlcInstalled, DlcInstalled_t, m_CallbackDlcInstalled);
};

/*bool res = SteamAPI_RestartAppIfNecessary(0);
if (res)
{
return 1;
}
res = SteamAPI_Init();

if (!SteamUser()->BLoggedOn())
{
std::cout << "User is not logged on" << std::endl;
}

bool installedShouldBeF = SteamApps()->BIsAppInstalled(457320);
if (!installedShouldBeF)
{
SteamApps()->InstallDLC(457320);
}
bool installedTrue = SteamApps()->BIsAppInstalled(392190);

while (true)
{
SteamAPI_RunCallbacks();
std::this_thread::sleep_for(std::chrono::seconds(3));
}

SteamAPI_Shutdown();*/
