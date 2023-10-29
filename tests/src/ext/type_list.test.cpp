/**
 * @file    type_list.test.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    24.10.2023
 */

#include <type_traits>

#include <gtest/gtest.h>

#include <ext/type_list.hpp>

TEST(TypeListTests, CheckSize)
{
    using namespace ext;
    static_assert(type_list<>::size == 0);
    static_assert(type_list<int>::size == 1);
    static_assert(type_list<int, char, int>::size == 3);
}

TEST(TypeListTests, PopFront)
{
    using namespace ext;
    using L0 = type_list<int, char>;
    using L1 = L0::pop_front;
    static_assert(L1::is_equal<type_list<char>>);
    using L2 = type_list<>::pop_front;
    static_assert(L2::empty);
}

TEST(TypeListTests, PopBack)
{
    using namespace ext;
    using L0 = type_list<int, char>;
    using L1 = L0::pop_back;
    static_assert(L1::is_equal<type_list<int>>);
    using L2 = type_list<>::pop_back;
    static_assert(L2::empty);
}

TEST(TypeListTests, PushBackNoType)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_back<>;
    static_assert(L1::is_equal<type_list<int>>);
}

TEST(TypeListTests, PushBackSingleType)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_back<char>;
    static_assert(L1::is_equal<type_list<int, char>>);
}

TEST(TypeListTests, PushBackMultipleTypes)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_back<char, int>;
    static_assert(L1::is_equal<type_list<int, char, int>>);
}

TEST(TypeListTests, PushFrontNoType)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_front<>;
    static_assert(L1::is_equal<type_list<int>>);
}

TEST(TypeListTests, PushFrontSingleType)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_front<char>;
    static_assert(L1::is_equal<type_list<char, int>>);
}

TEST(TypeListTests, PushFrontMultipleTypes)
{
    using namespace ext;
    using L0 = type_list<int>;
    using L1 = L0::push_front<char, double>;
    static_assert(L1::is_equal<type_list<char, double, int>>);
}

TEST(TypeListTests, IsEqual)
{
    using namespace ext;
    using L0 = type_list<>;
    static_assert(L0::is_equal<L0>);
    using L1 = type_list<int, char>;
    using L2 = type_list<char, int>;
    static_assert(L1::is_equal<L1>);
    static_assert(!L1::is_equal<L2>);
}

template<typename L, typename T>
struct front_inserter
{
    using type = L::template push_front<T>;
};

template<typename L, typename T>
struct back_inserter
{
    using type = L::template push_back<T>;
};

TEST(TypeListTests, Foldl)
{
    using namespace ext;
    using L0 = type_list<int, char, double>;
    using L1 = L0::foldl<type_list<>, front_inserter>;
    static_assert(L1::is_equal<type_list<double, char, int>>);
}

TEST(TypeListTests, Foldr)
{
    using namespace ext;
    using L0 = type_list<int, char, double>;
    using L1 = L0::foldr<type_list<>, back_inserter>;
    static_assert(L1::is_equal<type_list<double, char, int>>);
}

TEST(TypeListTests, Reverse)
{
    using namespace ext;
    using L0 = type_list<int, char, double>;
    using L1 = L0::reverse;
    static_assert(L1::is_equal<type_list<double, char, int>>);
}

TEST(TypeListTests, Filter)
{
    using namespace ext;
    using L0 = type_list<int, char, double, short, float>;
    using L1 = L0::filter<std::is_integral>;
    static_assert(L1::is_equal<type_list<int, char, short>>);
}

TEST(TypeListTests, RemoveIf)
{
    using namespace ext;
    using L0 = type_list<int, char, double, short, float>;
    using L1 = L0::remove_if<std::is_integral>;
    static_assert(L1::is_equal<type_list<double, float>>);
}

TEST(TypeListTests, Get)
{
    using namespace ext;
    using L0 = type_list<float, char, double, short>;
    static_assert(std::is_same_v<L0::get<0>, float>);
    static_assert(std::is_same_v<L0::get<1>, char>);
    static_assert(std::is_same_v<L0::get<2>, double>);
    static_assert(std::is_same_v<L0::get<3>, short>);
}

