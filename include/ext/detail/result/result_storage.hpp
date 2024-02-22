/**
 * @file    result_storage.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <type_traits>

namespace ext
{

/// Result status.
/// @ingroup ext-utility-result
/// @since   0.1.0
enum class result_status : char8_t
{
    failure, ///< Indicates that the result contains an error.
    success  ///< Indicates that the result contains a value.
};

namespace detail
{

template<typename T, typename E, typename = void>
struct result_trivially_destructible
    : std::bool_constant<std::is_trivially_destructible_v<T> &&
                         std::is_trivially_destructible_v<E>>
{};

template<typename T, typename E>
struct result_trivially_destructible<T&, E>
    : std::bool_constant<std::is_trivially_destructible_v<E>>
{};

template<typename T, typename E>
struct result_trivially_destructible<T, E, std::enable_if_t<std::is_void_v<T>>>
    : std::bool_constant<std::is_trivially_destructible_v<E>>
{};

template<typename T, typename E, typename = void, auto = result_trivially_destructible<T, E>::value>
class result_storage_base;

template<typename T, typename E>
class result_storage_base<T, E, void*, true>
{
public:
    /// Default constructor.
    constexpr result_storage_base() noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>
        : _value()
        , _status(result_status::success)
    {}

    /// Destructor.
    ~result_storage_base() = default;

    union
    {
        T _value;
        E _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage_base<T, E, void*, false>
{
public:
    /// Default constructor.
    constexpr result_storage_base() noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>
        : _value()
        , _status(result_status::success)
    {}

    /// Destructor.
    constexpr ~result_storage_base() noexcept(std::is_nothrow_destructible_v<T> &&
                                              std::is_nothrow_destructible_v<E>)
    {
        if (_status == result_status::success)
        {
            _value.~T();
        }
        else
        {
            _error.~E();
        }
    }

    union
    {
        T _value;
        E _error;
    };
    result_status _status;
};

template<typename T, typename E, typename = void>
class result_storage
    : public result_storage_base<T, E, void*>
{};

template<typename T, typename E>
class result_storage_base<T&, E, void, true>
{
public:
    /// Default constructor.
    result_storage_base() = delete;

    /// Destructor.
    ~result_storage_base() = default;

    union
    {
        T* _value;
        E  _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage_base<T&, E, void, false>
{
public:
    /// Default constructor.
    result_storage_base() = delete;

    /// Destructor.
    constexpr ~result_storage_base() noexcept(std::is_nothrow_destructible_v<E>)
    {
        if (_status == result_status::failure)
        {
            _error.~E();
        }
    }

    union
    {
        T* _value;
        E  _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage<T&, E>
    : public result_storage_base<T&, E>
{};

template<typename T, typename E>
class result_storage_base<T, E, std::enable_if_t<std::is_void_v<T>>, true>
{
public:
    /// Default constructor.
    constexpr result_storage_base() noexcept
        : _status(result_status::success)
    {}

    /// Destructor.
    ~result_storage_base() = default;

    union
    {
        E _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage_base<T, E, std::enable_if_t<std::is_void_v<T>>, false>
{
public:
    /// Default constructor.
    constexpr result_storage_base() noexcept
        : _status(result_status::success)
    {}

    /// Destructor.
    constexpr ~result_storage_base() noexcept(std::is_nothrow_destructible_v<E>)
    {
        if (_status == result_status::failure)
        {
            _error.~E();
        }
    }

    union
    {
        E _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage<T, E, std::enable_if_t<std::is_void_v<T>>>
    : public result_storage_base<T, E>
{};

} // namespace detail
} // namespace ext
