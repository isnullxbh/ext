/**
 * @file    maybe.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    26.05.2021
 */

#ifndef EXT_MAYBE_HPP
#define EXT_MAYBE_HPP

#include <type_traits>
#include <stdexcept>
#include <utility>
#include <functional>
#include <variant>
#include <memory> // std::addressof

#include <ext/base_types.hpp>

namespace ext {

template<typename T>
class Maybe;

namespace traits {

template<typename T>
struct IsMaybe : std::false_type {};

template<typename T>
struct IsMaybe<Maybe<T>> : std::true_type {};

} // namespace traits

/**
 * @brief   Applies the given function to the maybe value.
 *
 * @tparam  T         Maybe value type.
 * @tparam  Function  Function type.
 * @param   maybe     A reference to the maybe.
 * @param   function  Function.
 *
 * @return  If the maybe has a value - result of applying the given function to the value of maybe,
 *          otherwise - empty maybe.
 *
 * @throw   Any exception thrown by the given function.
 *
 * @since   0.1.3
 */

template<typename T, typename Function>
    requires std::is_invocable_v<Function, const T&>
          && traits::IsMaybe<std::invoke_result_t<Function, const T&>>::value
constexpr auto operator>>=(const Maybe<T>& maybe, Function&& function) -> std::invoke_result_t<Function, const T&>;

/**
 * @brief   Compares two maybe for equality.
 *
 * @tparam  T    Maybe value type.
 * @param   lhs  Maybe to compare.
 * @param   rhs  Maybe to compare.
 *
 * @return  If both maybes have a values - the result of value comparison,
 *          if both maybes are empty - true, otherwise - false.
 *
 * @throw   Any exception thrown by the equality compare operator for the type T.
 *
 * @since   0.1.3
 */

template<typename T>
constexpr auto operator==(const Maybe<T>& lhs, const Maybe<T>& rhs) -> bool;

/**
 * @brief   Compares the given maybe with None.
 *
 * @tparam  T    Maybe value type.
 * @param   lhs  Maybe to compare.
 *
 * @return  If the given maybe is empty - true, otherwise - false.
 *
 * @since   0.1.3
 */

template<typename T>
constexpr auto operator==(const Maybe<T>& lhs, const None&) noexcept -> bool;

/**
 * @brief   Compares the maybe with the given value.
 *
 * @tparam  T    Maybe value type.
 * @tparam  U    Value type.
 * @param   lhs  Maybe to compare.
 * @param   rhs  Value to compare.
 *
 * @return  If the maybe has a value - the result of values comparison, otherwise - false.
 *
 * @throw   Any exception thrown by the equality compare operator for the type T.
 *
 * @since   0.1.3
 */

template<typename T, typename U>
constexpr auto operator==(const Maybe<T>& lhs, const U& rhs) -> bool;

/**
 * @class   Maybe
 * @brief   Represents optional value.
 *          A Maybe either contains a value of the type T or it is empty. To indicate that
 *          Maybe is empty used a value of the type None.
 * @tparam  T  Value type.
 * @since   0.1.3
 */

template<typename T>
class Maybe
{
protected:
    using Storage = std::variant<None, T>;

public:

    /**
     * @brief   Constructor.
     *          Creates empty Maybe.
     */

    constexpr Maybe() noexcept = default;

    /**
     * @brief   Constructor.
     *          Copy constructor.
     *
     * @throw   Any exception thrown by the copy constructor of the type T.
     */

    constexpr Maybe(const Maybe&) = default;

    /**
     * @brief   Constructor.
     *          Move constructor.
     *
     * @throw   Any exception thrown by the move constructor of the type T.
     */

    constexpr Maybe(Maybe&&) = default;

    /**
     * @brief   Constructor.
     *          Creates empty Maybe.
     */

    constexpr Maybe(const None&) noexcept;

    /**
     * @brief   Constructor.
     *          Constructs Maybe instance with the given value.
     *
     * @tparam  U      Value type.
     * @param   value  Value.
     */

    template<typename U = T>
        requires std::is_constructible_v<T, U>
    constexpr explicit Maybe(U&& value);

    /// Destructor.
    ~Maybe() = default;

    /**
     * @brief   Copy assignment operator.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by the copy assignment operator of the type T.
     */

    constexpr auto operator=(const Maybe&) -> Maybe& = default;

    /**
     * @brief   Move assignment operator.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by the move assignment operator of the type T.
     */

    constexpr auto operator=(Maybe&&) -> Maybe& = default;

