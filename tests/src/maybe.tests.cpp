/**
 * @file    maybe.tests.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    26.05.2021
 */

#include <type_traits>
#include <string>

#include <gtest/gtest.h>

#include <ext/maybe.hpp>

using namespace ext;

TEST(MaybeTests, ConstructNoArgs)
{
    {
        Maybe<int> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }

    {
        Maybe<const int> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }

    {
        Maybe<int*> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }

    {
        Maybe<const int*> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }

    {
        Maybe<int&> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }

    {
        Maybe<const int&> maybe {};
        ASSERT_TRUE(maybe.isNone());
        ASSERT_FALSE(maybe);
    }
}

TEST(MaybeTests, CopyConstructor)
{
    {
        auto value = 10;
        Maybe<int> maybe { value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int> maybe { value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<int*> maybe { &value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), &value);
    }

    {
        auto value = 10;
        Maybe<const int*> maybe { &value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), &value);
    }

    {
        auto value = 10;
        Maybe<int&> maybe { value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int&> maybe { value };
        Maybe other_maybe { maybe };
        ASSERT_EQ(other_maybe.value(), value);
    }
}

TEST(MaybeTests, MoveConstructor)
{
    {
        auto value = 10;
        Maybe<int> maybe { value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int> maybe { value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<int*> maybe { &value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), &value);
    }

    {
        auto value = 10;
        Maybe<const int*> maybe { &value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), &value);
    }

    {
        auto value = 10;
        Maybe<int&> maybe { value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int&> maybe { value };
        Maybe other_maybe { std::move(maybe) };
        ASSERT_EQ(other_maybe.value(), value);
    }
}

TEST(MaybeTests, CopyAssignment)
{
    {
        auto value = 10;
        Maybe<int> m1 { value };
        Maybe<int> m2 = m1;
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int> m1 { value };
        Maybe<const int> m2 = m1;
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<int*> m1 { &value };
        Maybe<int*> m2 = m1;
        ASSERT_EQ(m2.value(), &value);
    }

    {
        auto value = 10;
        Maybe<const int*> m1 { &value };
        Maybe<const int*> m2 = m1;
        ASSERT_EQ(m2.value(), &value);
    }

    {
        auto value = 10;
        Maybe<int&> m1 { value };
        Maybe<int&> m2 = m1;
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int&> m1 { value };
        Maybe<const int&> m2 = m1;
        ASSERT_EQ(m2.value(), value);
    }
}

TEST(MaybeTests, MoveAssignment)
{
    {
        auto value = 10;
        Maybe<int> m1 { value };
        Maybe<int> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int> m1 { value };
        Maybe<const int> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<int*> m1 { &value };
        Maybe<int*> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), &value);
    }

    {
        auto value = 10;
        Maybe<const int*> m1 { &value };
        Maybe<const int*> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), &value);
    }

    {
        auto value = 10;
        Maybe<int&> m1 { value };
        Maybe<int&> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int&> m1 { value };
        Maybe<const int&> m2 = std::move(m1);
        ASSERT_EQ(m2.value(), value);
    }
}

TEST(MaybeTests, AssignValue)
{
    {
        auto value = 10;
        Maybe<int> m1;
        m1 = value;
        ASSERT_EQ(m1.value(), value);
    }

    {
        auto value = 10;
        Maybe<int*> m1;
        m1 = &value;
        ASSERT_EQ(m1.value(), &value);
    }

    {
        auto value = 10;
        Maybe<const int*> m1;
        m1 = &value;
        ASSERT_EQ(m1.value(), &value);
    }

    {
        auto value = 10;
        Maybe<int&> m1;
        m1 = value;
        ASSERT_EQ(m1.value(), value);
    }

    {
        auto value = 10;
        Maybe<const int&> m1;
        m1 = value;
        ASSERT_EQ(m1.value(), value);
    }
}

