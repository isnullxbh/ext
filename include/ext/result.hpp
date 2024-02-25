/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <ext/detail/result/result_move_constructor.hpp>
#include <ext/value_wrapper.hpp>

/// @defgroup ext-utility-result
/// @ingroup  ext-utility

namespace ext
{

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
public:
    /// Default constructor.
    result() = default;

    /// Copy constructor.
    result(const result&) = default;

    /// Move constructor.
    result(result&&) = default;

    /// Destructor.
    ~result() = default;

    /// Gets the status of the result.
    /// @return Result status.
    constexpr auto status() const noexcept -> result_status;
};

template<typename T, typename E>
constexpr auto result<T, E>::status() const noexcept -> result_status
{
    return this->_status;
}

} // namespace ext
