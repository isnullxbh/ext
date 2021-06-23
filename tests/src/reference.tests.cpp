/**
 * @file    reference.tests.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.06.2021
 */

EXT_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
EXT_DISABLE_WARNINGS_POP

#include <ext/reference.hpp>

using namespace ext;

TEST(ReferenceTests, Construct)
{
    {
        Reference<int> ref {};
        ASSERT_TRUE(ref.isNull());
        ASSERT_EQ(ref, nullref);
    }

    {
        int value = 10;
        Reference<int> ref { value };
        ASSERT_FALSE(ref.isNull());
        ASSERT_NE(ref, nullref);
        ASSERT_EQ(ref.get(), value);
    }

    {
        const int value = 10;
        Reference<const int> ref { value };
        ASSERT_FALSE(ref.isNull());
        ASSERT_NE(ref, nullref);
        ASSERT_EQ(ref.get(), value);
    }
}

TEST(ReferenceTests, Wrap)
{
    {
        auto value = 10;
        auto ref = ext::ref(value);
        ASSERT_TRUE(ref);
        ASSERT_FALSE(ref.isNull());
        ASSERT_EQ(ref, value);
    }

    {
        auto value = 10;
        auto ref = ext::cref(value);
        ASSERT_TRUE(ref);
        ASSERT_FALSE(ref.isNull());
        ASSERT_EQ(ref, value);
    }
}

TEST(ReferenceTests, Unwrap)
{
    {
        Reference<int> ref {};
        auto maybe = unwrap(ref);
        ASSERT_TRUE(maybe.isNone());
    }

    {
        auto value = 10;
        Reference ref { value };
        auto maybe = unwrap(ref);
        ASSERT_TRUE(maybe.isSome());
    }

    {
        const auto value = 10;
        Reference ref { value };
        auto maybe = unwrap(ref);
        ASSERT_TRUE(maybe.isSome());
    }
}

TEST(ReferenceTests, Get)
{
    {
        try
        {
            Reference<int> ref;
            ref.get();
            FAIL();
        }
        catch (const NullPointerException& ex)
        {
            SUCCEED() << ex.message();
        }
    }

    {
        try
        {
            auto value = 10;
            Reference<int> ref { value };
            ref.get();
            SUCCEED();
        }
        catch (const NullPointerException& ex)
        {
            FAIL();
        }
    }
}

TEST(ReferenceTests, Comparison)
{
    {
        Reference<int> ref1 {};
        Reference<int> ref2 {};
        ASSERT_EQ(ref1, ref2);
    }

    {
        auto value = 10;
        Reference<int> ref1 { value };
        Reference<int> ref2 { value };
        ASSERT_EQ(ref1 ,ref2);
    }

    {
        const auto value = 10;
        Reference<const int> ref1 { value };
        Reference<const int> ref2 { value };
        ASSERT_EQ(ref1 ,ref2);
    }
}