    /**
     * @brief   Assignment operator.
     *          Assigns the given value to the Maybe.
     *
     * @tparam  U      Value type.
     * @param   value  Value.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by assignment operator of the type T.
     */

    template<typename U = T>
        requires std::is_constructible_v<T, U>
    constexpr auto operator=(U&& value) -> Maybe&;

public:

    /**
     * @brief   Indicates that the Maybe has a value.
     *
     * @return  If the Maybe has a value - true, otherwise - false.
     */

    constexpr auto isSome() const noexcept -> bool;

    /**
     * @brief   Indicates that the Maybe is empty.
     *
     * @return  If the Maybe is empty - true, otherwise - false.
     */

    constexpr auto isNone() const noexcept -> bool;

    /**
     * @brief   Converts the Maybe to a boolean value.
     *
     * @return  If the Maybe has a value - true, otherwise - false.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  A reference to value.
     *
     * @throw   std::runtime_error - will be thrown if the Maybe is empty.
     */

    constexpr auto value() -> T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  A constant reference to value.
     *
     * @throw   std::runtime_error - will be thrown if the Maybe is empty.
     */

    constexpr auto value() const -> const T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  If the Maybe has a value - a reference to value, otherwise - reference
     *          to the given value.
     */

    constexpr auto valueOr(T& value) noexcept -> T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  If the Maybe has a value - a reference to value, otherwise - reference
     *          to the given value.
     */

    constexpr auto valueOr(const T& value) const noexcept -> const T&;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Mapping  Mapping type.
     * @param   mapping  Mapping.
     *
     * @return  If the Maybe has a value - result of application the given mapping to the value
     *          of the Maybe wrapped to Maybe, otherwise - empty Maybe.
     *
     * @throw   Any exception thrown at application the given mapping to the value of the specified type.
     */

    template<typename Mapping>
        requires std::is_invocable_v<Mapping, const T&>
    constexpr auto map(Mapping&& mapping) const -> Maybe<std::invoke_result_t<Mapping, const T&>>;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Binder  Binder type.
     * @param   binder  Binder.
     *
     * @return  If the Maybe has a value - result of application the given binder to the value
     *          of the Maybe, otherwise - empty Maybe.
     *
     * @throw   Any exception thrown at application the given mapping to the value of the specified type.
     */

    template<typename Binder>
        requires std::is_invocable_v<Binder, const T&>
              && traits::IsMaybe<std::invoke_result_t<Binder, const T&>>::value
    constexpr auto bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Mutator  Mutator type.
     * @param   mutator  Mutator.
     *
     * @return  A reference to itself.
     */

    template<typename Mutator>
        requires std::is_invocable_v<Mutator, T&>
    constexpr auto mutate(Mutator&& mutator) -> Maybe&;

protected:
    Storage m_storage {};
};

template<typename T>
constexpr Maybe<T>::Maybe(const None&) noexcept
{}

template<typename T>
template<typename U>
    requires std::is_constructible_v<T, U>
constexpr Maybe<T>::Maybe(U&& value)
    : m_storage(T(std::forward<U>(value)))
{}

template<typename T>
template<typename U>
    requires std::is_constructible_v<T, U>
constexpr auto Maybe<T>::operator=(U&& value) -> Maybe&
{
    m_storage = T(std::forward<U>(value));
    return *this;
}

template<typename T>
constexpr auto Maybe<T>::isSome() const noexcept -> bool
{
    return std::holds_alternative<T>(m_storage);
}

template<typename T>
constexpr auto Maybe<T>::isNone() const noexcept -> bool
{
    return std::holds_alternative<None>(m_storage);
}

template<typename T>
constexpr Maybe<T>::operator bool() const noexcept
{
    return std::holds_alternative<T>(m_storage);
}

template<typename T>
constexpr auto Maybe<T>::value() -> T&
{
    if (isNone()) throw std::runtime_error("Bad Maybe access");
    return std::get<T>(m_storage);
}

template<typename T>
constexpr auto Maybe<T>::value() const -> const T&
{
    if (isNone()) throw std::runtime_error("Bad Maybe access");
    return std::get<T>(m_storage);
}

template<typename T>
constexpr auto Maybe<T>::valueOr(T& value) noexcept -> T&
{
    return isSome() ? std::get<T>(m_storage) : value;
}

template<typename T>
constexpr auto Maybe<T>::valueOr(const T& value) const noexcept -> const T&
{
    return isSome() ? std::get<T>(m_storage) : value;
}

template<typename T>
template<typename Mapping>
    requires std::is_invocable_v<Mapping, const T&>
constexpr auto Maybe<T>::map(Mapping&& mapping) const -> Maybe<std::invoke_result_t<Mapping, const T&>>
{
    Maybe<std::invoke_result_t<Mapping, const T&>> maybe {};
    if (isSome())
        maybe = std::invoke(std::forward<Mapping>(mapping), std::get<T>(m_storage));
    return maybe;
}

template<typename T>
template<typename Binder>
requires std::is_invocable_v<Binder, const T&>
      && traits::IsMaybe<std::invoke_result_t<Binder, const T&>>::value
constexpr auto Maybe<T>::bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>
{
    if (isSome())
    {
        return std::invoke(std::forward<Binder>(binder), std::get<T>(m_storage));
    }
    else
    {
        return none;
    }
}

template<typename T>
template<typename Mutator>
    requires std::is_invocable_v<Mutator, T&>
constexpr auto Maybe<T>::mutate(Mutator&& mutator) -> Maybe&
{
    if (isSome())
    {
        std::invoke(std::forward<Mutator>(mutator), std::get<T>(m_storage));
    }

    return *this;
}

/**
 * @class   Maybe<T&>
 * @brief   Maybe template class specialization for reference types.
 * @tparam  T  Value type.
 * @since   0.1.3
 */

template<typename T>
class Maybe<T&>
{
public:

    /**
     * @brief   Constructor.
     *          Constructs empty Maybe instance.
     */

    constexpr Maybe() noexcept = default;

    /**
     * @brief   Constructor.
     *          Copy constructor.
     */

    constexpr Maybe(const Maybe&) noexcept = default;

    /**
     * @brief   Constructor.
     *          Move constructor.
     */

    constexpr Maybe(Maybe&&) noexcept = default;

    /**
     * @brief   Constructor.
     *          Constructs empty Maybe instance.
     */

    constexpr Maybe(const None&) noexcept;

    /**
     * @brief   Constructor.
     *          Constructs Maybe instance with the reference to the given value.
     *
     * @param   value  Value.
     */

    constexpr explicit Maybe(T& value) noexcept;

    /// Destructor
    ~Maybe() = default;

    /**
     * @brief   Copy assignment operator.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(const Maybe&) noexcept -> Maybe& = default;

    /**
     * @brief   Move assignment operator.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(Maybe&&) noexcept -> Maybe& = default;

    /**
     * @brief   Assignment operator.
     *          Assigns the reference to the given value to the Maybe.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(T& value) noexcept -> Maybe&;

public:
    /**
     * @brief   Indicates that the Maybe has a value.
     *
     * @return  If the Maybe has a value - true, otherwise - false.
     */

    constexpr auto isSome() const noexcept -> bool;

    /**
     * @brief   Indicates that the Maybe is empty.
     *
     * @return  If the Maybe is empty - true, otherwise - false.
     */

    constexpr auto isNone() const noexcept -> bool;

    /**
     * @brief   Converts the Maybe to a boolean value.
     *
     * @return  If the Maybe has a value - true, otherwise - false.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  A reference to value.
     *
     * @throw   std::runtime_error - will be thrown if the Maybe is empty.
     */

    constexpr auto value() -> T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  A constant reference to value.
     *
     * @throw   std::runtime_error - will be thrown if the Maybe is empty.
     */

    constexpr auto value() const -> const T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  If the Maybe has a value - a reference to value, otherwise - reference
     *          to the given value.
     */

    constexpr auto valueOr(T& value) noexcept -> T&;

    /**
     * @brief   Gets the value of the Maybe.
     *
     * @return  If the Maybe has a value - a reference to value, otherwise - reference
     *          to the given value.
     */

    constexpr auto valueOr(const T& value) const noexcept -> const T&;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Mapping  Mapping type.
     * @param   mapping  Mapping.
     *
     * @return  If the Maybe has a value - result of application the given mapping to the value
     *          of the Maybe wrapped to Maybe, otherwise - empty Maybe.
     *
     * @throw   Any exception thrown at application the given mapping to the value of the specified type.
     */

    template<typename Mapping>
        requires std::is_invocable_v<Mapping, const T&>
    constexpr auto map(Mapping&& mapping) const -> Maybe<std::invoke_result_t<Mapping, const T&>>;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Binder  Binder type.
     * @param   binder  Binder.
     *
     * @return  If the Maybe has a value - result of application the given binder to the value
     *          of the Maybe, otherwise - empty Maybe.
     *
     * @throw   Any exception thrown at application the given mapping to the value of the specified type.
     */

