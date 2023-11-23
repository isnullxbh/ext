/**
 * @file    none.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.11.2023
 */

#pragma once

namespace ext
{

/// An auxiliary type used as marker.
/// @ingroup ext-utility
/// @since   0.1.0
struct none_t
{
    struct secure {};
    constexpr explicit none_t(secure) noexcept {}
};

/// An instance of the none_t type.
/// @ingroup ext-utility
/// @since   0.1.0
constexpr inline none_t none { none_t::secure() };

} // namespace ext
