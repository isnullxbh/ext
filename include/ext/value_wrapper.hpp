/**
 * @file    value_wrapper.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    20.01.2024
 */

#pragma once

#include <utility>

#include <ext/detail/value_wrapper.hpp>

/// @defgroup ext-utility-valuewrapper Value Wrapper
/// @ingroup  ext-utility

/// @defgroup ext-utility-valuewrapper-extensions Extensions
/// @ingroup  ext-utility-valuewrapper

namespace ext
{

template<typename T, typename Tag = void, template<typename> typename... Extensions>
class value_wrapper;

/// A type trait to check if the specified type is value wrapper.
/// @tparam  T Type to check.
/// @ingroup ext-utility-valuewrapper
/// @since   0.1.0
template<typename T>
struct is_value_wrapper : detail::is_value_wrapper<std::remove_cv_t<T>>
{};

/// An auxiliary type alias template for the is_value_wrapper type trait.
/// @tparam  T Type to check.
/// @ingroup ext-utility-valuewrapper
/// @since   0.1.0
template<typename T>
constexpr inline bool is_value_wrapper_v = is_value_wrapper<T>::value;

/// Compares for equality value wrapper with value.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extention types.
/// @param   lhs        Value wrapper to compare.
/// @param   rhs        Value wrapper.
/// @return  If value wrapper's value and the specified value are equal - true, otherwise - false.
/// @ingroup ext-utility-valuewrapper
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions,
         typename U>
    requires std::negation_v<is_value_wrapper<std::remove_reference_t<U>>>
constexpr auto operator==(const value_wrapper<T, Tag, Extensions...>& lhs, const U& rhs) -> bool;

/// Compares value wrapper with value.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extention types.
/// @param   lhs        Value wrapper to compare.
/// @param   rhs        Value wrapper.
/// @return  If value wrapper's value and the specified value are equal - true, otherwise - false.
/// @ingroup ext-utility-valuewrapper
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions,
         typename U>
    requires std::negation_v<is_value_wrapper<std::remove_reference_t<U>>>
constexpr auto operator<=>(const value_wrapper<T, Tag, Extensions...>& lhs, const U& rhs);

/// A value wrapper.
///
/// Used to construct types with special meaning. For example, in matrix implementation we can use std::size_t for both
/// row index and column index, but sometimes this result to errors caused by the fact that indexes has the same type
/// and we can easy confuse them. To distinguish column and row indexes we can wrap std::size_t in value_wrapper (adding
/// different tags to them, sure) and and now it will be a two different types.
///
/// Extensions allow us to extend the value wrapper functionality with friend operators. Thus, such operations as
/// increment, decrement, arithmetic, logical can be added through extensions.
///
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Classes that extend the value wrapper functionality.
/// @ingroup ext-utility-valuewrapper
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class value_wrapper
    : public Extensions<value_wrapper<T, Tag, Extensions...>>...
{
    static_assert(std::is_reference_v<T>, "T must not be reference type");

public:
    using value_type = T;   ///< Value type.
    using tag        = Tag; ///< Tag type.

public:
    /// Default constructor.
    value_wrapper() = default;

    /// Copy constructor.
    value_wrapper(const value_wrapper&) = default;

    /// Move constructor.
    value_wrapper(value_wrapper&&) = default;

    /// Constructs value wrapper that contains the specified value.
    /// @tparam U     Value type.
    /// @param  value Value.
    template<typename U>
        requires (!is_value_wrapper_v<std::remove_reference_t<U>>)
              && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
    constexpr value_wrapper(U&& value) noexcept(std::is_constructible_v<T, U>);

    /// Constructs value wrapper that contains value constructed from the specified arguments.
    /// @tparam Args Argument types.
    /// @param  args Arguments.
    template<typename... Args>
        requires std::is_constructible_v<T, Args...>
    constexpr explicit value_wrapper(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>);

    /// Destructor.
    ~value_wrapper() = default;

    /// Copy assignment operator.
    auto operator=(const value_wrapper&) -> value_wrapper& = default;

    /// Move assignment operator.
    auto operator=(value_wrapper&&) -> value_wrapper& = default;

    /// Replaces the value wrapper value with the specified one.
    /// @tparam U     Value type.
    /// @param  value Value.
    /// @return A reference to the current object.
    template<typename U>
        requires std::is_assignable_v<T&, U>
              && (!is_value_wrapper_v<U>)
    constexpr auto operator=(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U>) -> value_wrapper&;

    /// Gets the stored value.
    /// @return A reference to the value.
    constexpr auto value() & noexcept -> value_type&;

    /// Gets the stored value.
    /// @return A reference to the value.
    constexpr auto value() const & noexcept -> const value_type&;

    /// Gets the stored value.
    /// @return A reference to the value.
    constexpr auto value() && noexcept -> value_type&&;

    /// Gets the stored value.
    /// @return A reference to the value.
    constexpr auto value() const && noexcept -> const value_type&&;

    /// Compares for equality with the specified value wrapper.
    /// @param  rhs Value wrapper to compare.
    /// @return If value wrappers values are equal - true, otherwise - false.
    constexpr auto operator==(const value_wrapper& rhs) const -> bool;

    /// Compares with the specified value wrapper.
    /// @param  rhs Value wrapper to compare.
    /// @return Comparison result.
    constexpr auto operator<=>(const value_wrapper& rhs) const;

    /// Swap states of the current and specified value wrappers.
    /// @param rhs Other value wrapper.
    constexpr void swap(value_wrapper& rhs) noexcept;

private:
    value_type _value {}; ///< Value.
};

/// CTAD for value wrapper.
/// @tparam  U Value type.
/// @ingroup ext-utility-valuewrapper
template<typename U>
value_wrapper(U&&) -> value_wrapper<std::remove_cvref_t<U>>;

template<typename T, typename Tag, template<typename> typename... Extensions>
template<typename U>
    requires (!is_value_wrapper_v<std::remove_reference_t<U>>)
          && (!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
constexpr value_wrapper<T, Tag, Extensions...>::value_wrapper(U&& value) noexcept(std::is_constructible_v<T, U>)
    : _value(std::forward<U>(value))
{}

template<typename T, typename Tag, template<typename> typename... Extensions>
template<typename... Args>
    requires std::is_constructible_v<T, Args...>
constexpr value_wrapper<T, Tag, Extensions...>::value_wrapper(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    : _value(std::forward<Args>(args)...)
{}

template<typename T, typename Tag, template<typename> typename... Extensions>
template<typename U>
    requires std::is_assignable_v<T&, U>
          && (!is_value_wrapper_v<U>)
constexpr auto value_wrapper<T, Tag, Extensions...>::operator=(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U>)
    -> value_wrapper&
{
    _value = std::forward<U>(value);
    return *this;
}

template<typename T, typename Tag, template<typename> typename... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::value() & noexcept -> value_type&
{
    return _value;
}

template<typename T, typename Tag, template<typename> typename... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::value() const & noexcept -> const value_type&
{
    return _value;
}

template<typename T, typename Tag, template<typename> typename... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::value() && noexcept -> value_type&&
{
    return std::move(_value);
}

template<typename T, typename Tag, template<typename> typename... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::value() const && noexcept -> const value_type&&
{
    return std::move(_value);
}

template<typename T, typename Tag, template<typename> class... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::operator==(const value_wrapper& rhs) const -> bool
{
    return _value == rhs.value();
}

template<typename T, typename Tag, template<typename> class... Extensions>
constexpr auto value_wrapper<T, Tag, Extensions...>::operator<=>(const value_wrapper& rhs) const
{
    return _value <=> rhs.value();
}

template<typename T, typename Tag, template<typename> class... Extensions>
constexpr void value_wrapper<T, Tag, Extensions...>::swap(value_wrapper& rhs) noexcept
{
    using std::swap;
    swap(_value, rhs._value);
}

template<typename T, typename Tag, template<typename> typename... Extensions,
         typename U>
    requires std::negation_v<is_value_wrapper<std::remove_reference_t<U>>>
constexpr auto operator==(const value_wrapper<T, Tag, Extensions...>& lhs, const U& rhs) -> bool
{
    return lhs.value() == rhs;
}

template<typename T, typename Tag, template<typename> typename... Extensions,
         typename U>
    requires std::negation_v<is_value_wrapper<std::remove_reference_t<U>>>
constexpr auto operator<=>(const value_wrapper<T, Tag, Extensions...>& lhs, const U& rhs)
{
    return lhs.value() <=> rhs;
}

/// A namespace that contains extension classes for ext::value_wrapper.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
namespace value_wrapper_extensions
{

template<typename T>
class addition;

/// Defines addition operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class addition<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Addition assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator+=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() += rhs.value();
        return lhs;
    }

    /// Addition operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of addition.
    friend constexpr auto operator+(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() + rhs.value() };
    }
};

template<typename T>
class subtraction;

/// Defines subtraction operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class subtraction<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Subtraction assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator-=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() -= rhs.value();
        return lhs;
    }

    /// Subtraction operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of subtraction.
    friend constexpr auto operator-(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() - rhs.value() };
    }
};

