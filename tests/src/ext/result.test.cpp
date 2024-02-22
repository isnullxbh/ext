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
        // result<std::string&, int> r {};
    }

    {
        result<const void, int> r {};
    }
}
