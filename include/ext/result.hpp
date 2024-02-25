/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <exception>

#include <ext/detail/result/result_move_constructor.hpp>
#include <ext/value_wrapper.hpp>

/// @defgroup ext-utility-result
/// @ingroup  ext-utility

namespace ext
{

template<typename E>
class bad_result_access : public std::exception
{};

/// An error wrapper for result.
/// @tparam  E Error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename E>
class failure : public value_wrapper<E, failure_tag_t>
{
    static_assert(!std::is_reference_v<E>, "E must not be a reference type");
    static_assert(!std::is_array_v<E>, "E must not be an array type");
    static_assert(!std::is_void_v<E>, "E must not be the void type");

public:
    using value_wrapper<E, failure_tag_t>::value_wrapper;
};

/// CTAD for the ext::failure type.
/// @tparam  E Error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename E>
failure(E&&) -> failure<std::remove_reference_t<E>>;

/// A type traits for checking whether the specified type is a specialization of the ext::failure class template.
/// @tparam  T Type to check.
/// @tparam  E Expected error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename T, typename E = ignore>
struct is_failure
    : detail::is_failure<std::remove_cv_t<T>, E>
{};

/// An auxiliary variable template for the ext::is_failure type trait.
/// @tparam  T Type to check.
/// @tparam  E Expected error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename T, typename E = ignore>
constexpr inline auto is_failure_v = is_failure<T, E>::value;

template<typename T, typename E>
class result
    : private detail::result_move_constructor<T, E>
{
    static_assert(!std::is_void_v<E> && !std::is_reference_v<E> && !std::is_array_v<E>,
        "E must not be a reference, array or void type");

private:
    using base = detail::result_move_constructor<T, E>;

public:
    using value_type = T; ///< Value type.
    using error_type = E; ///< Error type.

    /// Default constructor.
    result() = default;

    /// Copy constructor.
    result(const result&) = default;

    /// Move constructor.
    result(result&&) = default;

    /// Constructs an object that contains a value, initialized with the specified value.
    /// @tparam U     Value type.
    /// @param  value Value with which to initialize the contained value.
    template<typename U>
        requires std::is_constructible_v<T, U&&>
              && (!is_failure_v<std::remove_reference_t<U>>)
              && (!std::is_void_v<T>)
              && (!std::is_reference_v<T>)
              && (!std::is_same_v<std::remove_cvref_t<U>, result<T, E>>)
              && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
              && (!std::is_same_v<std::remove_cvref_t<U>, failure_tag_t>)
    constexpr explicit(!std::is_convertible_v<U&&, T>) result(U&& value)
        noexcept(std::is_nothrow_constructible_v<T, U&&>);

    /// Constructs an object that contains a value, initialized with the specified value.
    /// @tparam U     Value type.
    /// @param  value Value with which to initialize the contained value.
    template<typename U>
        requires std::is_convertible_v<U&, std::add_lvalue_reference_t<T>>
              && std::is_reference_v<T>
              && (!is_failure_v<std::remove_reference_t<U>>)
              && (!std::is_void_v<T&>)
              && (!std::is_same_v<std::remove_cvref_t<U>, result<T, E>>)
              && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
              && (!std::is_same_v<std::remove_cvref_t<U>, failure_tag_t>)
    constexpr explicit (!std::is_convertible_v<U&, std::add_lvalue_reference_t<T>>) result(U& value) noexcept;

    /// Constructs an object that contains an error, initialized with the specified arguments.
    /// @tparam Args Values type.
    /// @param  args Values with which to initialize the contained error.
    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result(failure_tag_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args&&...>);

    /// Destructor.
    ~result() = default;

    /// Gets the status of the result.
    /// @return Result status.
    constexpr auto status() const noexcept -> result_status;

    /// Gets the value contained in the result.
    /// @return A reference to the contained value.
    constexpr auto value() & -> std::add_lvalue_reference_t<std::remove_reference_t<T>>;

    /// Gets the value contained in the result.
    /// @return A reference to the contained value.
    constexpr auto value() const & -> std::add_lvalue_reference_t<const std::remove_reference_t<T>>;

    /// Gets the value contained in the result.
    /// @return A reference to the contained value.
    constexpr auto value() && -> std::add_rvalue_reference_t<std::remove_reference_t<T>>;

    /// Gets the value contained in the result.
    /// @return A reference to the contained value.
    constexpr auto value() const && -> std::add_rvalue_reference_t<const std::remove_reference_t<T>>;

    /// Gets the error contained in the result.
    /// @return A reference to the contained error.
    constexpr auto error() & -> E&;

    /// Gets the error contained in the result.
    /// @return A reference to the contained error.
    constexpr auto error() const & -> const E&;

    /// Gets the error contained in the result.
    /// @return A reference to the contained error.
    constexpr auto error() && -> E&&;

    /// Gets the error contained in the result.
    /// @return A reference to the contained error.
    constexpr auto error() const && -> const E&&;

    /// Checks whether the result is success.
    /// @return If result contains a value - true, otherwise - false.
    constexpr explicit operator bool() const noexcept;
};

template<typename T, typename E>
template<typename U>
    requires std::is_constructible_v<T, U&&>
          && (!is_failure_v<std::remove_reference_t<U>>)
          && (!std::is_void_v<T>)
          && (!std::is_reference_v<T>)
          && (!std::is_same_v<std::remove_cvref_t<U>, result<T, E>>)
          && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
          && (!std::is_same_v<std::remove_cvref_t<U>, failure_tag_t>)
constexpr result<T, E>::result(U&& value)
    noexcept(std::is_nothrow_constructible_v<T, U&&>)
    : base(std::forward<U>(value))
{}

template<typename T, typename E>
template<typename U>
    requires std::is_convertible_v<U&, std::add_lvalue_reference_t<T>>
          && std::is_reference_v<T>
          && (!is_failure_v<std::remove_reference_t<U>>)
          && (!std::is_void_v<T&>)
          && (!std::is_same_v<std::remove_cvref_t<U>, result<T, E>>)
          && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
          && (!std::is_same_v<std::remove_cvref_t<U>, failure_tag_t>)
constexpr result<T, E>::result(U& value) noexcept
    : base(value)
{}

template<typename T, typename E>
template<typename... Args>
    requires std::is_constructible_v<E, Args&&...>
constexpr result<T, E>::result(failure_tag_t tag, Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args&&...>)
    : base(tag, std::forward<Args>(args)...)
{}

template<typename T, typename E>
constexpr auto result<T, E>::status() const noexcept -> result_status
{
    return this->_status;
}

template<typename T, typename E>
constexpr auto result<T, E>::value() & -> std::add_lvalue_reference_t<std::remove_reference_t<T>>
{
    if (this->_status != result_status::success)
    {
        throw bad_result_access<E>();
    }

    if constexpr (std::is_void_v<T>)
    {
        return;
    }
    else if constexpr (std::is_reference_v<T>)
    {
        return *this->_value;
    }
    else
    {
        return this->_value;
    }
}

template<typename T, typename E>
constexpr auto result<T, E>::value() const & -> std::add_lvalue_reference_t<const std::remove_reference_t<T>>
{
    if (this->_status != result_status::success)
    {
        throw bad_result_access<E>();
    }

    if constexpr (std::is_void_v<T>)
    {
        return;
    }
    else if constexpr (std::is_reference_v<T>)
    {
        return *this->_value;
    }
    else
    {
        return this->_value;
    }
}

template<typename T, typename E>
constexpr auto result<T, E>::value() && -> std::add_rvalue_reference_t<std::remove_reference_t<T>>
{
    if (this->_status != result_status::success)
    {
        throw bad_result_access<E>();
    }

    if constexpr (std::is_void_v<T>)
    {
        return;
    }
    else if constexpr (std::is_reference_v<T>)
    {
        return std::move(*this->_value);
    }
    else
    {
        return std::move(this->_value);
    }
}

template<typename T, typename E>
constexpr auto result<T, E>::value() const && -> std::add_rvalue_reference_t<const std::remove_reference_t<T>>
{
    if (this->_status != result_status::success)
    {
        throw bad_result_access<E>();
    }

    if constexpr (std::is_void_v<T>)
    {
        return;
    }
    else if constexpr (std::is_reference_v<T>)
    {
        return std::move(*this->_value);
    }
    else
    {
        return std::move(this->_value);
    }
}

template<typename T, typename E>
constexpr auto result<T, E>::error() & -> E&
{
    if (*this)
    {
        throw bad_result_access<E>();
    }
    return this->_error;
}

template<typename T, typename E>
constexpr auto result<T, E>::error() const & -> const E&
{
    if (*this)
    {
        throw bad_result_access<E>();
    }
    return this->_error;
}

template<typename T, typename E>
constexpr auto result<T, E>::error() && -> E&&
{
    if (*this)
    {
        throw bad_result_access<E>();
    }
    return std::move(this->_error);
}

template<typename T, typename E>
constexpr auto result<T, E>::error() const && -> const E&&
{
    if (*this)
    {
        throw bad_result_access<E>();
    }
    return std::move(this->_error);
}

template<typename T, typename E>
constexpr result<T, E>::operator bool() const noexcept
{
    return this->_status == result_status::success;
}

} // namespace ext
