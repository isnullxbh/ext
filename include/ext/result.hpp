/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <exception>
#include <functional>

#include <ext/detail/common.hpp>
#include <ext/detail/doxygen.hpp>
#include <ext/detail/result/result_move_constructor.hpp>
#include <ext/value_wrapper.hpp>

/// @defgroup ext-utility-result ext::result
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

/// A type traits for checking whether the specified type is a specialization of the ext::result class template.
/// @tparam  R Type to check.
/// @tparam  T Expected value type.
/// @tparam  E Expected error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename R, typename T = ignore, typename E = ignore>
struct is_result
    : detail::is_result<std::remove_cv_t<R>, T, E>
{};

/// An auxiliary variable template for the ext::is_result type trait.
/// @tparam  R Type to check.
/// @tparam  T Expected value type.
/// @tparam  E Expected error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename R, typename T = ignore, typename E = ignore>
constexpr inline auto is_result_v = is_result<R, T, E>::value;

/// Represents either success (value) or failure (error).
/// @tparam  T Value type.
/// @tparam  E Error type.
/// @ingroup ext-utility-result
/// @since   0.1.0
template<typename T, typename E>
class result
    : private detail::result_move_constructor<T, E>
{
    static_assert(!std::is_rvalue_reference_v<T> && !std::is_array_v<T>,
        "T must not be a rvalue-reference or array type");
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
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<U&&, T>)) result(U&& value)
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
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<U&, std::add_lvalue_reference_t<T>>))
        result(U& value) noexcept;

    /// Constructs an object that contains an error, initialized with the specified arguments.
    /// @tparam Args Values type.
    /// @param  args Values with which to initialize the contained error.
    template<typename... Args>
        requires std::is_constructible_v<E, Args&&...>
    constexpr explicit result(failure_tag_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args&&...>);

    /// Constructs an object that contains a value, initialized from the specified arguments.
    /// @tparam Args Argument types.
    /// @param  tag  Indicates that value will be constructed in place.
    /// @param  args Values with which to initialize the contained value.
    template<typename... Args>
        requires (!std::is_void_v<T>)
              && std::is_constructible_v<T, Args&&...>
    constexpr explicit result(std::in_place_t tag, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args&&...>);

    /// Constructs an object with status "success".
    /// @param tag Indicates that value will be constructed in place.
    constexpr explicit result(std::in_place_t tag) noexcept
        requires std::is_void_v<T>;

    /// Converting copy constructor.
    /// @tparam U   Value type of the original result.
    /// @tparam V   Error type of the original result.
    /// @param  rhs Original result.
    template<typename U, typename V>
        requires (!std::is_reference_v<T>)
              && ((std::is_void_v<T> && std::is_void_v<U>) ||
                   std::is_constructible_v<T, std::add_lvalue_reference_t<const std::remove_reference_t<U>>>)
              && std::is_constructible_v<E, const V&>
              && (!detail::converts_from_any_cvref_v<T, result<U, V>>)
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<std::add_lvalue_reference_t<const U>, T> ||
                                          !std::is_convertible_v<const V&, E>))
        result(const result<U, V>& rhs);

    /// Converting move constructor.
    /// @tparam U   Value type of the original result.
    /// @tparam V   Error type of the original result.
    /// @param  rhs Original result.
    template<typename U, typename V>
        requires (!std::is_reference_v<T>)
              && ((std::is_void_v<T> && std::is_void_v<U>) ||
                   std::is_constructible_v<T, std::add_rvalue_reference_t<std::remove_reference_t<U>>>)
              && std::is_constructible_v<E, V&&>
              && (!detail::converts_from_any_cvref_v<T, result<U, V>>)
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<std::add_rvalue_reference_t<U>, T> ||
                                          !std::is_convertible_v<V&&, E>))
        result(result<U, V>&& rhs);

    /// Converting copy constructor.
    /// This constructor does not participate in overload resolution unless T and U are reference types.
    /// @tparam U   Value type of the original result.
    /// @tparam V   Error type of the original result.
    /// @param  rhs Original result.
    template<typename U, typename V>
        requires std::is_reference_v<T>
              && std::is_reference_v<U>
              && std::is_convertible_v<U, T>
              && std::is_constructible_v<E, const V&>
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<U, T> || !std::is_convertible_v<const V&, E>))
        result(const result<U, V>& rhs);

    /// Converting move constructor.
    /// This constructor does not participate in overload resolution unless T and U are reference types.
    /// @tparam U   Value type of the original result.
    /// @tparam V   Error type of the original result.
    /// @param  rhs Original result.
    template<typename U, typename V>
        requires std::is_reference_v<T>
              && std::is_reference_v<U>
              && std::is_convertible_v<U, T>
              && std::is_constructible_v<E, V&&>
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<U, T> || !std::is_convertible_v<V&&, E>))
        result(result<U, V>&& rhs);

    /// Constructs an object that contains an error, initialized by failure value.
    /// @tparam U   Failure value type.
    /// @param  rhs Failure object.
    template<typename U>
        requires std::is_constructible_v<E, const U&>
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<const U&, E>) result(const failure<U>& rhs))
        noexcept(std::is_nothrow_constructible_v<E, const U&>);

    /// Constructs an object that contains an error, initialized by failure value.
    /// @tparam U   Failure value type.
    /// @param  rhs Failure object.
    template<typename U>
        requires std::is_constructible_v<E, U&&>
    constexpr EXT_DOXYGEN_IGNORE(explicit(!std::is_convertible_v<U&&, E>) result(failure<U>&& rhs))
        noexcept(std::is_nothrow_constructible_v<E, U&&>);

    /// Destructor.
    ~result() = default;

    /// Gets the status of the result.
    /// @return Result status.
    constexpr auto status() const noexcept -> result_status;

    /// Checks whether the result contains a value.
    /// @return If the result contains a value - true, otherwise - false.
    constexpr auto is_success() const noexcept -> bool;

    /// Checks whether the result contains an error.
    /// @return If the result contains an error - true, otherwise - false.
    constexpr auto is_failure() const noexcept -> bool;

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

    /// Maps result to a result of another type by applying the specified function (or functional object) to the
    /// contained value.
    /// Signature: result<T, E> -> (T -> T') -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
              && std::is_copy_constructible_v<E>
    constexpr auto map(F&& f) const &
        -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    /// Maps result to a result of another type by applying the specified function (or functional object) to the
    /// contained value.
    /// Signature: result<T, E> -> (T -> T') -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_copy_constructible_v<E>
    constexpr auto map(F&& f) const & -> result<std::invoke_result_t<F>, E>;

    /// Maps result to a result of another type by applying the specified function (or functional object) to the
    /// contained value.
    /// Signature: result<T, E> -> (T -> T') -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::remove_reference_t<T>&&>
              && std::is_move_constructible_v<E>
    constexpr auto map(F&& f) && -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    /// Maps result to a result of another type by applying the specified function (or functional object) to the
    /// contained value.
    /// Signature: result<T, E> -> (T -> T') -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_move_constructible_v<E>
    constexpr auto map(F&& f) && -> result<std::invoke_result_t<F>, E>;

    /// Binds the specified function (or functional object) across the result's value.
    /// Signature: result<T, E> -> (T -> result<T', E>) -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
              && is_result_v<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, ignore, E>
              && std::is_copy_constructible_v<E>
    constexpr auto bind(F&& f) const &
        -> std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>;

    /// Binds the specified function (or functional object) across the result's value.
    /// Signature: result<T, E> -> (T -> result<T', E>) -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && is_result_v<std::invoke_result_t<F>, ignore, E>
              && std::is_copy_constructible_v<E>
    constexpr auto bind(F&& f) const & -> std::invoke_result_t<F>;

    /// Binds the specified function (or functional object) across the result's value.
    /// Signature: result<T, E> -> (T -> result<T', E>) -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
              && is_result_v<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, ignore, E>
              && std::is_move_constructible_v<E>
    constexpr auto bind(F&& f) &&
        -> std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>;

    /// Binds the specified function (or functional object) across the result's value.
    /// Signature: result<T, E> -> (T -> result<T', E>) -> result<T', E>
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && is_result_v<std::invoke_result_t<F>, ignore, E>
              && std::is_move_constructible_v<E>
    constexpr auto bind(F&& f) && -> std::invoke_result_t<F>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: T -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
              && std::is_copy_constructible_v<E>
    constexpr auto apply(result<F, E>& rhs) const &
        -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: T -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
              && std::is_copy_constructible_v<E>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>& rhs) &&
        -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: T -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
              && std::is_copy_constructible_v<E>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>&& rhs) const &
        -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: T -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires (!std::is_void_v<T>)
              && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>&& rhs) &&
        -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: void -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_copy_constructible_v<E>
    constexpr auto apply(result<F, E>& rhs) const & -> result<std::invoke_result_t<F>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: void -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_copy_constructible_v<E>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>& rhs) && -> result<std::invoke_result_t<F>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: void -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_copy_constructible_v<E>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>&& rhs) const & -> result<std::invoke_result_t<F>, E>;

    /// Applies the function (or functional object) contained in the specified result to the result's value.
    /// Signature: result<T, E> -> result<F, E> -> result<T', E>, F: void -> T'
    /// @tparam F Function type.
    /// @param  f Function.
    template<typename F>
        requires std::is_void_v<T>
              && std::is_invocable_v<F>
              && std::is_move_constructible_v<E>
    constexpr auto apply(result<F, E>&& rhs) && -> result<std::invoke_result_t<F>, E>;

    template<typename U, typename V>
    friend class result;
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
template<typename... Args>
    requires (!std::is_void_v<T>)
         && std::is_constructible_v<T, Args&&...>
