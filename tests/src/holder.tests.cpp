/**
 * @file    holder.tests.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

#include <type_traits>

EXT_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
EXT_DISABLE_WARNINGS_POP

#include <ext/holder.hpp>
#include <ext/utility_classes.hpp>

using namespace ext;
using namespace ext::testing;

TEST(HolderTests, ConstructNoArgs)
{
    ASSERT_TRUE(std::is_default_constructible_v<Holder<int>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<const int>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<int*>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<const int*>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<int&>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<const int&>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<ClassHasDefaultCtor>>);
    ASSERT_TRUE(std::is_default_constructible_v<Holder<const ClassHasDefaultCtor>>);

    Holder<int> h1 {};
    ASSERT_EQ(h1.value(), 0);

    Holder<const int> h2 {};
    ASSERT_EQ(h2.value(), 0);

    Holder<int*> h3 {};
    ASSERT_EQ(h3.value(), nullptr);

    Holder<const int*> h4 {};
    ASSERT_EQ(h4.value(), nullptr);

    try
    {
        Holder<int&> h5;
        h5.value();
        FAIL();
    }
    catch (const std::runtime_error&)
    {
        SUCCEED();
    }

    try
    {
        Holder<const int&> h6;
        h6.value();
        FAIL();
    }
    catch (const std::runtime_error&)
    {
        SUCCEED();
    }

    ASSERT_FALSE(std::is_default_constructible_v<Holder<ClassHasNoDefaultCtor>>);
    ASSERT_FALSE(std::is_default_constructible_v<Holder<const ClassHasNoDefaultCtor>>);
}

TEST(HolderTests, CopyConstruction)
{
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<int>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<const int>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<int*>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<const int*>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<int&>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<const int&>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<ClassHasCopyCtor>>);
    ASSERT_TRUE(std::is_copy_constructible_v<Holder<const ClassHasCopyCtor>>);

    auto value = 10;

    Holder<int> h1 { value };
    Holder<int> h1_cp { h1 };
    ASSERT_EQ(h1.value(), h1_cp.value());

    Holder<const int> h2 { value };
    Holder<const int> h2_cp { h2 };
    ASSERT_EQ(h2.value(), h2_cp.value());

    Holder<int*> h3 { &h1.value() };
    Holder<int*> h3_cp { h3 };
    ASSERT_EQ(h3.value(), h3_cp.value());

    Holder<const int*> h4 { &h1.value() };
    Holder<const int*> h4_cp { h4 };
    ASSERT_EQ(h4.value(), h4_cp.value());

    Holder<int&> h5 { value };
    Holder<int&> h5_cp { h5 };
    ASSERT_EQ(h5.value(), h5_cp.value());

    Holder<const int&> h6 { value };
    Holder<const int&> h6_cp { h6 };
    ASSERT_EQ(h6.value(), h6_cp.value());

    ASSERT_FALSE(std::is_copy_constructible_v<Holder<ClassHasNoCopyCtor>>);
    ASSERT_FALSE(std::is_copy_constructible_v<Holder<const ClassHasNoCopyCtor>>);
}

TEST(HolderTests, MoveConstruction)
{
    ASSERT_TRUE(std::is_move_constructible_v<Holder<int>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<const int>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<int*>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<const int*>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<int&>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<const int&>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<ClassHasMoveCtor>>);
    ASSERT_TRUE(std::is_move_constructible_v<Holder<const ClassHasMoveCtor>>);

    auto value = 10;

    Holder<int> h1 { value };
    Holder<int> h1_cp { std::move(h1) };
    ASSERT_EQ(h1_cp.value(), value);

    Holder<const int> h2 { value };
    Holder<const int> h2_cp { std::move(h2) };
    ASSERT_EQ(h2_cp.value(), value);

    Holder<int*> h3 { &h1.value() };
    Holder<int*> h3_cp { std::move(h3) };
    ASSERT_EQ(h3_cp.value(), &h1.value());

    Holder<const int*> h4 { &h1.value() };
    Holder<const int*> h4_cp { std::move(h4) };
    ASSERT_EQ(h4_cp.value(), &h1.value());

    Holder<int&> h5 { value };
    Holder<int&> h5_cp { std::move(h5) };
    ASSERT_EQ(h5_cp.value(), value);

    Holder<const int&> h6 { value };
    Holder<const int&> h6_cp { std::move(h6) };
    ASSERT_EQ(h6_cp.value(), value);

    ASSERT_FALSE(std::is_move_constructible_v<Holder<ClassHasNoMoveCtor>>);
    ASSERT_FALSE(std::is_move_constructible_v<Holder<const ClassHasNoMoveCtor>>);
}

TEST(HolderTests, ConstructWithArgs)
{
    ASSERT_TRUE((std::is_constructible_v<Holder<int>, int>));
    ASSERT_TRUE((std::is_constructible_v<Holder<int>, int&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<int>, int&&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int>, int>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int>, int&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int>, int&&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<int*>, int*>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int*>, int*>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int*>, const int*>));
    ASSERT_TRUE((std::is_constructible_v<Holder<int&>, int&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int&>, int&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int&>, const int&>));
    ASSERT_TRUE((std::is_constructible_v<Holder<const int&>, int>));    // Ok, const extends value lifetime
    ASSERT_TRUE((std::is_constructible_v<Holder<const int&>, int&&>));  // Ok, const extends value lifetime

    auto value = 10;

    Holder<int> h1 { value };
    ASSERT_EQ(h1.value(), value);

    Holder<const int> h2 { value };
    ASSERT_EQ(h2.value(), value);

    Holder<int*> h3 { &value };
    ASSERT_EQ(h3.value(), &value);

    Holder<const int*> h4 { &value };
    ASSERT_EQ(h4.value(), &value);

    Holder<int&> h5 { value };
    ASSERT_EQ(h5.value(), value);

    Holder<const int&> h6 { value };
    ASSERT_EQ(h6.value(), value);

    ASSERT_FALSE((std::is_constructible_v<Holder<int&>, int>));
    ASSERT_FALSE((std::is_constructible_v<Holder<int&>, const int&>));
    ASSERT_FALSE((std::is_constructible_v<Holder<int&>, int&&>));
    ASSERT_FALSE((std::is_constructible_v<Holder<int*>, const int*>));
}

TEST(HolderTests, CopyAssignment)
{
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<int>>);
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<int*>>);
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<const int*>>);
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<int&>>);
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<const int&>>);
    ASSERT_TRUE(std::is_copy_assignable_v<Holder<ClassHasCopyCtor>>);

    auto value = 10;

    Holder<int> h1;
    Holder<int> h2 { value };
    h1 = h2;
    ASSERT_EQ(h1.value(), value);

    Holder<int*> h3;
    Holder<int*> h4 { &value };
    h3 = h4;
    ASSERT_EQ(h3.value(), &value);

    Holder<const int*> h5;
    Holder<const int*> h6 { &value };
    h5 = h6;
    ASSERT_EQ(h5.value(), &value);

    Holder<int&> h7;
    Holder<int&> h8 { value };
    h7 = h8;
    ASSERT_EQ(h7.value(), value);

    Holder<const int&> h9;
    Holder<const int&> h10 { value };
    h9 = h10;
    ASSERT_EQ(h9.value(), value);

    ASSERT_FALSE(std::is_copy_assignable_v<Holder<const int>>);
    ASSERT_FALSE(std::is_copy_assignable_v<Holder<const ClassHasCopyCtor>>);
    ASSERT_FALSE(std::is_copy_assignable_v<Holder<ClassHasNoCopyCtor>>);
    ASSERT_FALSE(std::is_copy_assignable_v<Holder<const ClassHasNoCopyCtor>>);
}

TEST(HolderTests, MoveAssignment)
{
    ASSERT_TRUE(std::is_move_assignable_v<Holder<int>>);
    ASSERT_TRUE(std::is_move_assignable_v<Holder<int*>>);
    ASSERT_TRUE(std::is_move_assignable_v<Holder<const int*>>);
    ASSERT_TRUE(std::is_move_assignable_v<Holder<int&>>);
    ASSERT_TRUE(std::is_move_assignable_v<Holder<const int&>>);
    ASSERT_TRUE(std::is_move_assignable_v<Holder<ClassHasMoveCtor>>);

    auto value = 10;

    Holder<int> h1;
    Holder<int> h2 { value };
    h1 = std::move(h2);
    ASSERT_EQ(h1.value(), value);

    Holder<int*> h3;
    Holder<int*> h4 { &value };
    h3 = std::move(h4);
    ASSERT_EQ(h3.value(), &value);

    Holder<const int*> h5;
    Holder<const int*> h6 { &value };
    h5 = std::move(h6);
    ASSERT_EQ(h5.value(), &value);

    Holder<int&> h7;
    Holder<int&> h8 { value };
    h7 = std::move(h8);
    ASSERT_EQ(h7.value(), value);

    Holder<const int&> h9;
    Holder<const int&> h10 { value };
    h9 = std::move(h10);
    ASSERT_EQ(h9.value(), value);

    ASSERT_FALSE(std::is_move_assignable_v<Holder<const int>>);
    ASSERT_FALSE(std::is_move_assignable_v<Holder<const ClassHasMoveCtor>>);
    ASSERT_FALSE(std::is_move_assignable_v<Holder<ClassHasNoCopyCtor>>);
    ASSERT_FALSE(std::is_move_assignable_v<Holder<const ClassHasNoCopyCtor>>);
}

TEST(HolderTests, AssignValue)
{
    ASSERT_TRUE((std::is_assignable_v<Holder<int>, int>));
    ASSERT_TRUE((std::is_assignable_v<Holder<int>, int&>));
    ASSERT_TRUE((std::is_assignable_v<Holder<int>, int&&>));
    ASSERT_TRUE((std::is_assignable_v<Holder<int*>, int*>));
    ASSERT_TRUE((std::is_assignable_v<Holder<const int*>, int*>));
    ASSERT_TRUE((std::is_assignable_v<Holder<const int*>, const int*>));
    ASSERT_TRUE((std::is_assignable_v<Holder<int&>, int&>));
    ASSERT_TRUE((std::is_assignable_v<Holder<const int&>, int&>));
    ASSERT_TRUE((std::is_assignable_v<Holder<const int&>, const int&>));
    ASSERT_TRUE((std::is_assignable_v<Holder<const int&>, int>));    // Ok, const extends value lifetime
    ASSERT_TRUE((std::is_assignable_v<Holder<const int&>, int&&>));  // Ok, const extends value lifetime

    auto value = 10;

    Holder<int> h1;
    h1 = value;
    ASSERT_EQ(h1.value(), value);

    Holder<int*> h2;
    h2 = &value;
    ASSERT_EQ(h2.value(), &value);

    Holder<const int*> h3;
    h3 = &value;
    ASSERT_EQ(h3.value(), &value);

    Holder<int&> h4;
    h4 = value;
    ASSERT_EQ(h4.value(), value);

    Holder<const int&> h5;
    h5 = value;
    ASSERT_EQ(h5.value(), value);

    ASSERT_FALSE((std::is_assignable_v<Holder<const int>, int>));
    ASSERT_FALSE((std::is_assignable_v<Holder<const int>, int&>));
    ASSERT_FALSE((std::is_assignable_v<Holder<const int>, int&&>));
    ASSERT_FALSE((std::is_assignable_v<Holder<int*>, const int*>));
    ASSERT_FALSE((std::is_assignable_v<Holder<int&>, int>));
    ASSERT_FALSE((std::is_assignable_v<Holder<int&>, const int&>));
    ASSERT_FALSE((std::is_assignable_v<Holder<int&>, int&&>));
}

TEST(HolderTests, ConvertToBoolean)
{
    auto value = 10;

    Holder<int> h1;
    ASSERT_TRUE(h1);

    Holder<const int> h2;
    ASSERT_TRUE(h2);

    Holder<int*> h3;
    ASSERT_FALSE(h3);

    Holder<int*> h4 { &value };
    ASSERT_TRUE(h4);

    Holder<const int*> h5;
    ASSERT_FALSE(h5);

    Holder<const int*> h6 { &value };
    ASSERT_TRUE(h6);

    Holder<int&> h7;
    ASSERT_FALSE(h7);

    Holder<int&> h8 { value };
    ASSERT_TRUE(h8);

    Holder<const int&> h9;
    ASSERT_FALSE(h9);

    Holder<const int&> h10 { value };
    ASSERT_TRUE(h10);
}

TEST(HolderTests, Comparision)
{
    auto value = 10;

    {
        Holder<int> h1;
        Holder<int> h2 { value };
        Holder<int> h3 { value };
        ASSERT_NE(h1, h2);
        ASSERT_EQ(h2, h3);
        ASSERT_EQ(h2, value);
    }

    {
        Holder<int*> h1;
        Holder<int*> h2 { &value };
        Holder<int*> h3 { &value };
        ASSERT_NE(h1, h2);
        ASSERT_EQ(h2, h3);
        ASSERT_EQ(h2, &value);
    }

    {
        Holder<const int*> h1;
        Holder<const int*> h2 { &value };
        Holder<const int*> h3 { &value };
        ASSERT_NE(h1, h2);
        ASSERT_EQ(h2, h3);
        ASSERT_EQ(h2, &value);
    }

    {
        Holder<int&> h1;
        Holder<int&> h2 { value };
        Holder<int&> h3 { value };
        ASSERT_NE(h1, h2);
        ASSERT_EQ(h2, h3);
        ASSERT_EQ(h2, value);
        ASSERT_EQ(h2, std::as_const(value));
    }

    {
        Holder<const int&> h1;
        Holder<const int&> h2 { value };
        Holder<const int&> h3 { value };
        ASSERT_NE(h1, h2);
        ASSERT_EQ(h2, h3);
        ASSERT_EQ(h2, value);
        ASSERT_EQ(h2, std::as_const(value));
    }
}
