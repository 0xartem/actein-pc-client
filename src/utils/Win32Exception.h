#ifndef WIN32_EXCEPTION_H__
#define WIN32_EXCEPTION_H__

#include <stdexcept>

namespace utils
{
    class Win32Exception : public std::runtime_error
    {
    public:
        Win32Exception(const std::string & message, unsigned long winErrorCode)
            : std::runtime_error(message)
            , mWinErrorCode(winErrorCode)
        {
        }

        unsigned long GetWindowsErrorCode() const
        {
            return mWinErrorCode;
        }

    private:
        unsigned long mWinErrorCode;
    };
}

#endif //WIN32_EXCEPTION_H__