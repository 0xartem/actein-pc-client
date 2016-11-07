#ifndef WIN32_EXCEPTION_H__
#define WIN32_EXCEPTION_H__

#include <stdexcept>
#include <sstream>

namespace utils
{
    class Win32Exception : public std::runtime_error
    {
    public:
        Win32Exception(const std::string & message, unsigned long winErrorCode)
            : std::runtime_error(message)
            , mWinErrorCode(winErrorCode)
        {
            mErrorMessage = message + "; Windows error code: " + std::to_string(mWinErrorCode);
        }

        const char * what() const override
        {
            return mErrorMessage.c_str();
        }

        unsigned long GetWindowsErrorCode() const
        {
            return mWinErrorCode;
        }

    private:
        unsigned long mWinErrorCode;
        std::string mErrorMessage;
    };
}

#endif //WIN32_EXCEPTION_H__