#ifndef WIN_REGISTRY_H__
#define WIN_REGISTRY_H__

#include <string>
#include <vector>
#include <windows.h>

namespace utils
{
    class WinRegistry
    {
    public:
        static HKEY ReadKey(HKEY rootKey, const std::string & subKey);
        static std::string ReadStringValue(HKEY parentKey, const std::string & valueName);
        static int ReadIntegerValue(HKEY parentKey, const std::string & valueName);

    private:
        template<typename CharType = char>
        static std::vector<CharType> ReadSpecificTypeValue(
            HKEY parentKey,
            const std::string & valueName,
            DWORD typeFlags,
            DWORD * resultType
        );
    };
}

#endif //WIN_REGISTRY_H__