constexpr result<T, E>::result(std::in_place_t tag, Args&&... args)
    noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
    : base(tag, std::forward<Args>(args)...)
{}

template<typename T, typename E>
constexpr result<T, E>::result(std::in_place_t tag) noexcept requires std::is_void_v<T>
    : base(tag)
{}

template<typename T, typename E>
template<typename U, typename V>
    requires (!std::is_reference_v<T>)
          && ((std::is_void_v<T> && std::is_void_v<U>) ||
               std::is_constructible_v<T, std::add_lvalue_reference_t<const std::remove_reference_t<U>>>)
          && std::is_constructible_v<E, const V&>
          && (!detail::converts_from_any_cvref_v<T, result<U, V>>)
constexpr result<T, E>::result(const result<U, V>& rhs)
    : base(detail::result_helpers::skip_init())
{
    if constexpr (!std::is_void_v<T>)
    {
        rhs ? this->construct_value(rhs.value())
            : this->construct_error(rhs.error());
    }
    else
    {
        rhs ? this->construct_value()
            : this->construct_error(rhs.error());
    }
}

template<typename T, typename E>
template<typename U, typename V>
    requires (!std::is_reference_v<T>)
          && ((std::is_void_v<T> && std::is_void_v<U>) ||
               std::is_constructible_v<T, std::add_rvalue_reference_t<std::remove_reference_t<U>>>)
          && std::is_constructible_v<E, V&&>
          && (!detail::converts_from_any_cvref_v<T, result<U, V>>)
