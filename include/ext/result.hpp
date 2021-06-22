/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

#ifndef EXT_RESULT_HPP
#define EXT_RESULT_HPP

#include <exception>
#include <variant>
#include <functional>

#include <ext/holder.hpp>

namespace ext {

template<typename T, typename E>
class Result;

namespace traits {

template<typename T>
struct IsResult : std::false_type {};

template<typename T, typename E>
struct IsResult<Result<T, E>> : std::true_type {};

} // namespace traits

/**
 * @brief   Binds the given function to the result.
 *
 * @tparam  T         Result value type.
 * @tparam  E         Result error type.
 * @tparam  Function  Function type.
 * @param   result    Result.
 * @param   fn        Function.
 *
 * @return  If the result is success - a new result containing the value which is result
 *          of application the given function to the result value, otherwise - new result
 *          containing error value copied from the original result.
 */

template<typename T, typename E, typename Function>
    requires std::is_invocable_v<Function, const T&>
          && traits::IsResult<std::invoke_result_t<Function, const T&>>::value
constexpr auto operator>>=(const Result<T, E>& result, Function&& fn) -> decltype(auto);

/**
 * @brief   Applies the given function to the result value.
 *
 * @tparam  T         Result value type.
 * @tparam  E         Result error type.
 * @tparam  Function  Function type.
 * @param   result    A reference to the result.
 * @param   function  Function.
 *
 * @return  If the result is success - a new result containing the value which is result
 *          of application the given function to the result value, otherwise - new result
 *          containing error value copied from the original result.
 */

template<typename T, typename E, typename Function>
    requires std::is_invocable_v<Function, const T&>
constexpr auto operator|(const Result<T, E>& result, Function&& function) -> decltype(auto);

/**
 * @brief   Compares two results for equality.
 *
 * @tparam  T    Value type.
 * @tparam  E    Error type.
 * @param   lhs  Result to compare.
 * @param   rhs  Result to compare.
 *
 * @return  If results are equal - true, otherwise - false.
 */

template<typename T, typename E>
constexpr auto operator==(const Result<T, E>& lhs, const Result<T, E>& rhs) -> bool;

/**
 * @brief   Compares the result value with the given value.
 *
 * @tparam  T    Result value type.
 * @tparam  E    Result error type.
 * @tparam  U    Value type.
 * @param   lhs  Result to compare.
 * @param   rhs  Value to compare.
 *
 * @return  If the result is failure - always false.
 *          If the result is success - if the result success value is equal to
 *          the given value - true, otherwise - false.
 */

template<typename T, typename E, typename U>
constexpr auto operator==(const Result<T, E>& lhs, const U& rhs) -> bool;

/**
 * @class   Success
 *          A named holder intended for creating Result instance containing
 *          a value.
 * @tparam  T  Value type.
 */

template<typename T> struct Success : Holder<T> { using Holder<T>::Holder; };

/**
 * @class   Failure
 *          A named holder intended for creating Result instance containing
 *          an error value.
 * @tparam  E  Error type.
 */

template<typename E> struct Failure : Holder<E> { using Holder<E>::Holder; };

/**
 * @brief   Compares two Failures for equality.
 *
 * @tparam  E    Error type.
 * @param   lhs  Failure to compare.
 * @param   rhs  Failure to compare.
 *
 * @return  If failure values are equal - true, otherwise - false.
 */

template<typename E>
constexpr auto operator==(const Failure<E>& lhs, const Failure<E>& rhs) -> bool;

/**
 * @class   Result
 * @brief   Represents either success or failure.
 *          Value of the type T represents success.
 *          Value of the type E represents failure.
 *
 * @tparam  T  Value type.
 * @tparam  E  Error type.
 *
 * @since   0.1.0
 */

template<typename T, typename E>
class Result
{
protected:
    using Storage = std::variant<Holder<T>, Failure<E>>;

public:

    /**
     * @brief   Constructor.
     *          Default constructor marked as deleted because result instance
     *          must be created with either a value or an error.
     */

    Result() = delete;

    /**
     * @brief   Constructor.
     *          Copy constructor.
     */

    constexpr Result(const Result&) = default;

    /**
     * @brief   Constructor.
     *          Move constructor.
     */

    constexpr Result(Result&&) = default;

    /**
     * @brief   Constructor.
     *          Constructs result instance with the value stored in the @a success holder.
     *
     * @param   success  Value holder.
     */

    constexpr Result(Success<T>&& success);

    /**
     * @brief   Constructor.
     *          Constructs result instance with the error stored in the @a failure holder.
     *
     * @param   failure  Error holder.
     */

    constexpr Result(Failure<E>&& failure);

    /// Destructor.
    ~Result() = default;

