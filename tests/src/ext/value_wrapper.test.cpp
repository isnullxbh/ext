/**
 * @file    value_wrapper.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    20.01.2024
 */

#include <string>

#include <gtest/gtest.h>

#include <ext/value_wrapper.hpp>

namespace
{
    struct A
    {
        A() = delete;
    };

    template<typename> struct B {};
}

TEST(ValueWrapperTests, IsValueWrapper)
{
    using namespace ext;
    static_assert(is_value_wrapper_v<value_wrapper<int>>);
    static_assert(is_value_wrapper_v<value_wrapper<int, int>>);
    static_assert(is_value_wrapper_v<value_wrapper<int, int, B>>);
    static_assert(!is_value_wrapper_v<int>);
}

TEST(ValueWrapperTests, DefaultConstruction)
{
    using namespace ext;
    constexpr value_wrapper<int> w {};
    static_assert(w.value() == decltype(w)::value_type());
}

TEST(ValueWrapperTests, CopyConstruction)
{
    using namespace ext;
    constexpr value_wrapper w1 { 10 };
    constexpr value_wrapper w2 { w1 };
    static_assert(w1.value() == w2.value());
}

TEST(ValueWrapperTests, MoveConstruction)
{
    using namespace ext;
    const std::string value { "abc" };
    value_wrapper w1 { value };
    value_wrapper w2 { std::move(w1) };
    EXPECT_EQ(w2.value(), value);
}

TEST(ValueWrapperTests, ConstructFromSingleArgument)
{
    using namespace ext;
    constexpr value_wrapper w { 11 };
    static_assert(w.value() == 11);
}

TEST(ValueWrapperTests, ConstructFromArgumentPack)
{
    using namespace ext;
    const value_wrapper<std::string> w { std::in_place, 3U, 'x' };
    EXPECT_EQ(w.value(), "xxx");
}

TEST(ValueWrapperTests, CopyAssignment)
{
    using namespace ext;
    constexpr auto test = []
    {
        const value_wrapper w1 { 10 };
        value_wrapper       w2 { 11 };
        w2 = w1;
        return w2;
    };
    static_assert(test().value() == 10);
}

TEST(ValueWrapperTests, MoveAssignment)
{
    using namespace ext;
    constexpr auto test = []
    {
        value_wrapper w1 { 10 };
        value_wrapper w2 { 11 };
        w2 = std::move(w1);
        return w2;
    };
    static_assert(test().value() == 10);
}

TEST(ValueWrapperTests, AssignValue)
{
    using namespace ext;
    constexpr auto test = []
    {
        value_wrapper w { 10 };
        w = 11;
        return w;
    };
    static_assert(test().value() == 11);
}

TEST(ValueWrapperTests, Comparison)
{
    using namespace ext;
    using vw = value_wrapper<int>;

    {
        constexpr value_wrapper<const int> vw1 { 10 };
        constexpr value_wrapper<int>       vw2 { 10 };
        static_assert(vw1 == vw2);
    }

    static_assert(vw(1) == vw(1));
    static_assert(vw(1) != vw(2));
    static_assert(vw(1) < vw(2));
    static_assert(vw(1) <= vw(2));
    static_assert(vw(2) > vw(1));
    static_assert(vw(2) >= vw(1));

    static_assert(vw(1) == 1);
    static_assert(vw(1) != 2);
    static_assert(vw(1) < 2);
    static_assert(vw(1) <= 2);
    static_assert(vw(2) > 1);
    static_assert(vw(2) >= 1);

    static_assert(1 == vw(1));
    static_assert(1 != vw(2));
    static_assert(1 < vw(2));
    static_assert(1 <= vw(2));
    static_assert(2 > vw(1));
    static_assert(2 >= vw(1));
}

TEST(ValueWrapperTests, AdditionExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::addition>;

    {
        constexpr auto r = []
        {
            wrapper w { 2 };
            w += wrapper(3);
            return w;
        }();
        static_assert(r == 5);
    }

    {
        constexpr auto r = wrapper { 2 } + wrapper { 3 };
        static_assert(r == 5);
    }
}

TEST(ValueWrapperTests, SubtractionExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::subtraction>;

    {
        constexpr auto r = []
        {
            wrapper w { 5 };
            w -= wrapper(3);
            return w;
        }();
        static_assert(r == 2);
    }

    {
        constexpr auto r = wrapper { 5 } - wrapper { 3 };
        static_assert(r == 2);
    }
}

TEST(ValueWrapperTests, MultiplicationExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::multiplication>;

    {
        constexpr auto r = []
        {
            wrapper w { 2 };
            w *= wrapper { 3 };
            return w;
        }();
        static_assert(r == 6);
    }

    {
        constexpr auto r = wrapper { 2 } * wrapper { 3 };
        static_assert(r == 6);
    }
}

