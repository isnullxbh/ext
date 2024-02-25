/**
 * @file    result_move_constructor.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    23.02.2024
 */

#pragma once

#include <ext/detail/result/result_copy_constructor.hpp>

namespace ext
{

namespace detail
{

template<typename T, typename E, typename = void>
struct result_trivially_move_constructible
     : std::bool_constant<std::is_trivially_move_constructible_v<T> &&
                          std::is_trivially_move_constructible_v<E>>
{};

template<typename T, typename E>
struct result_trivially_move_constructible<T&, E>
     : std::is_trivially_move_constructible<E>
{};

template<typename T, typename E>
struct result_trivially_move_constructible<T, E, std::enable_if_t<std::is_void_v<T>>>
    : std::is_trivially_move_constructible<E>
{};

template<typename T, typename E, auto = result_trivially_move_constructible<T, E>::value>
class result_move_constructor
    : public result_copy_constructor<T, E>
{
public:
    using result_copy_constructor<T, E>::result_copy_constructor;

    /// Move constructor.
    result_move_constructor() = default;
};

template<typename T, typename E, typename = void>
class result_move_constructor_base
    : public result_copy_constructor<T, E>
{
public:
    using result_copy_constructor<T, E>::result_copy_constructor;

    /// Default constructor.
    result_move_constructor_base() = default;

    /// Copy constructor.
    result_move_constructor_base(const result_move_constructor_base&) = default;

    /// Move constructor.
    constexpr result_move_constructor_base(result_move_constructor_base&& rhs)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
        requires std::is_move_constructible_v<T>
              && std::is_move_constructible_v<E>
        : result_copy_constructor<T, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value(std::move(rhs._value))
            : this->construct_error(std::move(rhs._error));
    }
};

template<typename T, typename E>
class result_move_constructor_base<T&, E>
    : public result_copy_constructor<T&, E>
{
public:
    using result_copy_constructor<T&, E>::result_copy_constructor;

    /// Move constructor.
    constexpr result_move_constructor_base(result_move_constructor_base&& rhs)
        noexcept(std::is_nothrow_move_constructible_v<E>)
        requires std::is_move_constructible_v<E>
        : result_copy_constructor<T&, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value(*rhs._value)
            : this->construct_error(std::move(rhs._error));
    }
};

template<typename T, typename E>
class result_move_constructor_base<T, E, std::enable_if_t<std::is_void_v<T>>>
    : public result_copy_constructor<T, E>
{
public:
    /// Default constructor.
    result_move_constructor_base() = default;

    /// Copy constructor.
    result_move_constructor_base(const result_move_constructor_base&) = default;

    /// Move constructor.
    constexpr result_move_constructor_base(result_move_constructor_base&& rhs)
        noexcept(std::is_nothrow_move_constructible_v<E>)
        requires std::is_move_constructible_v<E>
        : result_copy_constructor<T, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value()
            : this->construct_error(std::move(rhs._error));
    }
};

template<typename T, typename E>
class result_move_constructor<T, E, false>
    : public result_move_constructor_base<T, E>
{
public:
    using result_move_constructor_base<T, E>::result_move_constructor_base;
};

} // namespace detail
} // namespace ext
