/**
 * @file    utility_classes.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

#ifndef EXT_UTILITY_CLASSES_HPP
#define EXT_UTILITY_CLASSES_HPP

namespace ext {
namespace testing {

struct ClassHasDefaultCtor {};
struct ClassHasNoDefaultCtor { ClassHasNoDefaultCtor() = delete; };
struct ClassHasCopyCtor {};
struct ClassHasMoveCtor {};

struct ClassHasNoCopyCtor
{
    ClassHasNoCopyCtor(const ClassHasNoCopyCtor&) = delete;
    ClassHasNoCopyCtor& operator=(const ClassHasNoCopyCtor&) = delete;
};

struct ClassHasNoMoveCtor
{
    ClassHasNoMoveCtor(ClassHasNoMoveCtor&&) = delete;
    ClassHasNoMoveCtor& operator=(ClassHasNoMoveCtor&&) = delete;
};

struct ClassHasNoCopyCtorButMovable
{
    ClassHasNoCopyCtorButMovable() = default;
    ClassHasNoCopyCtorButMovable(const ClassHasNoCopyCtorButMovable&) = delete;
    ClassHasNoCopyCtorButMovable(ClassHasNoCopyCtorButMovable&&) = default;
    ClassHasNoCopyCtorButMovable& operator=(const ClassHasNoCopyCtorButMovable&) = delete;
    ClassHasNoCopyCtorButMovable& operator=(ClassHasNoCopyCtorButMovable&&) = default;
};

} // namespace testing
} // namespace ext

#endif // EXT_UTILITY_CLASSES_HPP