TEST(ValueWrapperTests, DivisionExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::division>;

    {
        constexpr auto r = []
        {
            wrapper w { 6 };
            w /= wrapper { 2 };
            return w;
        }();
        static_assert(r == 3);
    }

    {
        constexpr auto r = wrapper { 6 } / wrapper { 2 };
        static_assert(r == 3);
    }
}

TEST(ValueWrapperTests, RemainderExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::remainder>;

    {
        constexpr auto r = []
        {
            wrapper w { 7 };
            w %= wrapper { 2 };
            return w;
        }();
        static_assert(r == 1);
    }

    {
        constexpr auto r = wrapper { 7 } % wrapper { 2 };
        static_assert(r == 1);
    }
}

TEST(ValueWrapperTests, BitwiseExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::bitwise>;

    {
        constexpr auto r = []
        {
            wrapper w1 { 6 };
            wrapper w2 { 4 };
            w1 &= w2;
            return w1;
        }();
        static_assert(r == 4);
    }

    {
        constexpr auto r = []
        {
            wrapper w1 { 6 };
            wrapper w2 { 1 };
            w1 |= w2;
            return w1;
        }();
        static_assert(r == 7);
    }

    {
        constexpr auto r = []
        {
            wrapper w1 { 6 };
            wrapper w2 { 4 };
            w1 ^= w2;
            return w1;
        }();
        static_assert(r == 2);
    }

    {
        constexpr auto r = []
        {
            wrapper w1 { 2 };
            wrapper w2 { 1 };
            w1 <<= w2;
            return w1;
        }();
        static_assert(r == 4);
    }

    {
        constexpr auto r = []
        {
            wrapper w1 { 2 };
            wrapper w2 { 1 };
            w1 >>= w2;
            return w1;
        }();
        static_assert(r == 1);
    }

    {
        constexpr value_wrapper<unsigned char, void, value_wrapper_extensions::bitwise> w { static_cast<unsigned char>(254) };
        static_assert(~w == 1);
    }

    {
        constexpr auto r = wrapper { 3 } & wrapper { 1 };
        static_assert(r.value() == 1);
    }

    {
        constexpr auto r = wrapper { 2 } | wrapper { 1 };
        static_assert(r.value() == 3);
    }

    {
        constexpr auto r = wrapper { 2 } ^ wrapper { 1 };
        static_assert(r.value() == 3);
    }

    {
        constexpr auto r = wrapper { 2 } << wrapper { 1 };
        static_assert(r.value() == 4);
    }

    {
        constexpr auto r = wrapper { 2 } >> wrapper { 1 };
        static_assert(r.value() == 1);
    }
}

TEST(ValueWrapperTests, ArithmeticExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::arithmetic>;

    constexpr auto r1 = []
    {
        wrapper w { 2 };
        return +w;
    }();
    static_assert(r1 == 2);

    constexpr auto r2 = []
    {
        wrapper w { 2 };
        return -w;
    }();
    static_assert(r2 == -2);
}

TEST(ValueWrapperTests, IncrementExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::increment>;

    {
        constexpr auto r = []() -> std::pair<wrapper::value_type, wrapper>
        {
            wrapper w { 2 };
            return { ++w.value(), w };
        }();
        static_assert(r.first == 3 && r.second == 3);
    }

    {
        constexpr auto r = []() -> std::pair<wrapper, wrapper>
        {
            wrapper w { 2 };
            return { w++, w };
        }();
        static_assert(r.first == 2 && r.second == 3);
    }
}

TEST(ValueWrapperTests, DecrementExtension)
{
    using namespace ext;
    using wrapper = value_wrapper<int, void, value_wrapper_extensions::decrement>;

    {
        constexpr auto r = []() -> std::pair<wrapper::value_type, wrapper>
        {
            wrapper w { 2 };
            return { --w.value(), w };
        }();
        static_assert(r.first == 1 && r.second == 1);
    }

    {
        constexpr auto r = []() -> std::pair<wrapper, wrapper>
        {
            wrapper w { 6 };
            return { w--, w };
        }();
        static_assert(r.first == 6 && r.second == 5);
    }
}

TEST(ValueWrapperTests, Swap)
{
    using namespace ext;

    constexpr auto case1 = []
    {
        value_wrapper<int> vw1 { 10 };
        value_wrapper<int> vw2 { 11 };
        vw1.swap(vw2);
        return vw1 == 11 && vw2 == 10;
    };
    static_assert(case1());
}
