/**
 * @file    result_storage.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <type_traits>
#include <utility>

#include <ext/ignore.hpp>

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

using failure_tag_t = std::integral_constant<result_status, result_status::failure>;
constexpr inline failure_tag_t failure_tag {};

template<typename>
class failure;

template<typename, typename>
class result;

namespace detail
{

namespace result_helpers
{

struct skip_init {};

} // namespace result_helpers

template<typename, typename>
struct is_failure : std::false_type
{};

template<typename Ecur, typename Eexp>
struct is_failure<failure<Ecur>, Eexp>
    : std::disjunction<std::is_same<Eexp, ignore>,
                       std::is_same<Ecur, Eexp>>
{};

template<typename T, typename U = ignore, typename V = ignore>
struct is_result : std::false_type
{};

template<typename T, typename E, typename U, typename V>
struct is_result<result<T, E>, U, V>
     : std::conjunction<
          std::disjunction<std::is_same<U, ignore>, std::is_same<T, U>>,
          std::disjunction<std::is_same<V, ignore>, std::is_same<E, V>>>
{};

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

    template<typename U>
        requires std::is_constructible_v<T, U&&>
    constexpr explicit result_storage_base(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _value(std::forward<U>(value))
        , _status(result_status::success)
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
    {}

    template<typename... Args>
        requires std::is_constructible_v<T, Args&&...>
    constexpr explicit result_storage_base(std::in_place_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : _value(std::forward<Args>(args)...)
        , _status(result_status::success)
    {}

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
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

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
    {}

    template<typename U>
        requires std::is_constructible_v<T, U&&>
    constexpr explicit result_storage_base(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : _value(std::forward<U>(value))
        , _status(result_status::success)
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
    {}

    template<typename... Args>
        requires std::is_constructible_v<T, Args&&...>
    constexpr explicit result_storage_base(std::in_place_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : _value(std::forward<Args>(args)...)
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
{
public:
    using result_storage_base<T, E, void*>::result_storage_base;

    /// Constructs value from the specified value.
    /// @tparam U     Value type.
    /// @param  value Value with which to initialize the contained value.
    template<typename U>
        requires std::is_constructible_v<T, U&&>
    constexpr auto construct_value(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>) -> void
    {
        if constexpr (!std::is_trivially_constructible_v<T, U&&>)
        {
            std::construct_at(std::addressof(this->_value), std::forward<U>(value));
        }
        else
        {
            this->_value = std::forward<U>(value);
        }
        this->_status = result_status::success;
    }

    /// Constructs error from the specified value.
    /// @tparam U     Value type.
    /// @param  error Value with which to initialize the contained error.
    template<typename U>
        requires std::is_constructible_v<E, U&&>
    constexpr auto construct_error(U&& error) noexcept(std::is_nothrow_constructible_v<E, U&&>) -> void
    {
        if constexpr (!std::is_trivially_constructible_v<E, U&&>)
        {
            std::construct_at(std::addressof(this->_error), std::forward<U>(error));
        }
        else
        {
            this->_error = std::forward<U>(error);
        }
        this->_status = result_status::failure;
    }
};

template<typename T, typename E>
class result_storage_base<T&, E, void, true>
{
public:
    /// Default constructor.
    result_storage_base() = delete;

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
    {}

    template<typename U>
        requires std::is_convertible_v<U&, T&>
    constexpr explicit result_storage_base(U& value) noexcept
        : _value(std::addressof(value))
        , _status(result_status::success)
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
    {}

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

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
    {}

    template<typename U>
        requires std::is_convertible_v<U&, T&>
    constexpr explicit result_storage_base(U& value) noexcept
        : _value(std::addressof(value))
        , _status(result_status::success)
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
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
        T* _value;
        E  _error;
    };
    result_status _status;
};

template<typename T, typename E>
class result_storage<T&, E>
    : public result_storage_base<T&, E>
{
public:
    using result_storage_base<T&, E>::result_storage_base;

    /// Constructs value from the specified value.
    /// @tparam U     Value type.
    /// @param  value Value with which to initialize the contained value.
    template<typename U>
        requires std::is_convertible_v<U&, T&>
    constexpr auto construct_value(U& value) noexcept -> void
    {
        this->_value = std::addressof(value);
        this->_status = result_status::success;
    }

    /// Constructs error from the specified value.
    /// @tparam U     Value type.
    /// @param  error Value with which to initialize the contained error.
    template<typename U>
        requires std::is_constructible_v<E, U&&>
    constexpr auto construct_error(U&& error) noexcept(std::is_nothrow_constructible_v<E, U&&>) -> void
    {
        if constexpr (!std::is_trivially_constructible_v<E, U&&>)
        {
            std::construct_at(std::addressof(this->_error), std::forward<U>(error));
        }
        else
        {
            this->_error = std::forward<U>(error);
        }
        this->_status = result_status::failure;
    }
};

template<typename T, typename E>
class result_storage_base<T, E, std::enable_if_t<std::is_void_v<T>>, true>
{
public:
    /// Default constructor.
    constexpr result_storage_base() noexcept
        : _status(result_status::success)
    {}

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
    {}

    constexpr explicit result_storage_base(std::in_place_t) noexcept
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

    /// Constructs an object without initialization of value or error.
    constexpr explicit result_storage_base(result_helpers::skip_init) noexcept
        : _status()
    {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result_storage_base(failure_tag_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
        : _error(std::forward<Args>(args)...)
        , _status(result_status::failure)
    {}

    constexpr explicit result_storage_base(std::in_place_t) noexcept
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
{
public:
    using result_storage_base<T, E>::result_storage_base;

    /// Constructs value from the specified value.
    constexpr auto construct_value() noexcept -> void
    {
        this->_status = result_status::success;
    }

    /// Constructs error from the specified value.
    /// @tparam U     Value type.
    /// @param  error Value with which to initialize the contained error.
    template<typename U>
        requires std::is_constructible_v<E, U&&>
    constexpr auto construct_error(U&& error) noexcept(std::is_nothrow_constructible_v<E, U&&>) -> void
    {
        if constexpr (!std::is_trivially_constructible_v<E, U&&>)
        {
            std::construct_at(std::addressof(this->_error), std::forward<U>(error));
        }
        else
        {
            this->_error = std::forward<U>(error);
        }
        this->_status = result_status::failure;
    }
};

} // namespace detail
} // namespace ext
