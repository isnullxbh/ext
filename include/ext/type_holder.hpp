/**
 * @file    type_holder.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    03.01.2024
 */

#pragma once

#include <ext/detail/indexed.hpp>

namespace ext
{

/// A type holder.
/// @tparam  T      The type to hold.
/// @tparam  Mixins Base classes.
/// @ingroup ext-metaprogramming
/// @since   0.2.0
template<typename T, typename... Mixins>
struct type_holder : Mixins...
{
    using type = T;
};

/// A helper type alias template for type holders that contains some index.
/// @ingroup ext-metaprogramming
/// @since   0.2.0
template<typename T, auto Index>
using indexed_type_holder = type_holder<T, detail::indexed<Index>>;

} // namespace ext
