/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <ext/detail/result/result_copy_constructor.hpp>

namespace ext
{

template<typename T, typename E>
class result
    : private detail::result_copy_constructor<T, E>
{
public:
    /// Default constructor.
    result() = default;

    /// Copy constructor.
    result(const result&) = default;

    /// Destructor.
    ~result() = default;
};

} // namespace ext