    /**
     * @brief   Copy assignment operator.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(const Result&) -> Result& = default;

    /**
     * @brief   Move assignment operator.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(Result&&) -> Result& = default;

public:

    /**
     * @brief   Indicates that the result store a value.
     *
     * @return  If result store a value of the value type - true, otherwise - false.
     */

    constexpr auto success() const noexcept -> bool;

    /**
     * @brief   Indicates that the result store an error.
     *
     * @return  If result store a value of the error type - true, otherwise - false.
     */

    constexpr auto failure() const noexcept -> bool;

    /**
     * @brief   Converts the result to a boolean value.
     *
     * @return  If the result store a value of an error type - false, otherwise - true.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  A reference to the stored value.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the error type.
     */

    constexpr auto value() & -> std::remove_reference_t<T>&;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  A const reference to the stored value.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the error type.
     */

    constexpr auto value() const & -> const std::remove_reference_t<T>&;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  A forwarding reference to the stored value.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the error type.
     */

    constexpr auto value() && -> std::remove_reference_t<T>&&;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  A forwarding reference to the stored value.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the error type.
     */

    constexpr auto value() const && -> const std::remove_reference_t<T>&&;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  If result is success - a reference to the stored value,
     *          otherwise - reference to the given value.
     */

    constexpr auto valueOr(T& value) noexcept -> std::remove_reference_t<T>&;

    /**
     * @brief   Gets the value of the result.
     *
     * @return  If result is success - a reference to the stored value,
     *          otherwise - reference to the given value.
     */

    constexpr auto valueOr(const T& value) const noexcept -> const std::remove_reference_t<T>&;

    /**
     * @brief   Gets the error of the result.
     *
     * @return  A reference to the stored error.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the value type.
     */

    constexpr auto error() & -> std::remove_reference_t<E>&;

    /**
     * @brief   Gets the error of the result.
     *
     * @return  A const reference to the stored error.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the value type.
     */

    constexpr auto error() const & -> const std::remove_reference_t<E>&;

    /**
     * @brief   Gets the error of the result.
     *
     * @return  A forwarding reference to the stored error.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the value type.
     */

    constexpr auto error() && -> std::remove_reference_t<E>&&;

    /**
     * @brief   Gets the error of the result.
     *
     * @return  A forwarding reference to the stored error.
     *
     * @throw   std::bad_variant_access - will be thrown if the result store
     *          a value of the value type.
     */

    constexpr auto error() const && -> const std::remove_reference_t<E>&&;

    /**
     * @brief   Rethrows exception.
     *          The method is define only if the error type is std::exception_ptr
     *          or the error type is derived from the std::exception.
     */

    constexpr auto rethrow() const -> void requires std::is_base_of_v<std::exception, E>
                                                 or std::is_same_v<std::exception_ptr, E>;

    friend auto operator==<T, E>(const Result<T, E>& lhs, const Result<T, E>& rhs) -> bool;

    /**
     * @brief   Applies the given mapping to the result value.
     *
     * @tparam  Mapping  Mapping type (T -> 'T).
     * @param   mapping  Mapping.
     *
     * @return  If the result is success - a new result containing the value which is result
     *          of application the given mapping to the result value, otherwise - new result
     *          containing error value copied from the original result.
     */

    template<typename Mapping>
        requires std::is_invocable_v<Mapping, const T&>
    constexpr auto map(Mapping&& mapping) const -> Result<std::invoke_result_t<Mapping, const T&>, E>;

    /**
     * @brief   Applies the given binder to the result value.
     *
     * @tparam  Binder  Binder type (T -> Result<'T, E>).
     * @param   binder  Binder.
     *
     * @return  If the result is success - a new result containing the value which is result
     *          of application the given mapping to the result value, otherwise - new result
     *          containing error value copied from the original result.
     */

    template<typename Binder>
        requires std::is_invocable_v<Binder, const T&>
    constexpr auto bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>;

    /**
     * @brief   Applies the given mutator to the result value.
     *          If the result is success - modify the result by application a mutator to the result value,
     *          otherwise - do nothing.
     *
     * @tparam  Mutator  Mutator type (T& -> <unused>)
     * @param   mutator  Mutator.
     *
     * @return  A reference to itself.
     */

