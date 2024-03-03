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

TEST(ResultTests, IsResult)
{
    using namespace ext;

    static_assert(is_result_v<result<int, char>>);
    static_assert(is_result_v<result<int, char>, ignore, char>);
    static_assert(is_result_v<result<int, char>, int>);
    static_assert(is_result_v<result<int, char>, int, char>);

    static_assert(!is_result_v<result<int, char>, ignore, short>);
    static_assert(!is_result_v<result<int, char>, short>);
    static_assert(!is_result_v<result<int, char>, short, char>);
    static_assert(!is_result_v<result<int, char>, int, short>);

    static_assert(!is_result_v<result<int, char>&>);
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

TEST(ResultTests, ValueMatches)
{
    using namespace ext;

    const auto starts_with_abc = [](const std::string& str){ return str.starts_with("abc"); };

    {
        result<std::string, int> r { "abc" };
        EXPECT_TRUE(r.value_matches(starts_with_abc));
    }

    {
        result<std::string, int> r { "vbc" };
        EXPECT_FALSE(r.value_matches(starts_with_abc));
    }

    {
        result<std::string, int> r { failure_tag, 11 };
        EXPECT_FALSE(r.value_matches(starts_with_abc));
    }

    {
        std::string value { "abc" };
        result<std::string&, int> r { value };
        EXPECT_TRUE(r.value_matches(starts_with_abc));
    }

    {
        std::string value { "_bc" };
        result<std::string&, int> r { value };
        EXPECT_FALSE(r.value_matches(starts_with_abc));
    }

    {
        result<std::string&, int> r { failure_tag, 11 };
        EXPECT_FALSE(r.value_matches(starts_with_abc));
    }
}

TEST(ResultTests, ErrorMatches)
{
    using namespace ext;

    const auto starts_with_abc = [](const std::string& str){ return str.starts_with("abc"); };

    {
        result<int, std::string> r { failure_tag, "abc" };
        EXPECT_TRUE(r.error_matches(starts_with_abc));
    }

    {
        result<int, std::string> r { failure_tag, "xbc" };
        EXPECT_FALSE(r.error_matches(starts_with_abc));
    }

    {
        result<int, std::string> r { 10 };
        EXPECT_FALSE(r.error_matches(starts_with_abc));
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

TEST(ResultTests, Bind_LvalueRefQualifier)
{
    using namespace ext;

    const auto take_prefix = [](const std::string& str)
    {
        if (str.size() < 3)
        {
            return result<std::string, int> { failure_tag, 11 };
        }
        return result<std::string, int> { str.substr(0, 3) };
    };

    int a = 0;
    const auto take_nothing = [&a]()
    {
        if (a < 0)
        {
            return result<std::string, int> { failure_tag, 11 };
        }
        return result<std::string, int> { "abc" };
    };

    // T - object
    {
        result<std::string, int> r1 { "abcd" };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<std::string, int> r1 { failure_tag, 7 };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        result<std::string, int> r1 { "a" };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - reference
    {
        std::string value { "abcd" };
        result<std::string&, int> r1 { value };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<std::string&, int> r1 { failure_tag, 7 };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        std::string value { "a" };
        result<std::string&, int> r1 { value };
        const auto r2 = r1.bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - void
    {
        result<void, int> r1 {};
        a = 0;
        const auto r2 = r1.bind(take_nothing);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = r1.bind(take_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        result<void, int> r1 {};
        a = -1;
        const auto r2 = r1.bind(take_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }
}

TEST(ResultTests, Bind_RvalueRefQualifier)
{
    using namespace ext;

    const auto take_prefix = [](std::string&& str)
    {
        if (str.size() < 3)
        {
            return result<std::string, int> { failure_tag, 11 };
        }
        return result<std::string, int> { str.substr(0, 3) };
    };

    int a = 0;
    const auto take_nothing = [&a]()
    {
        if (a < 0)
        {
            return result<std::string, int> { failure_tag, 11 };
        }
        return result<std::string, int> { "abc" };
    };

    // T - object
    {
        result<std::string, int> r1 { "abcd" };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<std::string, int> r1 { failure_tag, 7 };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        result<std::string, int> r1 { "a" };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - reference
    {
        std::string value { "abcd" };
        result<std::string&, int> r1 { value };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<std::string&, int> r1 { failure_tag, 7 };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        std::string value { "a" };
        result<std::string&, int> r1 { value };
        const auto r2 = std::move(r1).bind(take_prefix);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }

    // T - void
    {
        result<void, int> r1 {};
        a = 0;
        const auto r2 = std::move(r1).bind(take_nothing);
        ASSERT_TRUE(r2);
        EXPECT_EQ(r2.value(), "abc");
    }

    {
        result<void, int> r1 { failure_tag, 7 };
        const auto r2 = std::move(r1).bind(take_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 7);
    }

    {
        result<void, int> r1 {};
        a = -1;
        const auto r2 = std::move(r1).bind(take_nothing);
        ASSERT_FALSE(r2);
        EXPECT_EQ(r2.error(), 11);
    }
}

TEST(ResultTests, Apply_LvalueRefQualifier)
{
    using namespace ext;

    auto get_length = [](const std::string& str){ return str.size(); };
    auto get_random = [](){ return 11; };

    struct GetLength
    {
        auto operator()(const std::string& str){ return str.size(); }
    };

    struct GetRandom
    {
        auto operator()(){ return 11; }
    };

    // T - object
    {
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        const result<std::string, int>    r0 { "abc" };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_TRUE(r01);
        EXPECT_EQ(r01.value(), 3U);
        ASSERT_TRUE(r02);
        EXPECT_EQ(r02.value(), 3U);
        ASSERT_TRUE(r03);
        EXPECT_EQ(r03.value(), 3U);
        ASSERT_TRUE(r04);
        EXPECT_EQ(r04.value(), 3U);
    }

    {
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        const result<std::string, int>    r0 { failure_tag, 11 };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 11);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 11);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 11);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 11);
    }

    {
        result<decltype(get_length), int> r1 { failure_tag, 12 };
        result<GetLength, int>            r2 { failure_tag, 14 };
        const result<std::string, int>    r0 { "abc" };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 12);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 14);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 12);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 14);
    }

    // T - reference
    {
        std::string value { "abc" };
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        const result<std::string&, int>   r0 { value };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_TRUE(r01);
        EXPECT_EQ(r01.value(), 3U);
        ASSERT_TRUE(r02);
        EXPECT_EQ(r02.value(), 3U);
        ASSERT_TRUE(r03);
        EXPECT_EQ(r03.value(), 3U);
        ASSERT_TRUE(r04);
        EXPECT_EQ(r04.value(), 3U);
    }

    {
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        const result<std::string&, int>   r0 { failure_tag, 11 };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 11);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 11);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 11);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 11);
    }

    {
        std::string value { "abc" };
        result<decltype(get_length), int> r1 { failure_tag, 12 };
        result<GetLength, int>            r2 { failure_tag, 14 };
        const result<std::string&, int>   r0 { value };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 12);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 14);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 12);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 14);
    }

    // T - void
    {
        result<decltype(get_random), int> r1 { get_random };
        result<GetRandom, int>            r2 { GetRandom() };
        const result<void, int>           r0 {};
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_TRUE(r01);
        EXPECT_EQ(r01.value(), 11);
        ASSERT_TRUE(r02);
        EXPECT_EQ(r02.value(), 11);
        ASSERT_TRUE(r03);
        EXPECT_EQ(r03.value(), 11);
        ASSERT_TRUE(r04);
        EXPECT_EQ(r04.value(), 11);
    }

    {
        result<decltype(get_random), int> r1 { get_random };
        result<GetRandom, int>            r2 { GetRandom() };
        const result<void, int>           r0 { failure_tag, 11 };
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 11);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 11);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 11);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 11);
    }

    {
        result<decltype(get_random), int> r1 { failure_tag, 12 };
        result<GetRandom, int>            r2 { failure_tag, 14 };
        const result<void, int>           r0 {};
        const auto r01 = r0.apply(r1);
        const auto r02 = r0.apply(r2);
        const auto r03 = r0.apply(std::move(r1));
        const auto r04 = r0.apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 12);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 14);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 12);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 14);
    }
}