    template<typename Binder>
        requires std::is_invocable_v<Binder, const T&>
              && traits::IsMaybe<std::invoke_result_t<Binder, const T&>>::value
    constexpr auto bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>;

    /**
     * @brief   Applies the given functional object to the value of the Maybe.
     *
     * @tparam  Mutator  Mutator type.
     * @param   mutator  Mutator.
     *
     * @return  A reference to itself.
     */

    template<typename Mutator>
        requires std::is_invocable_v<Mutator, T&>
    constexpr auto mutate(Mutator&& mutator) -> Maybe&;

protected:
    T* m_ptr {};
};

template<typename T>
constexpr Maybe<T&>::Maybe(const None&) noexcept
{}

template<typename T>
constexpr Maybe<T&>::Maybe(T& value) noexcept
    : m_ptr(std::addressof(value))
{}

template<typename T>
constexpr auto Maybe<T&>::operator=(T& value) noexcept -> Maybe&
{
    m_ptr = std::addressof(value);
    return *this;
}

template<typename T>
constexpr auto Maybe<T&>::isSome() const noexcept -> bool
{
    return static_cast<bool>(m_ptr);
}

template<typename T>
constexpr auto Maybe<T&>::isNone() const noexcept -> bool
{
    return !static_cast<bool>(m_ptr);
}

template<typename T>
constexpr Maybe<T&>::operator bool() const noexcept
{
    return static_cast<bool>(m_ptr);
}

template<typename T>
constexpr auto Maybe<T&>::value() -> T&
{
    if (isNone()) throw std::runtime_error("Bad Maybe access");
    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::value() const -> const T&
{
    if (isNone()) throw std::runtime_error("Bad Maybe access");
    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::valueOr(T& value) noexcept -> T&
{
    return isSome() ? *m_ptr : value;
}

template<typename T>
constexpr auto Maybe<T&>::valueOr(const T& value) const noexcept -> const T&
{
    return isSome() ? *m_ptr : value;
}

template<typename T>
template<typename Mapping>
    requires std::is_invocable_v<Mapping, const T&>
constexpr auto Maybe<T&>::map(Mapping&& mapping) const -> Maybe<std::invoke_result_t<Mapping, const T&>>
{
    Maybe<std::invoke_result_t<Mapping, const T&>> maybe {};
    if (isSome())
        maybe = std::invoke(std::forward<Mapping>(mapping), *m_ptr);
    return maybe;
}

template<typename T>
template<typename Binder>
requires std::is_invocable_v<Binder, const T&>
      && traits::IsMaybe<std::invoke_result_t<Binder, const T&>>::value
constexpr auto Maybe<T&>::bind(Binder&& binder) const -> std::invoke_result_t<Binder, const T&>
{
    if (isSome())
    {
        return std::invoke(std::forward<Binder>(binder), *m_ptr);
    }
    else
    {
        return none;
    }
}

template<typename T>
template<typename Mutator>
    requires std::is_invocable_v<Mutator, T&>
constexpr auto Maybe<T&>::mutate(Mutator&& mutator) -> Maybe&
{
    if (isSome())
    {
        std::invoke(std::forward<Mutator>(mutator), *m_ptr);
    }

    return *this;
}

template<typename T, typename Function>
requires std::is_invocable_v<Function, const T&>
         && traits::IsMaybe<std::invoke_result_t<Function, const T&>>::value
constexpr auto operator>>=(const Maybe<T>& maybe, Function&& function) -> std::invoke_result_t<Function, const T&>
{
    return maybe.bind(std::move(function));
}

template<typename T>
constexpr auto operator==(const Maybe<T>& lhs, const Maybe<T>& rhs) -> bool
{
    if (static_cast<bool>(lhs) != static_cast<bool>(rhs))
        return false;

    if (lhs.isNone()) return true;

    return lhs.value() == rhs.value();
}

template<typename T>
constexpr auto operator==(const Maybe<T>& lhs, const None&) noexcept -> bool
{
    return lhs.isNone();
}

template<typename T, typename U>
constexpr auto operator==(const Maybe<T>& lhs, const U& rhs) -> bool
{
    if (lhs.isNone()) return false;

    return lhs.value() == rhs;
}

} // namespace ext

#endif // EXT_MAYBE_HPP
