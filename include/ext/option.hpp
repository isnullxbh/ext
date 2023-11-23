/**
 * @file    option.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.11.2023
 */

#pragma once

#include <exception>
#include <type_traits>

#include <ext/none.hpp>
#include <ext/detail/option_base.hpp>

namespace ext
{

/// An exception that indicates access to the option object that does not contain a value.
/// @ingroup ext-utility
/// @since   0.1.0
class bad_option_access : public std::exception
{
public:
    const char* what() const noexcept { return "Bad option access"; }
};

/// Represents an optional value.
/// @tparam T Value type.
/// @ingroup ext-utility
/// @since   0.1.0
template<typename T>
class option : private detail::option_base<T>
{
public:
    /// Constructs an object that does not contain a value.
    option() = default;

    /// Constructs an object that does not contain a value.
    /// @param value None.
    constexpr option([[maybe_unused]] none_t value) noexcept
        : detail::option_base<T>()
    {}

    /// Copy constructor.
    option(const option&) = default;

    /// Move constructor.
    option(option&&) = default;

    /// Constructs an object that contains value initialized by the @a value.
    /// @tparam U     Value type.
    /// @param  value Value.
    template<typename U>
        requires (!std::is_reference_v<T>)
              && std::is_constructible_v<T, U>
              && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
              && (!std::is_same_v<std::remove_cvref_t<U>, option<T>>)
    constexpr explicit(!std::is_convertible_v<U, T>) option(U&& value)
        noexcept(std::is_nothrow_constructible_v<T, U>);

    /// Constructs an object that contains a value constructed from the specified arguments @a args.
    /// @tparam Args Argument types.
    /// @param  tag  In-place tag.
    /// @param  args Arguments.
    template<typename... Args>
        requires (!std::is_reference_v<T>)
              && std::is_constructible_v<T, Args...>
    constexpr explicit option(std::in_place_t tag, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>);

    /// Constructs an object that contains a reference to the specified value.
    /// @tparam U     Value type.
    /// @param  value Value.
    template<typename U>
        requires std::is_reference_v<T>
              && std::is_assignable_v<std::remove_cvref_t<T>&, U&>
              && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
              && (!std::is_same_v<std::remove_cvref_t<U>, option<T>>)
    constexpr explicit(!std::is_convertible_v<U, T>) option(U& value) noexcept;

    /// Constructs an object that contains a value initialized by rhs value.
    /// @tparam U   Value type.
    /// @param  rhs Other option.
    template<typename U>
        requires (!std::is_reference_v<T>)
              && std::is_constructible_v<T, const U&>
    constexpr explicit(!std::is_convertible_v<const U&, T>) option(const option<U>& rhs);

    /// Constructs an object that contains a value initialized by rhs value.
    /// @tparam U   Value type.
    /// @param  rhs Other option.
    template<typename U>
        requires (!std::is_reference_v<T>)
              && std::is_constructible_v<T, U>
    constexpr explicit(!std::is_convertible_v<U, T>) option(option<U>&& rhs);

    /// Constructs an object that contains a reference to the same object as rhs.
    /// @tparam U   Reference type.
    /// @param  rhs Other option.
    template<typename U>
        requires std::is_reference_v<T>
              && std::is_reference_v<U>
              && std::is_convertible_v<U, T>
    constexpr option(option<U> rhs) noexcept;

    /// Destructor.
    ~option() = default;

    /// Copy assignment operator.
    /// @return A reference to itself.
    auto operator=(const option&) -> option& = default;

    /// Move assignment operator.
    /// @return A reference to itself.
    auto operator=(option&&) -> option& = default;

    using detail::option_base<T>::has_value;

    /// Gets contained value.
    /// @throws bad_option_access Will be thrown if the option doesn't contain a value.
    constexpr auto value() & -> std::remove_reference_t<T>&;

    /// Gets contained value.
    /// @throws bad_option_access Will be thrown if the option doesn't contain a value.
    constexpr auto value() const& -> const std::remove_reference_t<T>&;

    /// Gets contained value.
    /// @throws bad_option_access Will be thrown if the option doesn't contain a value.
    constexpr auto value() && -> std::remove_reference_t<T>&&;

    /// Gets contained value.
    /// @throws bad_option_access Will be thrown if the option doesn't contain a value.
    constexpr auto value() const&& -> const std::remove_reference_t<T>&&;
};

template<typename T>
template<typename U>
    requires (!std::is_reference_v<T>)
          && std::is_constructible_v<T, U>
          && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
          && (!std::is_same_v<std::remove_cvref_t<U>, option<T>>)
constexpr option<T>::option(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>)
    : detail::option_base<T>(std::forward<U>(value))
{}

template<typename T>
template<typename... Args>
    requires (!std::is_reference_v<T>)
          && std::is_constructible_v<T, Args...>
constexpr option<T>::option(std::in_place_t tag, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    : detail::option_base<T>(tag, std::forward<Args>(args)...)
{}

template<typename T>
template<typename U>
    requires std::is_reference_v<T>
          && std::is_assignable_v<std::remove_cvref_t<T>&, U&>
          && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
          && (!std::is_same_v<std::remove_cvref_t<U>, option<T>>)
constexpr option<T>::option(U& value) noexcept
    : detail::option_base<T>(value)
{}

template<typename T>
template<typename U>
    requires (!std::is_reference_v<T>)
          && std::is_constructible_v<T, const U&>
constexpr option<T>::option(const option<U>& rhs)
    : detail::option_base<T>()
{
    if (rhs.has_value())
    {
        this->construct(rhs.value());
    }
}

template<typename T>
template<typename U>
    requires (!std::is_reference_v<T>)
          && std::is_constructible_v<T, U>
constexpr option<T>::option(option<U>&& rhs)
    : detail::option_base<T>()
{
    if (rhs.has_value())
    {
        this->construct(std::move(rhs.value()));
    }
}

template<typename T>
template<typename U>
    requires std::is_reference_v<T>
          && std::is_reference_v<U>
          && std::is_convertible_v<U, T>
constexpr option<T>::option(option<U> rhs) noexcept
    : detail::option_base<T>()
{
    if (rhs.has_value())
    {
        this->assign(rhs.value());
    }
}

template<typename T>
constexpr auto option<T>::value() & -> std::remove_reference_t<T>&
{
    return has_value() ? this->get_value() : throw bad_option_access();
}

template<typename T>
constexpr auto option<T>::value() const & -> const std::remove_reference_t<T>&
{
    return has_value() ? this->get_value() : throw bad_option_access();
}

template<typename T>
constexpr auto option<T>::value() && -> std::remove_reference_t<T>&&
{
    return has_value() ? std::move(this->get_value()) : throw bad_option_access();
}

template<typename T>
constexpr auto option<T>::value() const && -> const std::remove_reference_t<T>&&
{
    return has_value() ? std::move(this->get_value()) : throw bad_option_access();
}

} // namespace ext
