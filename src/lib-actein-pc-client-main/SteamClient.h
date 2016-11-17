#ifndef STEAM_CLIENT_H__
#define STEAM_CLIENT_H__

#include <chrono>

namespace actein
{
    class SteamClient
    {
    public:
        SteamClient();
        ~SteamClient();
        bool WaitForSteamUserToLogin(const std::chrono::milliseconds & duration);
        void Shutdown();
    };
}

#endif //STEAM_CLIENT_H__
