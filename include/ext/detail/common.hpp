/**
 * @file    common.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    26.02.2024
 */

#pragma once

#include <type_traits>

namespace ext::detail
{

template<typename T, typename U>
struct converts_from_any_cvref
    : std::disjunction<
          std::is_constructible<T, U&>,        std::is_convertible<U&, T>,
          std::is_constructible<T, const U&>,  std::is_convertible<const U&, T>,
          std::is_constructible<T, U&&>,       std::is_convertible<U&&, T>,
          std::is_constructible<T, const U&&>, std::is_convertible<const U&&, T>>
{};

template<typename T, typename U>
constexpr inline auto converts_from_any_cvref_v = converts_from_any_cvref<T, U>::value;

} // namespace ext::detail
