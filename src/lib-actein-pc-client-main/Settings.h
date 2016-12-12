#ifndef SETTINGS_H__
#define SETTINGS_H__

#include <string>
#include <WinUtils.h>

namespace actein
{
    class Settings
    {
    public:
        Settings()
            : mBoothId(0)
            , mSteamPath(u8"C:\\Program Files (x86)\\Steam") //default value
        {
            if (utils::IsWindows64bit())
            {
                mVrTutorialFullSubPath = VR_TUTORIAL_SUB_PATH + u8"win64\\" + VR_TUTORIAL_EXE_NAME;
            }
            else
            {
                mVrTutorialFullSubPath = VR_TUTORIAL_SUB_PATH + u8"win32\\" + VR_TUTORIAL_EXE_NAME;
            }
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

        virtual const std::string & GetSteamExeName() const
        {
            return STEAM_EXE_NAME;
        }

        virtual const std::string & GetSteamVrExeName() const
        {
            return STEAM_VR_EXE_NAME;
        }

        virtual const std::string & GetSteamVrDashboardExeName() const
        {
            return STEAM_VR_DASHBOARD_EXE_NAME;
        }

        virtual const std::string & GetVrTutorialExeName() const
        {
            return VR_TUTORIAL_EXE_NAME;
        }

        virtual std::string GetVrTutorialExePath() const
        {
            return mSteamPath + "\\" + mVrTutorialFullSubPath;
        }

        virtual std::string GetSteamExePath() const
        {
            return mSteamPath + "\\" + STEAM_EXE_NAME;
        }

        virtual ~Settings() = default;

    protected:
        int mBoothId;
        std::string mBrokerHost;
        std::string mSteamAccountName;
        std::string mSteamAccountPassword;
        std::string mSteamPath;

    private:
        std::string mVrTutorialFullSubPath;

        static const std::string STEAM_EXE_NAME;
        static const std::string STEAM_VR_EXE_NAME;
        static const std::string STEAM_VR_DASHBOARD_EXE_NAME;
        static const std::string VR_TUTORIAL_SUB_PATH;
        static const std::string VR_TUTORIAL_EXE_NAME;
    };
}

#endif //SETTINGS_H__
