#ifndef STRING_UTILS_H__
#define STRING_UTILS_H__

#include <string>
#include <vector>

namespace utils
{
    std::string wstring2string(const std::wstring & str);
    std::string wstrBuf2string(const std::vector<wchar_t> & buffer);
    std::wstring string2wstring(const std::string & str);
}

#endif //STRING_UTILS_H__