TEST(ResultTests, Apply_RvalueRefQualifier)
{
    using namespace ext;

    auto get_length = [](std::string&& str){ return str.size(); };
    auto get_random = [](){ return 11; };

    struct GetLength
    {
        auto operator()(std::string&& str){ return str.size(); }
    };

    struct GetRandom
    {
        auto operator()(){ return 11; }
    };

    // T - object
    {
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        result<std::string, int>          r10 { "abc" };
        result<std::string, int>          r20 { "abc" };
        result<std::string, int>          r30 { "abc" };
        result<std::string, int>          r40 { "abc" };
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_TRUE(r01);
        EXPECT_EQ(r01.value(), 3U);
        ASSERT_TRUE(r02);
        EXPECT_EQ(r02.value(), 3U);
        ASSERT_TRUE(r03);
        EXPECT_EQ(r03.value(), 3U);
        ASSERT_TRUE(r04);
        EXPECT_EQ(r04.value(), 3U);
    }

    {
        result<decltype(get_length), int> r1 { get_length };
        result<GetLength, int>            r2 { GetLength() };
        result<std::string, int>          r10 { failure_tag, 11 };
        result<std::string, int>          r20 { failure_tag, 11 };
        result<std::string, int>          r30 { failure_tag, 11 };
        result<std::string, int>          r40 { failure_tag, 11 };
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 11);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 11);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 11);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 11);
    }

    {
        result<decltype(get_length), int> r1 { failure_tag, 12 };
        result<GetLength, int>            r2 { failure_tag, 14 };
        result<std::string, int>          r10 { "abc" };
        result<std::string, int>          r20 { "abc" };
        result<std::string, int>          r30 { "abc" };
        result<std::string, int>          r40 { "abc" };
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 12);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 14);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 12);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 14);
    }

    // TODO: T - reference (add get_value with const qualifier)

    // T - void
    {
        result<decltype(get_random), int> r1 { get_random };
        result<GetRandom, int>            r2 { GetRandom() };
        result<void, int>                 r10 {};
        result<void, int>                 r20 {};
        result<void, int>                 r30 {};
        result<void, int>                 r40 {};
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_TRUE(r01);
        EXPECT_EQ(r01.value(), 11);
        ASSERT_TRUE(r02);
        EXPECT_EQ(r02.value(), 11);
        ASSERT_TRUE(r03);
        EXPECT_EQ(r03.value(), 11);
        ASSERT_TRUE(r04);
        EXPECT_EQ(r04.value(), 11);
    }

    {
        result<decltype(get_random), int> r1 { get_random };
        result<GetRandom, int>            r2 { GetRandom() };
        result<void, int>                 r10 { failure_tag, 11 };
        result<void, int>                 r20 { failure_tag, 11 };
        result<void, int>                 r30 { failure_tag, 11 };
        result<void, int>                 r40 { failure_tag, 11 };
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 11);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 11);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 11);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 11);
    }

    {
        result<decltype(get_random), int> r1 { failure_tag, 12 };
        result<GetRandom, int>            r2 { failure_tag, 14 };
        result<void, int>                 r10 {};
        result<void, int>                 r20 {};
        result<void, int>                 r30 {};
        result<void, int>                 r40 {};
        const auto r01 = std::move(r10).apply(r1);
        const auto r02 = std::move(r20).apply(r2);
        const auto r03 = std::move(r30).apply(std::move(r1));
        const auto r04 = std::move(r40).apply(std::move(r2));
        ASSERT_FALSE(r01);
        EXPECT_EQ(r01.error(), 12);
        ASSERT_FALSE(r02);
        EXPECT_EQ(r02.error(), 14);
        ASSERT_FALSE(r03);
        EXPECT_EQ(r03.error(), 12);
        ASSERT_FALSE(r04);
        EXPECT_EQ(r04.error(), 14);
    }
}

