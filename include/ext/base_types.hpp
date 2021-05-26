/**
 * @file    base_types.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    26.05.2021
 */

#ifndef EXT_BASE_TYPES_HPP
#define EXT_BASE_TYPES_HPP

namespace ext {

/**
 * @class   None
 * @brief   A utility class used to indicate that some value is not present.
 * @since   0.1.3
 */

struct None final {};

constexpr inline None none {};

} // namespace ext

#endif // EXT_BASE_TYPES_HPP
