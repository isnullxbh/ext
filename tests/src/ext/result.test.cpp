/**
 * @file    result.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#include <string>
#include <string_view>

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

TEST(ResultTests, ConvertingCopyConstructor)
{
    using namespace ext;

    struct A {};
    struct B : A {};

    {
        result<std::string_view, int> r1 { "abc" };
        result<std::string, int>      r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(r1.value(), r2.value());
    }

    {
        result<int, std::string_view> r1 { failure_tag, "abc" };
        result<int, std::string>      r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(r1.error(), r2.error());
    }

    {
        result<void, std::string_view> r1 {};
        result<void, std::string>      r2 { r1 };
        EXPECT_TRUE(r2);
    }

    {
        result<void, std::string_view> r1 { failure_tag, "abc" };
        result<void, std::string>      r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(r1.error(), r2.error());
    }

    {
        B value {};
        result<B&, int> r1 { value };
        result<A&, int> r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(&r1.value(), &r2.value());
    }

    {
        B value {};
        result<B&, int>       r1 { value };
        result<const A&, int> r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(&r1.value(), &r2.value());
    }

    {
        result<B&, int> r1 { failure_tag, 11 };
        result<A&, int> r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(r1.error(), r2.error());
    }

    {
        result<B&, int>       r1 { failure_tag, 11 };
        result<const A&, int> r2 { r1 };
        ASSERT_EQ(r1.status(), r2.status());
        EXPECT_EQ(r1.error(), r2.error());
    }
}

TEST(ResultTests, ConvertingMoveConstructor)
{
    using namespace ext;

    struct A {};
    struct B : A {};

    {
        result<std::string_view, int> r1 { "abc" };
        result<std::string, int>      r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::success);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<int, std::string_view> r1 { failure_tag, "abc" };
        result<int, std::string>      r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::failure);
        EXPECT_EQ(r2.error(), "abc");
    }

    {
        result<void, std::string_view> r1 {};
        result<void, std::string>      r2 { std::move(r1) };
        EXPECT_TRUE(r2);
    }

    {
        result<void, std::string_view> r1 { failure_tag, "abc" };
        result<void, std::string>      r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::failure);
        EXPECT_EQ(r2.error(), "abc");
    }

    {
        B value {};
        result<B&, int> r1 { value };
        result<A&, int> r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::success);
        EXPECT_EQ(&r2.value(), &value);
    }

    {
        B value {};
        result<B&, int>       r1 { value };
        result<const A&, int> r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::success);
        EXPECT_EQ(&r2.value(), &value);
    }

    {
        result<B&, int> r1 { failure_tag, 11 };
        result<A&, int> r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::failure);
        EXPECT_EQ(r2.error(), 11);
    }

    {
        result<B&, int>       r1 { failure_tag, 11 };
        result<const A&, int> r2 { std::move(r1) };
        ASSERT_EQ(r2.status(), result_status::failure);
        EXPECT_EQ(r2.error(), 11);
    }
}

TEST(ResultTests, ConstructFromFailure)
{
    using namespace ext;

    {
        failure<std::string>     f { "abc" };
        result<int, std::string> r { f };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), f.value());
    }

    {
        failure<std::string>      f { "abc" };
        result<int&, std::string> r { f };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), f.value());
    }

    {
        failure<std::string>      f { "abc" };
        result<void, std::string> r { f };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), f.value());
    }

    {
        failure<std::string>     f { "abc" };
        result<int, std::string> r { std::move(f) };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), "abc");
    }

    {
        failure<std::string>      f { "abc" };
        result<int&, std::string> r { std::move(f) };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), "abc");
    }

    {
        failure<std::string>      f { "abc" };
        result<void, std::string> r { std::move(f) };
        ASSERT_FALSE(r);
        EXPECT_EQ(r.error(), "abc");
    }
}

TEST(ResultTests, Map_LvalueRefQualifier)
{
    using namespace ext;

    const auto get_length = [](const std::string& str){ return str.size(); };
    const auto get_random = [](){ return 11; };
    const auto do_nothing = [](){};
    const auto use_string = [](const std::string&){};

    // T - object
    {
        result<std::string, int> r1 { "abc" };
        const auto r2 = r1.map(get_length);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 3U);
    }

    {
        result<std::string, int> r1 { failure_tag, 11 };
        const auto r2 = r1.map(get_length);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), r1.error());
    }

    {
        result<std::string, int> r1 { "abc" };
        const auto r2 = r1.map(use_string);
        ASSERT_TRUE(r2);
    }

    {
        result<std::string, int> r1 { failure_tag, 11 };
        const auto r2 = r1.map(use_string);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), r1.error());
    }

    // T - reference
    {
        std::string value { "abc" };
        result<std::string&, int> r1 { value };
        const auto r2 = r1.map(get_length);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 3U);
    }

    {
        result<std::string&, int> r1 { failure_tag, 11 };
        const auto r2 = r1.map(get_length);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    {
        std::string value { "abc" };
        result<std::string&, int> r1 { value };
        const auto r2 = r1.map(use_string);
        ASSERT_TRUE(r2);
    }

    {
        result<std::string&, int> r1 { failure_tag, 11 };
        const auto r2 = r1.map(use_string);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - void
    {
        result<void, int> r1 {};
        const auto r2 = r1.map(get_random);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 11);
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = r1.map(get_random);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), r1.error());
    }

    {
        result<void, int> r1 {};
        const auto r2 = r1.map(do_nothing);
        ASSERT_TRUE(r2);
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = r1.map(do_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), r1.error());
    }
}

TEST(ResultTests, Map_RvalueRefQualifier)
{
    using namespace ext;

    const auto get_length = [](std::string&& str){ return str.size(); };
    const auto get_random = [](){ return 11; };
    const auto do_nothing = [](){};
    const auto use_string = [](std::string&&){};

    // T - object
    {
        result<std::string, int> r1 { "abc" };
        const auto r2 = std::move(r1).map(get_length);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 3U);
    }

    {
        result<std::string, int> r1 { failure_tag, 11 };
        const auto r2 = std::move(r1).map(get_length);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    {
        result<std::string, int> r1 { "abc" };
        const auto r2 = std::move(r1).map(use_string);
        ASSERT_TRUE(r2);
    }

    {
        result<std::string, int> r1 { failure_tag, 11 };
        const auto r2 = std::move(r1).map(use_string);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - reference
    {
        std::string value { "abc" };
        result<std::string&, int> r1 { value };
        const auto r2 = std::move(r1).map(get_length);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 3U);
    }

    {
        result<std::string&, int> r1 { failure_tag, 11 };
        const auto r2 = std::move(r1).map(get_length);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    {
        std::string value { "abc" };
        result<std::string&, int> r1 { value };
        const auto r2 = std::move(r1).map(use_string);
        ASSERT_TRUE(r2);
    }

    {
        result<std::string&, int> r1 { failure_tag, 11 };
        const auto r2 = std::move(r1).map(use_string);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - void
    {
        result<void, int> r1 {};
        const auto r2 = std::move(r1).map(get_random);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), 11);
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = std::move(r1).map(get_random);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        result<void, int> r1 {};
        const auto r2 = std::move(r1).map(do_nothing);
        ASSERT_TRUE(r2);
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = std::move(r1).map(do_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }
}
