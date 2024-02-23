/**
 * @file    result.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#include <string>

#include <gtest/gtest.h>

#include <ext/result.hpp>

TEST(ResultTests, DefaultConstruction)
{
    using namespace ext;

    {
        result<std::string, int> r {};
    }

    {
        result<const void, int> r {};
    }
}

TEST(ResultTests, CopyConstruction)
{
    using namespace ext;

    {
        result<std::string, int> r1 {};
        result<std::string, int> r2 { r1 };
    }

    {
        result<int, std::string> r1 {};
        result<int, std::string> r2 { r1 };
    }

    {
        result<void, int> r1 {};
        result<void, int> r2 { r1 };
    }

    {
        result<void, std::string> r1 {};
        result<void, std::string> r2 { r1 };
    }
}
