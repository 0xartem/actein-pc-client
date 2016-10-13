#ifndef ARRAY_UTILS_H__
#define ARRAY_UTILS_H__

#include <gtest/gtest.h>

namespace test_utils
{
    template<typename T>
    bool ArraysMatch(const T(&expected), const T(&actual))
    {
        if (expected.size() != actual.size())
        {
            return ::testing::AssertionFailure();
        }

        auto expIt = expected.begin();
        auto actIt = actual.begin();
        for (; expIt != expected.end(), actIt != actual.end(); ++expIt, ++actIt)
        {
            if (*expIt != *actIt)
            {
                return ::testing::AssertionFailure() << "actual"
                    << " (" << *actIt << ") != expected"
                    << " (" << *expIt << ")";
            }
        }

        return ::testing::AssertionSuccess();
    }
}

#endif //ARRAY_UTILS_H__
