#include <thread>
#include <stdexcept>
//#include <steam/steam_api.h>
#include "SteamClient.h"

namespace actein
{
    SteamClient::SteamClient()
    {
        /*if (!SteamAPI_Init())
        {
            throw std::runtime_error("Can not init Steam API");
        }*/
    }

    SteamClient::~SteamClient()
    {
        //Shutdown();
    }

    bool SteamClient::WaitForSteamUserToLogin(const std::chrono::milliseconds & duration)
    {
        /*static const std::chrono::milliseconds step(500);
        std::chrono::milliseconds durationLeft = duration;
        while (durationLeft > std::chrono::milliseconds::zero())
        {
            if (SteamUser()->BLoggedOn())
            {
                return true;
            }
            std::this_thread::sleep_for(step);
            durationLeft -= step;
        }*/
        return false;
    }

    void SteamClient::Shutdown()
    {
        //SteamAPI_Shutdown();
    }
}