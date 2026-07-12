#pragma once

#include <iostream>

namespace ddknd::tests
{
    inline int failures = 0;

    inline void TestCheckImpl(
        bool condition,
        const char* expression,
        const char* file,
        int line)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << expression
                      << " at " << file << ':' << line
                      << '\n';

            ++failures;
        }
    }
}

#define TEST_CHECK(condition)                                      \
    ::ddknd::tests::TestCheckImpl(                                 \
        static_cast<bool>(condition),                              \
        #condition,                                                \
        __FILE__,                                                  \
        __LINE__)