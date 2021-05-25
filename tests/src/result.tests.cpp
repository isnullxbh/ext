/**
 * @file    result.tests.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

#include <string>

#include <gtest/gtest.h>

#include <ext/result.hpp>

using namespace ext;

TEST(ResultTests, Construct)
{
    ASSERT_TRUE((std::is_constructible_v<Result<int, int>, Success<int>>));
    ASSERT_TRUE((std::is_constructible_v<Result<int, int>, Failure<int>>));

    {
        auto value = 10, error = 11;

        Result<int, int> r1 { Success<int>(value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), value);
        ASSERT_EQ(std::as_const(r1).value(), value);

        Result<int, int> r2 { Failure<int>(error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), error);
        ASSERT_EQ(std::as_const(r2).error(), error);
    }

    {
        auto value = 10, error = 11;

        Result<const int, const int> r1 { Success<const int>(value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), value);
        ASSERT_EQ(std::as_const(r1).value(), value);

        Result<const int, const int> r2 { Failure<const int>(error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), error);
        ASSERT_EQ(std::as_const(r2).error(), error);
    }

    {
        auto value = 10, error = 11;
        Result<int*, int*> r1 { Success<int*>(&value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), &value);
        ASSERT_EQ(std::as_const(r1).value(), &value);

        Result<int*, int*> r2 { Failure<int*>(&error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), &error);
        ASSERT_EQ(std::as_const(r2).error(), &error);
    }

    {
        auto value = 10, error = 11;
        Result<const int*, const int*> r1 { Success<const int*>(&value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), &value);
        ASSERT_EQ(std::as_const(r1).value(), &value);

        Result<const int*, const int*> r2 { Failure<const int*>(&error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), &error);
        ASSERT_EQ(std::as_const(r2).error(), &error);
    }

    {
        auto value = 10, error = 11;
        Result<int&, int&> r1 { Success<int&>(value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), value);
        ASSERT_EQ(std::as_const(r1).value(), value);

        Result<int&, int&> r2 { Failure<int&>(error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), error);
        ASSERT_EQ(std::as_const(r2).error(), error);
    }

    {
        auto value = 10, error = 11;
        Result<const int&, const int&> r1 { Success<const int&>(value) };
        ASSERT_TRUE(r1.success());
        ASSERT_TRUE(r1);
        ASSERT_EQ(r1.value(), value);
        ASSERT_EQ(std::as_const(r1).value(), value);

        Result<const int&, const int&> r2 { Failure<const int&>(error) };
        ASSERT_TRUE(r2.failure());
        ASSERT_FALSE(r2);
        ASSERT_EQ(r2.error(), error);
        ASSERT_EQ(std::as_const(r2).error(), error);
    }
}

TEST(ResultTests, CopyConstruction)
{
    {
        auto value = 10;

        Result<int, int> r1 { Success<int>(value) };
        Result<int, int> r2 { r1 };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int, const int> r3 { Success<const int>(value) };
        Result<const int, const int> r4 { r3 };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }

    {
        auto value = 10;

        Result<int*, int*> r1 { Success<int*>(&value) };
        Result<int*, int*> r2 { r1 };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), &value);

        Result<const int*, const int*> r3 { Success<const int*>(&value) };
        Result<const int*, const int*> r4 { r3 };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), &value);
    }

    {
        auto value = 10;

        Result<int&, int&> r1 { Success<int&>(value) };
        Result<int&, int&> r2 { r1 };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int&, const int&> r3 { Success<const int&>(value) };
        Result<const int&, const int&> r4 { r3 };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }
}

TEST(ResultTests, MoveConstruction)
{
    {
        auto value = 10;

        Result<int, int> r1 { Success<int>(value) };
        Result<int, int> r2 { std::move(r1) };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int, const int> r3 { Success<const int>(value) };
        Result<const int, const int> r4 { std::move(r3) };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }

    {
        auto value = 10;

        Result<int*, int*> r1 { Success<int*>(&value) };
        Result<int*, int*> r2 { std::move(r1) };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), &value);

        Result<const int*, const int*> r3 { Success<const int*>(&value) };
        Result<const int*, const int*> r4 { std::move(r3) };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), &value);
    }

    {
        auto value = 10;

        Result<int&, int&> r1 { Success<int&>(value) };
        Result<int&, int&> r2 { std::move(r1) };
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int&, const int&> r3 { Success<const int&>(value) };
        Result<const int&, const int&> r4 { std::move(r3) };
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }
}

TEST(ResultTests, CopyAssignment)
{
    {
        auto value = 10;

        Result<int, int> r1 { Success<int>(value) };
        Result<int, int> r2 { Failure<int>() };
        r2 = r1;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);
    }

    {
        auto value = 10;

        Result<int*, int*> r1 { Success<int*>(&value) };
        Result<int*, int*> r2 { Failure<int*>() };
        r2 = r1;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), &value);

        Result<const int*, const int*> r3 { Success<const int*>(&value) };
        Result<const int*, const int*> r4 { Failure<const int*>() };
        r4 = r3;
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), &value);
    }

    {
        auto value = 10;

        Result<int&, int&> r1 { Success<int&>(value) };
        Result<int&, int&> r2 { Failure<int&>() };
        r2 = r1;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int&, const int&> r3 { Success<const int&>(value) };
        Result<const int&, const int&> r4 { Failure<const int&>() };
        r4 = r3;
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }
}

TEST(ResultTests, MoveAssignment)
{
    {
        auto value = 10;

        Result<int, int> r1 { Success<int>(value) };
        Result<int, int> r2 { Failure<int>() };
        r2 = std::move(r1);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);
    }

    {
        auto value = 10;

        Result<int*, int*> r1 { Success<int*>(&value) };
        Result<int*, int*> r2 { Failure<int*>() };
        r2 = std::move(r1);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), &value);

        Result<const int*, const int*> r3 { Success<const int*>(&value) };
        Result<const int*, const int*> r4 { Failure<const int*>() };
        r4 = std::move(r3);
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), &value);
    }

    {
        auto value = 10;

        Result<int&, int&> r1 { Success<int&>(value) };
        Result<int&, int&> r2 { Failure<int&>() };
        r2 = std::move(r1);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), value);

        Result<const int&, const int&> r3 { Success<const int&>(value) };
        Result<const int&, const int&> r4 { Failure<const int&>() };
        r4 = std::move(r3);
        ASSERT_TRUE(r4.success());
        ASSERT_EQ(r4.value(), value);
    }
}

TEST(ResultTests, GetValueOrDefaultValue)
{
    {
        auto value = 10, def = 11;

        Result<int, int> r1 { Success<int>(value) };
        const auto v1 = r1.valueOr(def);
        ASSERT_EQ(v1, value);

        Result<int, int> r2 { Failure<int>(value) };
        const auto v2 = r2.valueOr(def);
        ASSERT_EQ(v2, def);
    }

    {
        auto value = 10, def = 11;

        Result<const int, const int> r1 { Success<const int>(value) };
        const auto v1 = r1.valueOr(def);
        ASSERT_EQ(v1, value);

        Result<const int, const int> r2 { Failure<const int>(value) };
        const auto v2 = r2.valueOr(def);
        ASSERT_EQ(v2, def);
    }

    {
        auto value = 10, def = 11;

        Result<int*, int*> r1 { Success<int*>(&value) };
        const auto v1 = r1.valueOr(&def);
        ASSERT_EQ(v1, &value);

        Result<int*, int*> r2 { Failure<int*>(&value) };
        const auto v2 = r2.valueOr(&def);
        ASSERT_EQ(v2, &def);
    }

    {
        auto value = 10, def = 11;

        Result<const int*, const int*> r1 { Success<const int*>(&value) };
        const auto v1 = r1.valueOr(&def);
        ASSERT_EQ(v1, &value);

        Result<const int*, const int*> r2 { Failure<const int*>(&value) };
        const auto v2 = r2.valueOr(&def);
        ASSERT_EQ(v2, &def);
    }

    {
        auto value = 10, def = 11;

        Result<int&, int&> r1 { Success<int&>(value) };
        const auto v1 = r1.valueOr(def);
        ASSERT_EQ(v1, value);

        Result<int&, int&> r2 { Failure<int&>(value) };
        const auto v2 = r2.valueOr(def);
        ASSERT_EQ(v2, def);
    }

    {
        auto value = 10, def = 11;

        Result<const int&, const int&> r1 { Success<const int&>(value) };
        const auto v1 = r1.valueOr(def);
        ASSERT_EQ(v1, value);

        Result<const int&, const int&> r2 { Failure<const int&>(value) };
        const auto v2 = r2.valueOr(def);
        ASSERT_EQ(v2, def);
    }
}

TEST(ResultTests, Comparison)
{
    {
        auto value = 10;

        Result<int, int> r1 { Success<int>(value) };
        Result<int, int> r2 { Success<int>(value) };
        Result<int, int> r3 { Failure<int>(value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, value);
        ASSERT_NE(r1, r3);
    }

    {
        auto value = 10;

        Result<const int, const int> r1 { Success<const int>(value) };
        Result<const int, const int> r2 { Success<const int>(value) };
        Result<const int, const int> r3 { Failure<const int>(value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, value);
        ASSERT_NE(r1, r3);
    }

    {
        auto value = 10;

        Result<int*, int*> r1 { Success<int*>(&value) };
        Result<int*, int*> r2 { Success<int*>(&value) };
        Result<int*, int*> r3 { Failure<int*>(&value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, &value);
        ASSERT_NE(r1, r3);
    }

    {
        auto value = 10;

        Result<const int*, const int*> r1 { Success<const int*>(&value) };
        Result<const int*, const int*> r2 { Success<const int*>(&value) };
        Result<const int*, const int*> r3 { Failure<const int*>(&value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, &value);
        ASSERT_NE(r1, r3);
    }

    {
        auto value = 10;

        Result<int&, int&> r1 { Success<int&>(value) };
        Result<int&, int&> r2 { Success<int&>(value) };
        Result<int&, int&> r3 { Failure<int&>(value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, value);
        ASSERT_NE(r1, r3);
    }

    {
        auto value = 10;

        Result<const int&, const int&> r1 { Success<const int&>(value) };
        Result<const int&, const int&> r2 { Success<const int&>(value) };
        Result<const int&, const int&> r3 { Failure<const int&>(value) };

        ASSERT_EQ(r1, r2);
        ASSERT_EQ(r1, value);
        ASSERT_NE(r1, r3);
    }
}

TEST(ResultTests, Map)
{
    {
        std::string str { "xxx" };
        Result<std::string, int> r1 { Success<std::string>(str) };
        const auto r2 = r1.map([](const std::string& str){ return str + "yyy"; });
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "xxxyyy");
    }

    {
        std::string str { "xxx" };
        Result<const std::string, int> r1 { Success<const std::string>(str) };
        const auto r2 = r1.map([](const std::string& str){ return str + "yyy"; });
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "xxxyyy");
    }

    {
        std::string str { "xxx" };
        Result<std::string&, int> r1 { Success<std::string&>(str) };
        const auto r2 = r1.map([](const std::string& str){ return str + "yyy"; });
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "xxxyyy");
    }

    {
        std::string str { "xxx" };
        Result<const std::string&, int> r1 { Success<const std::string&>(str) };
        const auto r2 = r1.map([](const std::string& str){ return str + "yyy"; });
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "xxxyyy");
    }

    {
        Result<std::string, int> r1 { Failure<int>(10) };
        const auto r2 = r1.map([](const std::string& str){ return str + "yyy"; });
        ASSERT_TRUE(r2.failure());
    }
}

TEST(ResultTests, Bind)
{
    const auto binder =
        [](const std::string& str) -> Result<std::size_t, int>
            {
                return Success<std::size_t>(str.size());
            };

    {
        std::string str { "xxx" };
        Result<std::string, int> r1 { Success<std::string>(str) };
        const auto r2 = r1.bind(binder);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), 3U);
    }

    {
        std::string str { "xxx" };
        Result<const std::string, int> r1 { Success<const std::string>(str) };
        const auto r2 = r1.bind(binder);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), 3U);
    }

    {
        std::string str { "xxx" };
        Result<std::string&, int> r1 { Success<std::string&>(str) };
        const auto r2 = r1.bind(binder);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), 3U);
    }

    {
        std::string str { "xxx" };
        Result<const std::string&, int> r1 { Success<const std::string&>(str) };
        const auto r2 = r1.bind(binder);
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), 3U);
    }

    {
        Result<std::string, int> r1 { Failure<int>(10) };
        const auto r2 = r1.bind(binder);
        ASSERT_TRUE(r2.failure());
    }
}

TEST(ResultTests, Mutate)
{
    {
        Result<std::string, int> r1 { Success<std::string>("xxx") };
        r1.mutate([](std::string& str){ str.append("yyy"); });
        ASSERT_EQ(r1.value(), "xxxyyy");
    }

    {
        std::string str { "xxx" };
        Result<std::string&, int> r1 { Success<std::string&>(str) };
        r1.mutate([](std::string& str){ str.append("yyy"); });
        ASSERT_EQ(r1.value(), "xxxyyy");
        ASSERT_EQ(str, "xxxyyy");
    }
}

TEST(ResultTests, RethrowException)
{
    {
        try
        {
            Result<int, std::runtime_error> r1 { Success<int>() };
            r1.rethrow();
        }
        catch (...)
        {
            FAIL();
        }
    }

    {
        try
        {
            Result<int, std::runtime_error> r1 { Failure<std::runtime_error>("") };
            r1.rethrow();
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
            auto ex = std::make_exception_ptr(std::runtime_error(""));
            Result<int, std::exception_ptr> r1 { Failure<std::exception_ptr>(ex) };
            r1.rethrow();
            FAIL();
        }
        catch (...)
        {
            SUCCEED();
        }
    }
}

TEST(ResultTests, BindOperator)
{
    auto fn = [](int n) -> Result<std::string, int>
        {
            return Success<std::string>(std::to_string(n));
        };

    {
        Result<int, int> r1 { Success<int>(10) };
        const auto r2 = r1 >>= fn;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "10");
    }

    {
        Result<const int, int> r1 { Success<const int>(10) };
        const auto r2 = r1 >>= fn;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "10");
    }

    {
        int a = 10;
        Result<int&, int> r1 { Success<int&>(a) };
        const auto r2 = r1 >>= fn;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "10");
    }

    {
        int a = 10;
        Result<const int&, int> r1 { Success<const int&>(a) };
        const auto r2 = r1 >>= fn;
        ASSERT_TRUE(r2.success());
        ASSERT_EQ(r2.value(), "10");
    }
}
