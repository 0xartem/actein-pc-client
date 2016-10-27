#include "DlcInstallationHandler.h"

DlcInstallationHandler::DlcInstallationHandler()
    : m_CallbackDlcInstalled(this, &DlcInstallationHandler::OnDlcInstalled)
{

}

void DlcInstallationHandler::OnDlcInstalled(DlcInstalled_t * callback)
{
    AppId_t appId = callback->m_nAppID;
}