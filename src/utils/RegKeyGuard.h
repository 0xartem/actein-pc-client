#ifndef REG_KEY_GUARD_H__
#define REG_KEY_GUARD_H__

#include <windows.h>

namespace utils
{
    class RegKeyGuard
    {
    public:
        explicit RegKeyGuard(HKEY key)
            : mKey(key)
        {
        }

        HKEY Get()
        {
            return mKey;
        }

        LONG Close()
        {
            LONG status = ERROR_SUCCESS;
            if (mKey != NULL)
            {
                status = ::RegCloseKey(mKey);
                mKey = NULL;
            }
            return status;
        }

        ~RegKeyGuard()
        {
            Close();
        }

    private:
        RegKeyGuard(const RegKeyGuard &) = delete;
        RegKeyGuard & operator=(const RegKeyGuard &) = delete;

    private:
        HKEY mKey;
    };
}

#endif //REG_KEY_GUARD_H__
