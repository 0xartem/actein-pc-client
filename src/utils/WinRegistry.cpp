#include <vector>
#include "StringUtils.h"
#include "Win32Exception.h"
#include "WinRegistry.h"

namespace utils
{
    HKEY WinRegistry::ReadKey(HKEY rootKey, const std::string & subKey)
    {
        REGSAM access = KEY_READ | KEY_QUERY_VALUE;

        HKEY resultKey = NULL;
        LONG status = ::RegOpenKeyEx(rootKey, string2wstring(subKey).c_str(), 0, access, &resultKey);
        if (status != ERROR_SUCCESS)
        {
            throw Win32Exception("Can not open registry key: " + subKey, status);
        }
        return resultKey;
    }

    std::string WinRegistry::ReadStringValue(HKEY parentKey, const std::string & valueName)
    {
        DWORD resultType = 0;
        std::vector<wchar_t> buffer = ReadSpecificTypeValue<wchar_t>(
            parentKey,
            valueName,
            RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ,
            &resultType
        );

        if (resultType != REG_SZ && resultType != REG_EXPAND_SZ)
        {
            throw Win32Exception("Value registry type is not a string: " + valueName, ::GetLastError());
        }
        return std::string(wstrBuf2string(buffer));
    }

    int WinRegistry::ReadIntegerValue(HKEY parentKey, const std::string & valueName)
    {
        DWORD resultType = 0;
        std::vector<char> buffer = ReadSpecificTypeValue(
            parentKey,
            valueName,
            RRF_RT_REG_DWORD,
            &resultType
        );

        if (resultType != REG_DWORD)
        {
            throw Win32Exception("Value registry type is not an integer: " + valueName, ::GetLastError());
        }

        int result = static_cast<int>(buffer.at(0));
        return result;
    }

    template<typename CharType>
    std::vector<CharType> WinRegistry::ReadSpecificTypeValue(
        HKEY parentKey,
        const std::string & valueName,
        DWORD typeFlags,
        DWORD * resultType)
    {
        DWORD bufferSize = MAX_PATH;
        std::vector<CharType> buffer(bufferSize);

        LONG status = ::RegGetValue(
            parentKey,
            NULL,
            string2wstring(valueName).c_str(),
            typeFlags,
            resultType,
            &buffer.at(0),
            &bufferSize
        );

        if (status == ERROR_MORE_DATA)
        {
            buffer.resize(bufferSize);
            status = ::RegGetValue(
                parentKey,
                NULL,
                string2wstring(valueName).c_str(),
                typeFlags,
                resultType,
                &buffer.at(0),
                &bufferSize
            );
        }

        if (status != ERROR_SUCCESS)
        {
            throw Win32Exception("Can not read registry value: " + valueName, status);
        }

        buffer.resize(bufferSize / sizeof(CharType));
        return buffer;
    }
}