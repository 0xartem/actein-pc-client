#pragma once

#include <public/steam/steam_api.h>

class DlcInstallationHandler
{
public:
    DlcInstallationHandler();
private:
    STEAM_CALLBACK(DlcInstallationHandler, OnDlcInstalled, DlcInstalled_t, m_CallbackDlcInstalled);
};
