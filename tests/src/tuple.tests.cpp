/**
 * @file    tuple.tests.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    23.06.2021
 */

#include <type_traits>
#include <sstream>

EXT_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
EXT_DISABLE_WARNINGS_POP

#include <ext/tuple.hpp>

using namespace std::string_literals;

TEST(TupleTests, ForEach)
{
    const auto t = std::make_tuple(20, "21"s);
    std::stringstream stream;
    ext::foreach(t, [&stream](const auto& value){ stream << value; });
    const auto str = stream.str();
    ASSERT_EQ(str, "2021");
}

TEST(TupleTests, FindIf)
{
    const auto t = std::make_tuple(20, "21"s, 30);
    const auto [found, index] =
        ext::findIf(t, [](const auto& value)
            {
                bool found = false;
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>, int>)
                    found = value == 30;
                return found;
            });

    ASSERT_TRUE(found);
    ASSERT_EQ(index, 2U);
}

TEST(TupleTests, Transform)
{
    const auto t1 = std::make_tuple(20, "21"s);
    const auto t2 =
        ext::transform(t1, [](const auto& value)
            {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>, std::string>)
                    return value;
                else
                    return value  + 1;
            });

    ASSERT_EQ(std::tuple_size_v<decltype(t2)>, 2U);
    ASSERT_EQ(std::get<0>(t2), 21);
    ASSERT_EQ(std::get<1>(t2), "21");
}

TEST(TupleTests, AllOf)
{
    const auto t = std::make_tuple(20, "21"s, 30);
    const auto found =
        ext::allOf(t, [](const auto& value)
            {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>, std::string>)
                    return std::stoi(value) > 10;
                else
                    return value > 10;
            });

    ASSERT_TRUE(found);
}

TEST(TupleTests, AnyOf)
{
    const auto t = std::make_tuple(20, "21"s, 30);
    const auto found =
        ext::anyOf(t, [](const auto& value)
            {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>, std::string>)
                    return std::stoi(value) >= 30;
                else
                    return value >= 30;
            });

    ASSERT_TRUE(found);
}

TEST(TupleTests, NoneOf)
{
    const auto t = std::make_tuple(20, "21"s, 30);
    const auto not_found =
        ext::noneOf(t, [](const auto& value)
            {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>, std::string>)
                    return std::stoi(value) >= 40;
                else
                    return value >= 40;
            });

    ASSERT_TRUE(not_found);
}