static int sum1(int a, int b){ return a + b; }

TEST(ResultTests, Lift)
{
    using namespace ext;

    {
        auto sum = result_lift<char>(sum1);
        result<int, char> r1 { 1 };
        result<int, char> r2 { 2 };
        const auto r3 = sum(r1, r2);
        ASSERT_TRUE(r3);
        EXPECT_EQ(r3.value(), 3);
    }

    {
        auto sum2 = +[](int a, int b){ return a + b; };
        auto sum = result_lift<char>(sum2);
        result<int, char> r1 { 1 };
        result<int, char> r2 { 2 };
        const auto r3 = sum(r1, r2);
        ASSERT_TRUE(r3);
        EXPECT_EQ(r3.value(), 3);
    }

    {
        auto concat = result_lift<int>(+[](const std::string& lhs, const std::string& rhs) -> std::string { return lhs + rhs; });
        result<std::string, int> r1 { "abc" };
        const result<std::string, int> r2 { "def" };
        const auto r3 = concat(r1, r2);
        ASSERT_TRUE(r3);
        EXPECT_EQ(r3.value(), "abcdef");
    }

    {
        auto concat = result_lift<int>(+[](const std::string& lhs, const std::string& rhs) -> std::string { return lhs + rhs; });
        result<std::string, int> r1 { "abc" };
        const result<std::string, int> r2 { failure_tag, 11 };
        const auto r3 = concat(r1, r2);
        ASSERT_FALSE(r3);
        EXPECT_EQ(r3.error(), 11);
    }

    {
        auto concat = result_lift<int>(+[](const std::string& lhs, const std::string& rhs) -> std::string { return lhs + rhs; });
        result<std::string, int> r1 { "abc" };
        const auto r3 = concat(r1, result<std::string, int>{ "def" });
        ASSERT_TRUE(r3);
        EXPECT_EQ(r3.value(), "abcdef");
    }
}