template<typename T>
class multiplication;

/// Defines multiplication operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class multiplication<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Multiplication assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator*=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() *= rhs.value();
        return lhs;
    }

    /// Multiplication operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of multiplication.
    friend constexpr auto operator*(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() * rhs.value() };
    }
};

template<typename T>
class division;

/// Defines division operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class division<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Division assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator/=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() /= rhs.value();
        return lhs;
    }

    /// Division operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of division.
    friend constexpr auto operator/(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() / rhs.value() };
    }
};

template<typename T>
class remainder;

/// Defines remainder operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class remainder<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Remainder assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator%=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() %= rhs.value();
        return lhs;
    }

    /// Remainder operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of remainder.
    friend constexpr auto operator%(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() % rhs.value() };
    }
};

template<typename T>
class bitwise;

/// Defines bitwise operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class bitwise<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Bitwise AND assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator&=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() &= rhs.value();
        return lhs;
    }

    /// Bitwise OR assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator|=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() |= rhs.value();
        return lhs;
    }

    /// Bitwise XOR assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator^=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() ^= rhs.value();
        return lhs;
    }

    /// Bitwise left shift assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator<<=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() <<= rhs.value();
        return lhs;
    }

    /// Bitwise right shift assignment operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return A reference to the first operand.
    friend constexpr auto operator>>=(wrapper& lhs, const wrapper& rhs) -> wrapper&
    {
        lhs.value() >>= rhs.value();
        return lhs;
    }

    /// Bitwise NOT operator.
    /// @param  lhs First operand.
    /// @return Result of bitwise NOT.
    friend constexpr auto operator~(const wrapper& lhs) -> wrapper
    {
        return wrapper { static_cast<typename wrapper::value_type>(~lhs.value()) };
    }

    /// Bitwise AND operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of bitwise AND.
    friend constexpr auto operator&(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() & rhs.value() };
    }

    /// Bitwise OR operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of bitwise OR.
    friend constexpr auto operator|(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() | rhs.value() };
    }

    /// Bitwise XOR operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of bitwise XOR.
    friend constexpr auto operator^(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() ^ rhs.value() };
    }

    /// Bitwise left shift operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of bitwise left shift.
    friend constexpr auto operator<<(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() << rhs.value() };
    }

    /// Bitwise right shift operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operand.
    /// @return Result of bitwise right shift.
    friend constexpr auto operator>>(const wrapper& lhs, const wrapper& rhs) -> wrapper
    {
        return wrapper { lhs.value() >> rhs.value() };
    }
};

