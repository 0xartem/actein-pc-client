#include <locale>
#include <codecvt>
#include "StringUtils.h"

namespace utils
{
    using wstring_utf8_converter = std::wstring_convert<std::codecvt_utf8<wchar_t> >;

    std::string wstring2string(const std::wstring & str)
    {
        wstring_utf8_converter converter;
        return converter.to_bytes(str);
    }

    std::string wstrBuf2string(const std::vector<wchar_t> & buffer)
    {
        if (buffer.empty())
        {
            return std::string();
        }
        wstring_utf8_converter converter;
        return converter.to_bytes(&buffer.front(), &buffer.back());
    }

    std::wstring string2wstring(const std::string & str)
    {
        wstring_utf8_converter converter;
        return converter.from_bytes(str);
    }
}