/**
 * @file    fixed_string.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    06.01.2024
 */

#include <gtest/gtest.h>

#include <ext/fixed_string.hpp>

namespace
{
    template<auto Str>
    struct Foo
    {
        static constexpr auto value = Str;
    };
}

TEST(FixedStringTests, ConstructFromStringLiteral)
{
    using namespace ext;
    constexpr basic_fixed_string str { "abc" };
    static_assert(str.size() == 3);
    static_assert(str == "abc");
}

TEST(FixedStringTests, CopyConstruction)
{
    using namespace ext;
    constexpr basic_fixed_string s1 { "abc" };
    constexpr basic_fixed_string s2 { s1 };
    static_assert(s2.size() == 3);
    static_assert(s1 == s2);
}

TEST(FixedStringTests, AssignCharacterArray)
{
    using namespace ext;
    constexpr auto s1 = []
    {
        basic_fixed_string s { "abc" };
        s = "def";
        return s;
    }();
    static_assert(s1 == "def");
}

TEST(FixedStringTests, CopyAssignment)
{
    using namespace ext;
    constexpr auto s1 = []
    {
        basic_fixed_string s1 { "abc" };
        basic_fixed_string s2 { "def" };
        s1 = s2;
        return s1;
    }();
    static_assert(s1 == "def");
}

TEST(FixedStringTests, At)
{
    using namespace ext;

    {
        constexpr basic_fixed_string str { "abc" };
        static_assert(str.at(2) == 'c');
        EXPECT_THROW(str.at(3), std::out_of_range);
    }

    {
        basic_fixed_string str { "abc" };
        EXPECT_EQ(str.at(2), 'c');
        EXPECT_THROW(str.at(3), std::out_of_range);
    }
}

TEST(FixedStringTests, CompileTimeAt)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc" };
        static_assert(s.at<0>() == 'a');
        static_assert(s.at<2>() == 'c');
    }

    {
        constexpr auto test = []
        {
            basic_fixed_string s { "abc" };
            return s.at<0>() == 'a' && s.at<1>() == 'b' && s.at<2>() == 'c';
        };
        static_assert(test());
    }
}

TEST(FixedStringTests, Front)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc" };
        static_assert(s.front() == 'a');
    }

    {
        basic_fixed_string s { "abc" };
        EXPECT_EQ(s.front(), 'a');
    }
}

TEST(FixedStringTests, Back)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc" };
        static_assert(s.back() == 'c');
    }

    {
        basic_fixed_string s { "abc" };
        EXPECT_EQ(s.back(), 'c');
    }
}

TEST(FixedStringTests, ConvertToStringView)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc" };
        const std::string_view       v { s };
        ASSERT_EQ(v.size(), 3U);
        EXPECT_EQ(v, "abc");
    }

    {
        constexpr basic_fixed_string s { "" };
        const std::string_view       v { s };
        ASSERT_EQ(v.size(), 0U);
        EXPECT_EQ(v, "");
    }
}

TEST(FixedStringTests, Iterators)
{
    using namespace ext;

    {
        constexpr auto test = []
        {
            auto passed = true;
            basic_fixed_string s { "abc" };
            auto it = s.begin();
            passed &= *it++ == 'a';
            passed &= *it++ == 'b';
            passed &= *it++ == 'c';
            passed &= it == s.end();
            return passed;
        };
        static_assert(test());
    }

    {
        constexpr auto test = []
        {
            auto passed = true;
            const basic_fixed_string s { "abc" };
            auto it = s.begin();
            passed &= *it++ == 'a';
            passed &= *it++ == 'b';
            passed &= *it++ == 'c';
            passed &= it == s.end();
            return passed;
        };
        static_assert(test());
    }

    {
        constexpr auto test = []
        {
            auto passed = true;
            const basic_fixed_string s { "abc" };
            auto it = s.cbegin();
            passed &= *it++ == 'a';
            passed &= *it++ == 'b';
            passed &= *it++ == 'c';
            passed &= it == s.cend();
            return passed;
        };
        static_assert(test());
    }
}

TEST(FixedStringTests, Empty)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "" };
        static_assert(s.empty());
    }

    {
        constexpr basic_fixed_string s { "abc" };
        static_assert(!s.empty());
    }
}

TEST(FixedStringTests, Size)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "" };
        static_assert(s.size() == 0);
    }

    {
        constexpr basic_fixed_string s { "abc" };
        static_assert(s.size() == 3);
    }

    {
        constexpr basic_fixed_string s { "abc\0abc" };
        static_assert(s.size() == 7);
    }
}

TEST(FixedStringTests, Map)
{
    using namespace ext;

    {
        constexpr auto to_upper = [](char c){ return static_cast<char>(c - 32); };
        constexpr basic_fixed_string s1 { "abc" };
        constexpr auto s2 = s1.map(to_upper);
        static_assert(s2 == "ABC");
    }
}

TEST(FixedStringTests, StartsWith)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc123abc" };
        static_assert(s.starts_with("abc"));
        static_assert(!s.starts_with("aba"));
    }

    {
        constexpr basic_fixed_string s { "" };
        static_assert(s.starts_with(""));
    }
}

TEST(FixedStringTests, EndsWith)
{
    using namespace ext;

    {
        constexpr basic_fixed_string s { "abc123abc" };
        static_assert(s.ends_with("abc"));
        static_assert(!s.ends_with("3bc"));
    }

    {
        constexpr basic_fixed_string s { "" };
        static_assert(s.ends_with(""));
    }
}

TEST(FixedStringTests, AsTemplateParameter)
{
    using namespace ext;
    using F1 = Foo<make_fixed_string("abc")>;
}

TEST(FixedStringTests, UserDefinedLiteral)
{
    using namespace ext;

    {
        constexpr auto s = "abc"_fs;
        static_assert(s.size() == 3);
        static_assert(s == "abc");
    }

    {
        constexpr auto s = U"abc"_fs;
        static_assert(s.size() == 3);
        static_assert(s == U"abc");
    }
}

TEST(FixedStringTests, MakeFixedString)
{
    using namespace ext;
    constexpr auto s = make_fixed_string(U"abc");
    static_assert(s.size() == 3);
    static_assert(s == U"abc");
}