template<typename T>
class increment;

/// Defines increment operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class increment<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Pre-increment operator.
    /// @param  lhs Value to increment.
    /// @return A reference to the incremented value.
    friend constexpr auto operator++(wrapper& lhs) -> wrapper&
    {
        ++lhs.value();
        return lhs;
    }

    /// Pre-increment operator.
    /// @param  lhs Value to increment.
    /// @return Incremented value.
    friend constexpr auto operator++(wrapper& lhs, int) -> wrapper
    {
        wrapper tmp { lhs };
        ++lhs.value();
        return tmp;
    }
};

template<typename T>
class decrement;

/// Defines decrement operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class decrement<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Pre-decrement operator.
    /// @param  lhs Value to decrement.
    /// @return A reference to the decremented value.
    friend constexpr auto operator--(wrapper& lhs) -> wrapper&
    {
        --lhs.value();
        return lhs;
    }

    /// Post-decrement operator.
    /// @param  lhs Value to decrement.
    /// @return Decremented value.
    friend constexpr auto operator--(wrapper& lhs, int) -> wrapper
    {
        wrapper tmp { lhs };
        --lhs.value();
        return tmp;
    }
};

template<typename T>
class arithmetic;

/// Defines arithmetic operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class arithmetic<value_wrapper<T, Tag, Extensions...>>
    : public addition<value_wrapper<T, Tag, Extensions...>>
    , public subtraction<value_wrapper<T, Tag, Extensions...>>
    , public multiplication<value_wrapper<T, Tag, Extensions...>>
    , public division<value_wrapper<T, Tag, Extensions...>>
    , public remainder<value_wrapper<T, Tag, Extensions...>>
    , public increment<value_wrapper<T, Tag, Extensions...>>
    , public decrement<value_wrapper<T, Tag, Extensions...>>
    , public bitwise<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Unary plus operator.
    /// @param  lhs Operand.
    /// @return A reference to the operand.
    friend constexpr auto operator+(const wrapper& lhs) -> wrapper
    {
        return wrapper { +lhs.value() };
    }

    /// Unary minus operator.
    /// @param  lhs Operand.
    /// @return A reference to the operand.
    friend constexpr auto operator-(const wrapper& lhs) -> wrapper
    {
        return wrapper { -lhs.value() };
    }
};

template<typename T>
class logical;

/// Defines logical operations for ext::value_wrapper.
/// @tparam  T          Value type.
/// @tparam  Tag        Tag type.
/// @tparam  Extensions Extension types.
/// @ingroup ext-utility-valuewrapper-extensions
/// @since   0.1.0
template<typename T, typename Tag, template<typename> typename... Extensions>
class logical<value_wrapper<T, Tag, Extensions...>>
{
private:
    using wrapper = value_wrapper<T, Tag, Extensions...>;

public:
    /// Logical NOT operator.
    /// @param  lhs Operand.
    /// @return Result of applying operator to the stored value.
    friend constexpr auto operator!(const wrapper& lhs) -> bool
    {
        return !lhs.value();
    }

    /// Logical AND operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operator.
    /// @return Result of applying operator to the stored values.
    friend constexpr auto operator&&(const wrapper& lhs, const wrapper& rhs) -> bool
    {
        return lhs.value() && rhs.value();
    }

    /// Logical OR operator.
    /// @param  lhs First operand.
    /// @param  rhs Second operator.
    /// @return Result of applying operator to the stored values.
    friend constexpr auto operator||(const wrapper& lhs, const wrapper& rhs) -> bool
    {
        return lhs.value() || rhs.value();
    }
};

} // namespace value_wrapper_extensions
} // namespace ext
