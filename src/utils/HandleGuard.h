#ifndef HANDLE_GUARD_H__
#define HANDLE_GUARD_H__

#include <windows.h>

namespace utils
{
    class HandleGuard
    {
    public:
        HandleGuard()
            : mHandle(INVALID_HANDLE_VALUE)
        {
        }

        explicit HandleGuard(HANDLE handle)
            : mHandle(handle)
        {
        }

        HANDLE get() const
        {
            return mHandle;
        }

        HANDLE release()
        {
            HANDLE temp = mHandle;
            mHandle = INVALID_HANDLE_VALUE;
            return temp;
        }

        void reset(HANDLE newHandle)
        {
            close();
            mHandle = newHandle;
        }

        bool close()
        {
            if (mHandle != INVALID_HANDLE_VALUE)
            {
                return ::CloseHandle(mHandle) == TRUE;
            }
            return true;
        }

        ~HandleGuard()
        {
            close();
        }

    private:
        HandleGuard(const HandleGuard &) = delete;
        HandleGuard & operator=(const HandleGuard &) = delete;

    private:
        HANDLE mHandle;
    };
}

#endif //HANDLE_GUARD_H__