TEST(TypeListTests, Set)
{
    using namespace ext;
    using L0 = type_list<int, char, float>;
    static_assert(L0::set<0, short>::is_equal<type_list<short, char, float>>);
    static_assert(L0::set<2, short>::is_equal<type_list<int, char, short>>);
}

TEST(TypeListTests, Slice)
{
    using namespace ext;
    using L0 = type_list<int, char, float>;
    static_assert(L0::slice<0>::is_equal<L0>);
    static_assert(L0::slice<0, 1>::is_equal<type_list<int>>);
    static_assert(L0::slice<0, 3>::is_equal<L0>);
    static_assert(L0::slice<1, 2>::is_equal<type_list<char, float>>);
    static_assert(L0::slice<1, 0>::is_equal<type_list<>>);
}

TEST(TypeListTests, Split)
{
    using namespace ext;
    using L0 = type_list<int, char, float, short>;

    using r0 = L0::split<0>;
    static_assert(r0::get<0>::empty);
    static_assert(r0::get<1>::is_equal<L0>);

    using r1 = L0::split<2>;
    static_assert(r1::get<0>::is_equal<type_list<int, char>>);
    static_assert(r1::get<1>::is_equal<type_list<float, short>>);

    using r2 = L0::split<4>;
    static_assert(r2::get<0>::is_equal<L0>);
    static_assert(r2::get<1>::empty);
}

TEST(TypeListTests, Map)
{
    using namespace ext;
    using l0 = type_list<int, char, float>;
    using r0 = l0::map<std::add_lvalue_reference>;
    static_assert(r0::is_equal<type_list<int&, char&, float&>>);
}

TEST(TypeListTests, Contains)
{
    using namespace ext;
    using l0 = type_list<int, char, float>;
    static_assert(l0::contains<int>);
    static_assert(l0::contains<float>);
    static_assert(!l0::contains<double>);
}

TEST(TypeListTests, IndexOf)
{
    using namespace ext;
    using l0 = type_list<int, char, float, int>;
    static_assert(l0::index_of<int> == 0);
    static_assert(l0::index_of<float> == 2);
    static_assert(l0::index_of<short> == l0::npos);
    static_assert(l0::index_of<int, 1> == 3);
    static_assert(l0::index_of<int, 4> == l0::npos);
}

TEST(TypeListTests, Concat)
{
    using namespace ext;
    using l0 = type_list<int, char>;
    using l1 = type_list<short, double>;
    using l3 = type_list<>;
    static_assert(l0::concat<l1>::is_equal<type_list<int, char, short, double>>);
    static_assert(l0::concat<l3>::is_equal<l0>);
}

TEST(TypeListTests, FindIf)
{
    using namespace ext;
    using l0 = type_list<int, char, float, int>;
    static_assert(l0::find_if<std::is_integral> == 0);
    static_assert(l0::find_if<std::is_integral, 1> == 1);
    static_assert(l0::find_if<std::is_integral, 2> == 3);
    static_assert(l0::find_if<std::is_integral, 4> == l0::npos);
}

template<typename T, typename U>
struct Less
{
    static constexpr auto value = sizeof(T) < sizeof(U);
};

template<typename T, typename U>
struct Great
{
    static constexpr auto value = sizeof(T) > sizeof(U);
};

TEST(TypeListTests, Sort)
{
    using namespace ext;

    {
        using l0 = type_list<short, char, double, signed>;
        using l1 = l0::sort<Less>;
        static_assert(l1::is_equal<type_list<char, short, int, double>>);
    }

    {
        using l0 = type_list<short, char, double, signed>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<double, int, short, char>>);
    }

    {
        using l0 = type_list<>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<>>);
    }

    {
        using l0 = type_list<int>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<int>>);
    }

    {
        using l0 = type_list<double, int>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<double, int>>);
    }

    {
        using l0 = type_list<double, double>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<double, double>>);
    }

    {
        using l0 = type_list<double, int, double>;
        using l1 = l0::sort<Great>;
        static_assert(l1::is_equal<type_list<double, double, int>>);
    }
}
