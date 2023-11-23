/**
 * @file    option.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.11.2023
 */

#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ext/option.hpp>

using namespace ext;

TEST(OptionTests, DefaultConstruction)
{
    {
        constexpr option<int> opt {};
        static_assert(!opt.has_value());
    }

    {
        constexpr option<const int*> opt {};
        static_assert(!opt.has_value());
    }

    {
        constexpr option<std::string> opt {};
        static_assert(!opt.has_value());
    }

    {
        constexpr option<int&> opt {};
        static_assert(!opt.has_value());
    }
}

TEST(OptionTests, ConstructFromNone)
{
    {
        constexpr option<int> opt { none };
        static_assert(!opt.has_value());
    }

    {
        constexpr option<const int*> opt { none };
        static_assert(!opt.has_value());
    }

    {
        constexpr option<std::string> opt { none };
        static_assert(!opt.has_value());
    }

    {
        constexpr option<int&> opt { none };
        static_assert(!opt.has_value());
    }
}

TEST(OptionTests, CopyConstruction)
{
    {
        constexpr option<int> opt1 { none };
        constexpr option<int> opt2 { opt1 };
        static_assert(opt1.has_value() == opt2.has_value());
    }

    {
        constexpr auto init { 11 };
        constexpr option<int> opt1 { init };
        constexpr option<int> opt2 { opt1 };
        static_assert(opt1.has_value() == opt2.has_value() && opt1.value() == opt2.value());
    }

    {
        constexpr option<const int*> opt1 { none };
        constexpr option<const int*> opt2 { opt1 };
        static_assert(opt1.has_value() == opt2.has_value());
    }

    {
        const auto init { 11 };
        option<const int*> opt1 { &init };
        option<const int*> opt2 { opt1 };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt1.value(), opt2.value());
    }

    {
        constexpr option<std::string> opt1 { none };
        constexpr option<std::string> opt2 { opt1 };
        static_assert(opt1.has_value() == opt2.has_value());
    }

    {
        const std::string init { "abc" };
        option<std::string> opt1 { init };
        option<std::string> opt2 { opt1 };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt1.value(), opt2.value());
    }

    {
        constexpr option<int&> opt1 { none };
        constexpr option<int&> opt2 { opt1 };
        static_assert(opt1.has_value() == opt2.has_value());
    }

    {
        const auto init { 11 };
        option<const int&> opt1 { init };
        option<const int&> opt2 { opt1 };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt1.value(), opt2.value());
    }
}

TEST(OptionTests, MoveConstruction)
{
    {
        option<int> opt1 { none };
        option<int> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
    }

    {
        constexpr auto init { 11 };
        option<int> opt1 { init };
        option<int> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt2.value(), init);
    }

    {
        option<const int*> opt1 { none };
        option<const int*> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
    }

    {
        constexpr auto init { 11 };
        option<const int*> opt1 { &init };
        option<const int*> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt2.value(), &init);
    }

    {
        option<std::string> opt1 { none };
        option<std::string> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
    }

    {
        const std::string init { "abc" };
        option<std::string> opt1 { init };
        option<std::string> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(opt2.value(), init);
    }

    {
        option<int&> opt1 { none };
        option<int&> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
    }

    {
        constexpr auto init { 11 };
        option<const int&> opt1 { init };
        option<const int&> opt2 { std::move(opt1) };
        EXPECT_EQ(opt1.has_value(), opt2.has_value());
        EXPECT_EQ(&opt2.value(), &init);
    }
}

TEST(OptionTests, ConstructFromArgs)
{
    {
        option<std::string> opt { std::in_place, 3, 'x' };
        EXPECT_EQ(opt.value(), "xxx");
    }

    {
        option<const std::string> opt { std::in_place, "xxx" };
        EXPECT_EQ(opt.value(), "xxx");
    }
}

TEST(OptionTests, ConstructFromOtherOption)
{
    {
        constexpr auto value { 11 };
        constexpr option<int>    opt1 { value };
        constexpr option<double> opt2 { opt1 };
        static_assert(opt2.value() == value);
    }

    {
        option<std::string_view> opt1 { "abc" };
        option<std::string>      opt2 { opt1 };
        EXPECT_EQ(opt1.value(), opt2.value());
    }

    {
        auto value { 11 };
        option<int&>       opt1 { value };
        option<const int&> opt2 { opt1 };
        EXPECT_EQ(&opt2.value(), &value);
    }

    {
        struct A {};
        struct B : A {};
        B value {};
        option<B&> opt1 { value };
        option<A&> opt2 { opt1 };
        EXPECT_EQ(&opt2.value(), &value);
    }
}
