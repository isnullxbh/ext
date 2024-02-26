/**
 * @file    result.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#include <string>

#include <gtest/gtest.h>

#include <ext/result.hpp>

TEST(ResultTests, FailureCTADs)
{
    using namespace ext;

    {
        int value {};
        failure f { value };
        static_assert(std::is_same_v<failure<int>, decltype(f)>);
    }

    {
        const int value {};
        failure f { value };
        static_assert(std::is_same_v<failure<const int>, decltype(f)>);
    }

    {
        volatile int value {};
        failure f { static_cast<volatile int&&>(value) };
        static_assert(std::is_same_v<failure<volatile int>, decltype(f)>);
    }
}

TEST(ResultTests, IsFailure)
{
    using namespace ext;

    static_assert(is_failure_v<failure<int>>);
    static_assert(is_failure_v<const failure<int>>);
    static_assert(is_failure_v<failure<int>, ignore>);
    static_assert(is_failure_v<failure<int>, int>);

    static_assert(!is_failure_v<failure<int>&>);
    static_assert(!is_failure_v<failure<int>, char>);
}

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
        EXPECT_EQ(r2.status(), result_status::success);
    }

    {
        result<int, std::string> r1 {};
        result<int, std::string> r2 { std::move(r1) };
        EXPECT_EQ(r2.status(), result_status::success);
    }

    {
        result<std::string, std::string> r1 {};
        result<std::string, std::string> r2 { std::move(r1) };
        EXPECT_EQ(r2.status(), result_status::success);
    }

    {
        result<int, int> r1 {};
        result<int, int> r2 { std::move(r1) };
        EXPECT_EQ(r2.status(), result_status::success);
    }

    {
        result<void, int> r1 {};
        result<void, int> r2 { std::move(r1) };
        EXPECT_EQ(r2.status(), result_status::success);
    }

    {
        result<void, std::string> r1 {};
        result<void, std::string> r2 { std::move(r1) };
        EXPECT_EQ(r2.status(), result_status::success);
    }
}

TEST(ResultTests, ConstructValue)
{
    using namespace ext;

    {
        result<int, std::string> r { 11 };
        EXPECT_TRUE(r);
        EXPECT_EQ(r.value(), 11);
    }

    {
        result<std::string, int> r { "abc" };
        EXPECT_TRUE(r);
        EXPECT_EQ(r.value(), "abc");
    }

    {
        auto value { 11 };
        result<int&, std::string> r { value };
        EXPECT_TRUE(r);
        EXPECT_EQ(&r.value(), &value);
    }

    {
        std::string value { "abc" };
        result<std::string&, int> r { value };
        EXPECT_TRUE(r);
        EXPECT_EQ(&r.value(), &value);
    }
}

TEST(ResultTests, ConstructErrorInPlace)
{
    using namespace ext;

    {
        result<int, std::string> r { failure_tag, 3U, 'x' };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), "xxx");
    }

    {
        result<std::string, int> r { failure_tag, 11 };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), 11);
    }

    {
        result<int&, std::string> r { failure_tag, 3U, 'x' };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), "xxx");
    }

    {
        result<std::string&, int> r { failure_tag, 11 };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), 11);
    }

    {
        result<void, std::string> r { failure_tag, 3U, 'x' };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), "xxx");
    }

    {
        result<void, int> r { failure_tag, 11 };
        EXPECT_FALSE(r);
        EXPECT_EQ(r.error(), 11);
    }
}

TEST(ResultTests, ConstructValueInPlace)
{
    using namespace ext;

    {
        result<int, std::string> r { std::in_place, 11 };
        EXPECT_TRUE(r);
        EXPECT_EQ(r.value(), 11);
    }

    {
        result<std::string, int> r { std::in_place, 3U, 'x' };
        EXPECT_TRUE(r);
        EXPECT_EQ(r.value(), "xxx");
    }

    {
        result<void, std::string> r { std::in_place };
        EXPECT_TRUE(r);
    }

    {
        result<void, int> r { std::in_place };
        EXPECT_TRUE(r);
    }
}
