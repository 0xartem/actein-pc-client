#ifndef SETTINGS_H__
#define SETTINGS_H__

#include <string>

namespace actein
{
    class Settings
    {
    public:
        Settings()
            : mBoothId(0)
            , mSteamExePath(u8"C:\\Program Files (x86)\\Steam\\Steam.exe")
        {
        }

        virtual int GetBoothId() const
        {
            return mBoothId;
        }

        virtual const std::string & GetBrokerHost() const
        {
            return mBrokerHost;
        }

        virtual const std::string & GetSteamAccountName() const
        {
            return mSteamAccountName;
        }

        virtual const std::string & GetSteamAccountPassword() const
        {
            return mSteamAccountPassword;
        }

        virtual const std::string & GetSteamExePath() const
        {
            return mSteamExePath;
        }

        virtual ~Settings() = default;

    protected:
        int mBoothId;
        std::string mBrokerHost;
        std::string mSteamAccountName;
        std::string mSteamAccountPassword;
        std::string mSteamExePath;
    };
}

#endif //SETTINGS_H__