    template<typename Mutator>
        requires std::is_invocable_v<Mutator, T&>
    constexpr auto mutate(Mutator&& mutator) -> Result&;

protected:
    Storage m_storage;
};

template<typename T, typename E>
constexpr Result<T, E>::Result(Success<T>&& success)
    : m_storage(std::move(success))
{}

template<typename T, typename E>
constexpr Result<T, E>::Result(Failure<E>&& failure)
    : m_storage(std::move(failure))
{}

template<typename T, typename E>
constexpr auto Result<T, E>::success() const noexcept -> bool
{
    return std::holds_alternative<Holder<T>>(m_storage);
}

template<typename T, typename E>
constexpr auto Result<T, E>::failure() const noexcept -> bool
{
    return std::holds_alternative<Failure<E>>(m_storage);
}

template<typename T, typename E>
constexpr Result<T, E>::operator bool() const noexcept
{
    return std::holds_alternative<Holder<T>>(m_storage);
}

template<typename T, typename E>
constexpr auto Result<T, E>::value() & -> std::remove_reference_t<T>&
{
    return std::get<Holder<T>>(m_storage).value();
}

template<typename T, typename E>
constexpr auto Result<T, E>::value() const & -> const std::remove_reference_t<T>&
{
    return std::get<Holder<T>>(m_storage).value();
}

template<typename T, typename E>
constexpr auto Result<T, E>::value() && -> std::remove_reference_t<T>&&
{
    return std::move(std::get<Holder<T>>(m_storage).value());
}

template<typename T, typename E>
constexpr auto Result<T, E>::value() const && -> const std::remove_reference_t<T>&&
{
    return std::move(std::get<Holder<T>>(m_storage).value());
}

template<typename T, typename E>
constexpr auto Result<T, E>::valueOr(T& value) noexcept -> std::remove_reference_t<T>&
{
    return success() ? std::get<Holder<T>>(m_storage).value() : value;
}

template<typename T, typename E>
constexpr auto Result<T, E>::valueOr(const T& value) const noexcept -> const std::remove_reference_t<T>&
{
    return success() ? std::get<Holder<T>>(m_storage).value() : value;
}

template<typename T, typename E>
constexpr auto Result<T, E>::error() & -> std::remove_reference_t<E>&
{
    return std::get<Failure<E>>(m_storage).value();
}

template<typename T, typename E>
constexpr auto Result<T, E>::error() const & -> const std::remove_reference_t<E>&
{
    return std::get<Failure<E>>(m_storage).value();
}

template<typename T, typename E>
constexpr auto Result<T, E>::error() && -> std::remove_reference_t<E>&&
{
    return std::move(std::get<Failure<E>>(m_storage).value());
}

template<typename T, typename E>
constexpr auto Result<T, E>::error() const && -> const std::remove_reference_t<E>&&
{
    return std::move(std::get<Failure<E>>(m_storage).value());
}

template<typename T, typename E>
constexpr auto Result<T, E>::rethrow() const -> void requires std::is_base_of_v<std::exception, E>
                                                           or std::is_same_v<std::exception_ptr, E>
{
    if (failure())
    {
        if constexpr (std::is_same_v<std::exception_ptr, E>)
        {
            std::rethrow_exception(error());
        }
        else
        {
            throw error();
        }
    }
}

template<typename T, typename E>
template<typename Mapping>
    requires std::is_invocable_v<Mapping, const T&>
constexpr auto Result<T, E>::map(Mapping&& mapping) const -> Result<std::invoke_result_t<Mapping, const T&>, E>
{
    if (success())
    {
        return Success<std::invoke_result_t<Mapping, const T&>>(std::invoke(std::forward<Mapping>(mapping), value()));
    }
    else
    {
        return Failure<E>(error());
    }
}

template<typename T, typename E>
template<typename Binder>
    requires std::is_invocable_v<Binder, const T&>
constexpr auto Result<T, E>::bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>
{
    if (success())
    {
        return std::invoke(std::forward<Binder>(binder), value());
    }
    else
    {
        return Failure<E>(error());
    }
}

template<typename T, typename E>
template<typename Mutator>
    requires std::is_invocable_v<Mutator, T&>
constexpr auto Result<T, E>::mutate(Mutator&& mutator) -> Result&
{
    if (success())
    {
        std::invoke(std::forward<Mutator>(mutator), value());
    }
    return *this;
}

template<typename T, typename E, typename Function>
requires std::is_invocable_v<Function, const T&>
         && traits::IsResult<std::invoke_result_t<Function, const T&>>::value
constexpr auto operator>>=(const Result<T, E>& result, Function&& fn) -> decltype(auto)
{
    return result.bind(std::forward<Function>(fn));
}

template<typename T, typename E, typename Function>
requires std::is_invocable_v<Function, const T&>
constexpr auto operator|(const Result<T, E>& result, Function&& function) -> decltype(auto)
{
    return result.map(std::forward<Function>(function));
}

template<typename T, typename E>
constexpr auto operator==(const Result<T, E>& lhs, const Result<T, E>& rhs) -> bool
{
    return lhs.m_storage == rhs.m_storage;
}

template<typename T, typename E, typename U>
constexpr auto operator==(const Result<T, E>& lhs, const U& rhs) -> bool
{
    return lhs.success() && lhs.value() == rhs;
}

template<typename E>
constexpr auto operator==(const Failure<E>& lhs, const Failure<E>& rhs) -> bool
{
    return lhs.equal(static_cast<const Holder<E>&>(rhs));
}

} // namespace ext

#endif // EXT_RESULT_HPP