TEST(MaybeTests, GetValue)
{
    {
        try
        {
            Maybe<int> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            Maybe<const int> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            Maybe<int*> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            Maybe<const int*> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            Maybe<int&> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            Maybe<const int&> m;
            m.value();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<int> m { value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<const int> m { value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<int*> m { &value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<const int*> m { &value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<int&> m { value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            auto value = 10;
            Maybe<const int&> m { value };
            m.value();
        }
        catch (...)
        {
            FAIL();
        }
    }
}

TEST(MaybeTests, GetValueOrDefaultValue)
{
    {
        auto value = 10;
        Maybe<int> m;
        const auto& r = m.valueOr(value);
        ASSERT_EQ(r, value);
    }

    {
        auto value = 10;
        Maybe<const int> m;
        const auto& r = m.valueOr(value);
        ASSERT_EQ(r, value);
    }

    {
        auto value = 10;
        Maybe<int*> m;
        const auto r = m.valueOr(&value);
        ASSERT_EQ(r, &value);
    }

    {
        auto value = 10;
        Maybe<const int*> m;
        const auto r = m.valueOr(&value);
        ASSERT_EQ(r, &value);
    }

    {
        auto value = 10;
        Maybe<int&> m;
        const auto& r = m.valueOr(value);
        ASSERT_EQ(r, value);
    }

    {
        auto value = 10;
        Maybe<const int&> m;
        const auto& r = m.valueOr(value);
        ASSERT_EQ(r, value);
    }
}

TEST(MaybeTests, Map)
{
    auto fn = [](const std::string& str){ return str.size(); };

    {
        Maybe<std::string> m1 { "xyz" };
        const auto r1 = m1.map(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<std::string> m2;
        const auto r2 = m2.map(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        Maybe<const std::string> m1 { "xyz" };
        const auto r1 = m1.map(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<const std::string> m2;
        const auto r2 = m2.map(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        std::string str = "xyz";

        Maybe<std::string&> m1 { str };
        const auto r1 = m1.map(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<std::string&> m2;
        const auto r2 = m2.map(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        std::string str = "xyz";

        Maybe<const std::string&> m1 { str };
        const auto r1 = m1.map(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<const std::string&> m2;
        const auto r2 = m2.map(fn);
        ASSERT_TRUE(r2.isNone());
    }
}

TEST(MaybeTests, Bind)
{
    auto fn = [](const std::string& str){ return Maybe<std::size_t>(str.size()); };

    {
        Maybe<std::string> m1 { "xyz" };
        const auto r1 = m1.bind(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<std::string> m2;
        const auto r2 = m2.bind(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        Maybe<const std::string> m1 { "xyz" };
        const auto r1 = m1.bind(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<const std::string> m2;
        const auto r2 = m2.bind(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        std::string str = "xyz";

        Maybe<std::string&> m1 { str };
        const auto r1 = m1.bind(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<std::string&> m2;
        const auto r2 = m2.bind(fn);
        ASSERT_TRUE(r2.isNone());
    }

    {
        std::string str = "xyz";

        Maybe<const std::string&> m1 { str };
        const auto r1 = m1.bind(fn);
        ASSERT_TRUE(r1.isSome());
        ASSERT_EQ(r1.value(), 3U);

        Maybe<const std::string&> m2;
        const auto r2 = m2.bind(fn);
        ASSERT_TRUE(r2.isNone());
    }
}

TEST(MaybeTests, Mutate)
{
    auto fn = [](std::string& str){ str.clear(); };

    {
        Maybe<std::string> m1 { "xyz" };
        m1.mutate(fn);
        ASSERT_TRUE(m1.value().empty());

        Maybe<std::string> m2;
        m2.mutate(fn);
        ASSERT_TRUE(m2.isNone());
    }

    {
        std::string str = "xyz";

        Maybe<std::string&> m1 { str };
        m1.mutate(fn);
        ASSERT_TRUE(m1.value().empty());
        ASSERT_TRUE(str.empty());

        Maybe<std::string> m2;
        m2.mutate(fn);
        ASSERT_TRUE(m2.isNone());
    }
}

TEST(MaybeTests, Comparison)
{
    {
        auto value = 10;
        Maybe<int> m1 { value };
        Maybe<int> m2 { value };
        Maybe<int> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, value);
    }

    {
        auto value = 10;
        Maybe<const int> m1 { value };
        Maybe<const int> m2 { value };
        Maybe<const int> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, value);
    }

    {
        auto value = 10;
        Maybe<int*> m1 { &value };
        Maybe<int*> m2 { &value };
        Maybe<int*> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, &value);
    }

    {
        auto value = 10;
        Maybe<const int*> m1 { &value };
        Maybe<const int*> m2 { &value };
        Maybe<const int*> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, &value);
    }

    {
        auto value = 10;
        Maybe<int&> m1 { value };
        Maybe<int&> m2 { value };
        Maybe<int&> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, value);
    }

    {
        auto value = 10;
        Maybe<const int&> m1 { value };
        Maybe<const int&> m2 { value };
        Maybe<const int&> m3;
        ASSERT_EQ(m1, m2);
        ASSERT_NE(m1, m3);
        ASSERT_NE(m1, none);
        ASSERT_EQ(m1, value);
    }
}
