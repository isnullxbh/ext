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
        result<int, std::string> r {};
    }

    {
        result<std::string, std::string> r {};
    }

    {
        result<int, int> r {};
    }

    {
        result<const void, int> r {};
    }

    {
        result<const void, std::string> r {};
    }
}

TEST(ResultTests, CopyConstruction)
{
    using namespace ext;

    {
        result<std::string, int> r1 {};
        result<std::string, int> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }

    {
        result<int, std::string> r1 {};
        result<int, std::string> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }

    {
        result<std::string, std::string> r1 {};
        result<std::string, std::string> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }

    {
        result<int, int> r1 {};
        result<int, int> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }

    {
        result<void, int> r1 {};
        result<void, int> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }

    {
        result<void, std::string> r1 {};
        result<void, std::string> r2 { r1 };
        EXPECT_EQ(r1.status(), r2.status());
    }
}

TEST(ResultTests, MoveConstruction)
{
    using namespace ext;

    {
        result<std::string, int> r1 {};
        result<std::string, int> r2 { std::move(r1) };
    }

    {
        result<int, std::string> r1 {};
        result<int, std::string> r2 { std::move(r1) };
    }

    {
        result<std::string, std::string> r1 {};
        result<std::string, std::string> r2 { std::move(r1) };
    }

    {
        result<int, int> r1 {};
        result<int, int> r2 { std::move(r1) };
    }

    {
        result<void, int> r1 {};
        result<void, int> r2 { std::move(r1) };
    }

    {
        result<void, std::string> r1 {};
        result<void, std::string> r2 { std::move(r1) };
    }
}