constexpr result<T, E>::result(result<U, V>&& rhs)
    : base(detail::result_helpers::skip_init())
{
    if constexpr (!std::is_void_v<T>)
    {
        rhs ? this->construct_value(std::move(rhs).value())
            : this->construct_error(std::move(rhs).error());
    }
    else
    {
        rhs ? this->construct_value()
            : this->construct_error(std::move(rhs).error());
    }
}

template<typename T, typename E>
template<typename U, typename V>
    requires std::is_reference_v<T>
          && std::is_reference_v<U>
          && std::is_convertible_v<U, T>
          && std::is_constructible_v<E, const V&>
constexpr result<T, E>::result(const result<U, V>& rhs)
    : base(detail::result_helpers::skip_init())
{
    rhs ? this->construct_value(*rhs._value) // TODO
        : this->construct_error(rhs.error());
}

template<typename T, typename E>
template<typename U, typename V>
    requires std::is_reference_v<T>
          && std::is_reference_v<U>
          && std::is_convertible_v<U, T>
          && std::is_constructible_v<E, V&&>
constexpr result<T, E>::result(result<U, V>&& rhs)
    : base(detail::result_helpers::skip_init())
{
    rhs ? this->construct_value(*rhs._value) // TODO
        : this->construct_error(std::move(rhs).error());
}

