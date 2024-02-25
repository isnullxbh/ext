/**
 * @file    result_copy_constructor.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    23.02.2024
 */

#pragma once

#include <ext/detail/result/result_storage.hpp>

namespace ext
{

namespace detail
{

template<typename T, typename E, typename = void>
struct result_trivially_copy_constructible
     : std::bool_constant<std::is_trivially_copy_constructible_v<T> &&
                          std::is_trivially_copy_constructible_v<E>>
{};

template<typename T, typename E>
struct result_trivially_copy_constructible<T&, E>
     : std::bool_constant<std::is_trivially_copy_constructible_v<E>>
{};

template<typename T, typename E>
struct result_trivially_copy_constructible<T, E, std::enable_if_t<std::is_void_v<T>>>
     : std::bool_constant<std::is_trivially_copy_constructible_v<E>>
{};

template<typename T, typename E, auto = result_trivially_copy_constructible<T, E>::value>
class result_copy_constructor
    : public result_storage<T, E>
{
public:
    using result_storage<T, E>::result_storage;

    /// Copy constructor.
    result_copy_constructor(const result_copy_constructor&) = default;
};

template<typename T, typename E, typename = void>
class result_copy_constructor_base
    : public result_storage<T, E>
{
public:
    using result_storage<T, E>::result_storage;

    /// Copy constructor.
    constexpr result_copy_constructor_base(const result_copy_constructor_base& rhs)
        : result_storage<T, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value(rhs._value)
            : this->construct_error(rhs._error);
    }
};

template<typename T, typename E>
class result_copy_constructor_base<T&, E>
    : public result_storage<T&, E>
{
public:
    using result_storage<T&, E>::result_storage;

    /// Copy constructor.
    constexpr result_copy_constructor_base(const result_copy_constructor_base& rhs)
        : result_storage<T&, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value(rhs._value)
            : this->construct_error(rhs._error);
    }
};

template<typename T, typename E>
class result_copy_constructor_base<T, E, std::enable_if_t<std::is_void_v<T>>>
    : public result_storage<T, E>
{
public:
    using result_storage<T, E>::result_storage;

    /// Copy constructor.
    constexpr result_copy_constructor_base(const result_copy_constructor_base& rhs)
        : result_storage<T, E>(result_helpers::skip_init())
    {
        rhs._status == result_status::success
            ? this->construct_value()
            : this->construct_error(rhs._error);
    }
};

template<typename T, typename E>
class result_copy_constructor<T, E, false>
    : public result_copy_constructor_base<T, E>
{
public:
    using result_copy_constructor_base<T, E>::result_copy_constructor_base;
};

} // namespace detail
} // namespace ext
