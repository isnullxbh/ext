/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <ext/detail/result/result_move_constructor.hpp>

namespace ext
{

template<typename T, typename E>
class result
    : private detail::result_move_constructor<T, E>
{
public:
    /// Default constructor.
    result() = default;

    /// Copy constructor.
    result(const result&) = default;

    /// Move constructor.
    result(result&&) = default;

    /// Destructor.
    ~result() = default;

    constexpr auto status() const noexcept -> result_status;
};

template<typename T, typename E>
constexpr auto result<T, E>::status() const noexcept -> result_status
{
    return this->_status;
}

} // namespace ext