template<typename T, typename E>
template<typename U>
    requires std::is_constructible_v<E, const U&>
constexpr result<T, E>::result(const failure<U>& rhs) noexcept(std::is_nothrow_constructible_v<E, const U&>)
    : base(failure_tag, rhs.value())
{}

template<typename T, typename E>
template<typename U>
    requires std::is_constructible_v<E, U&&>
constexpr result<T, E>::result(failure<U>&& rhs) noexcept(std::is_nothrow_constructible_v<E, U&&>)
    : base(failure_tag, std::move(rhs).value())
{}

template<typename T, typename E>
constexpr auto result<T, E>::status() const noexcept -> result_status
{
    return this->_status;
}

template<typename T, typename E>
constexpr auto result<T, E>::is_success() const noexcept -> bool
{
    return this->_status == result_status::success;
}

template<typename T, typename E>
constexpr auto result<T, E>::is_failure() const noexcept -> bool
{
    return this->_status == result_status::failure;
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

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::map(F&& f) const &
    -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    if constexpr (!std::is_void_v<typename result_t::value_type>)
    {
        return *this ? result_t { std::invoke(std::forward<F>(f), value()) }
                     : result_t { failure_tag, error() };
    }
    else
    {
        return *this ? (std::invoke(std::forward<F>(f), value()), result_t {})
                     : result_t { failure_tag, error() };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::map(F&& f) const & -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if constexpr (!std::is_void_v<typename result_t::value_type>)
    {
        return *this ? result_t { std::invoke(std::forward<F>(f)) }
                     : result_t { failure_tag, error() };
    }
    else
    {
        return *this ? (std::invoke(std::forward<F>(f)), result_t {})
                     : result_t { failure_tag, error() };
    }
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::remove_reference_t<T>&&>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::map(F&& f) &&
    -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    if constexpr (!std::is_void_v<typename result_t::value_type>)
    {
        return *this ? result_t { std::invoke(std::forward<F>(f), std::move(value())) }
                     : result_t { failure_tag, std::move(error()) };
    }
    else
    {
        return *this ? (std::invoke(std::forward<F>(f), std::move(value())), result_t {})
                     : result_t { failure_tag, std::move(error()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::map(F&& f) && -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if constexpr (!std::is_void_v<typename result_t::value_type>)
    {
        return *this ? result_t { std::invoke(std::forward<F>(f)) }
                     : result_t { failure_tag, std::move(error()) };
    }
    else
    {
        return *this ? (std::invoke(std::forward<F>(f)), result_t {})
                     : result_t { failure_tag, std::move(error()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
          && is_result_v<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, ignore, E>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::bind(F&& f) const &
    -> std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
{
    using result_t = std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>;
    return *this ? std::invoke(std::forward<F>(f), value())
                 : result_t { failure_tag, error() };
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && is_result_v<std::invoke_result_t<F>, ignore, E>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::bind(F&& f) const & -> std::invoke_result_t<F>
{
    using result_t = std::invoke_result_t<F>;
    return *this ? std::invoke(std::forward<F>(f))
                 : result_t { failure_tag, error() };
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
         && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
         && is_result_v<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, ignore, E>
         && std::is_move_constructible_v<E>
constexpr auto result<T, E>::bind(F&& f) &&
    -> std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
{
    using result_t = std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>;
    return *this ? std::invoke(std::forward<F>(f), std::move(value()))
                 : result_t { failure_tag, std::move(error()) };
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && is_result_v<std::invoke_result_t<F>, ignore, E>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::bind(F&& f) && -> std::invoke_result_t<F>
{
    using result_t = std::invoke_result_t<F>;
    return *this ? std::invoke(std::forward<F>(f))
                 : result_t { failure_tag, error() };
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>& rhs) const &
    -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    if (!*this)
    {
        return result_t { failure_tag, error() };
    }

    if (!rhs)
    {
        return result_t { failure_tag, rhs.error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(rhs.value(), value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(rhs.value(), value()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
          && std::is_copy_constructible_v<E>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>& rhs) &&
    -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    if (!*this)
    {
        return result_t { failure_tag, std::move(error()) };
    }

    if (!rhs)
    {
        return result_t { failure_tag, rhs.error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(rhs.value(), std::move(value())), result_t {});
    }
    else
    {
        return result_t { std::invoke(rhs.value(), std::move(value())) };
    }
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
          && std::is_copy_constructible_v<E>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>&& rhs) const &
    -> result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_lvalue_reference_t<const std::remove_reference_t<T>>>, E>;

    if (!*this)
    {
        return result_t { failure_tag, error() };
    }

    if (!rhs)
    {
        return result_t { failure_tag, std::move(rhs).error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(std::move(rhs).value(), value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(std::move(rhs).value(), value()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires (!std::is_void_v<T>)
          && std::is_invocable_v<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>&& rhs) &&
    -> result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>
{
    using result_t = result<std::invoke_result_t<F, std::add_rvalue_reference_t<std::remove_reference_t<T>>>, E>;

    if (!*this)
    {
        return result_t { failure_tag, std::move(error()) };
    }

    if (!rhs)
    {
        return result_t { failure_tag, std::move(rhs).error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(std::move(rhs).value(), std::move(value())), result_t {});
    }
    else
    {
        return result_t { std::invoke(std::move(rhs).value(), std::move(value())) };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_copy_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>& rhs) const & -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if (!*this)
    {
        return result_t { failure_tag, error() };
    }

    if (!rhs)
    {
        return result_t { failure_tag, rhs.error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(rhs.value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(rhs.value()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_copy_constructible_v<E>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>& rhs) && -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if (!*this)
    {
        return result_t { failure_tag, std::move(error()) };
    }

    if (!rhs)
    {
        return result_t { failure_tag, rhs.error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(rhs.value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(rhs.value()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_copy_constructible_v<E>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>&& rhs) const & -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if (!*this)
    {
        return result_t { failure_tag, error() };
    }

    if (!rhs)
    {
        return result_t { failure_tag, std::move(rhs).error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(std::move(rhs).value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(std::move(rhs).value()) };
    }
}

template<typename T, typename E>
template<typename F>
    requires std::is_void_v<T>
          && std::is_invocable_v<F>
          && std::is_move_constructible_v<E>
constexpr auto result<T, E>::apply(result<F, E>&& rhs) && -> result<std::invoke_result_t<F>, E>
{
    using result_t = result<std::invoke_result_t<F>, E>;

    if (!*this)
    {
        return result_t { failure_tag, std::move(error()) };
    }

    if (!rhs)
    {
        return result_t { failure_tag, std::move(rhs).error() };
    }

    if constexpr (std::is_void_v<typename result_t::value_type>)
    {
        return (std::invoke(std::move(rhs).value()), result_t {});
    }
    else
    {
        return result_t { std::invoke(std::move(rhs).value()) };
    }
}

} // namespace ext
