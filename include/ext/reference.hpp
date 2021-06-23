/**
 * @file    reference.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.06.2021
 */

#ifndef EXT_REFERENCE_HPP
#define EXT_REFERENCE_HPP

#include <memory>

#include <ext/exceptions.hpp>
#include <ext/maybe.hpp>

namespace ext {

constexpr struct {} nullref;
using nullref_t = decltype(nullref);

template<typename T>
class Reference;

template<typename T>
constexpr auto operator==(const Reference<T>& lhs, const Reference<T>& rhs) noexcept -> bool;

template<typename T, typename U>
constexpr auto operator==(const Reference<T>& lhs, const U& rhs) -> bool;

template<typename T>
constexpr auto operator==(const Reference<T>& lhs, nullref_t) noexcept -> bool;

/**
 * @brief   Wrap the given value of the reference type to Reference.
 *
 * @tparam  T      Value type.
 * @param   value  A reference to value.
 *
 * @return  Reference.
 *
 * @since   0.1.4
 */

template<typename T>
constexpr auto ref(T& value) -> Reference<T>;

/**
 * @brief   Wrap the given value of the reference type to Reference.
 *
 * @tparam  T      Value type.
 * @param   value  A reference to value.
 *
 * @return  Reference.
 */

template<typename T>
constexpr auto cref(T& value) -> Reference<const T>;

/**
 * @brief   Unwrap reference to the Maybe.
 *
 * @tparam  T    Value type.
 * @param   ref  Reference.
 *
 * @return  If reference is not null - Maybe<T&> with valid value,
 *          otherwise - none.
 */

template<typename T>
constexpr auto unwrap(Reference<T>& ref) -> Maybe<T&>;

/**
 * @class   Reference
 * @brief   A wrapper for values of reference types.
 * @tparam  T  Value type.
 * @since   0.1.4
 */

template<typename T>
class Reference
{
public:
    constexpr Reference() = default;
    constexpr Reference(const Reference& other) noexcept;
    constexpr explicit Reference(T& value) noexcept;
    constexpr explicit Reference(nullref_t nullref) noexcept;

    ~Reference() = default;

    constexpr auto operator=(const Reference& other) -> Reference&;
    constexpr auto operator=(T& value) noexcept -> Reference&;
    constexpr auto operator=(nullref_t) noexcept -> Reference&;

public:

    /**
     * @brief   Gets the reference.
     *
     * @return  A stored reference.
     *
     * @throw   NullPointerException - will be thrown if the reference is default-constructed
     *          or initialized by nullref.
     */

    constexpr auto get() -> T&;

    /**
     * @brief   Gets the reference.
     *
     * @return  A stored reference.
     *
     * @throw   NullPointerException - will be thrown if the reference is default-constructed
     *          or initialized by nullref.
     */

    constexpr auto get() const -> const T&;

    /**
     * @brief   Checks if the reference is null.
     *
     * @return  If the reference is default-constructed or initialized by nullref - true,
     *          otherwise - false.
     */

    constexpr auto isNull() const noexcept -> bool;

    /**
     * @brief   Checks if the wrapper store a valid reference.
     *
     * @return  If the reference is default-constructed or initialized by nullref - true,
     *          otherwise - false.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Gets the reference.
     *
     * @return  A stored reference.
     *
     * @throw   NullPointerException - will be thrown if the reference is default-constructed
     *          or initialized by nullref.
     */

    constexpr explicit operator T&();

    /**
     * @brief   Gets the reference.
     *
     * @return  A stored reference.
     *
     * @throw   NullPointerException - will be thrown if the reference is default-constructed
     *          or initialized by nullref.
     */

    constexpr explicit operator const T&() const;

    /**
     * @brief   Gets a pointer to the value associated with the stored reference.
     *
     * @return  A pointer to the value.
     */

    constexpr auto operator->() noexcept -> T*;

    /**
     * @brief   Gets a pointer to the value associated with the stored reference.
     *
     * @return  A pointer to the value.
     */

    constexpr auto operator->() const noexcept -> const T*;

    friend auto operator==<T>(const Reference<T>& lhs, const Reference<T>& rhs) noexcept -> bool;
    friend auto operator==<T>(const Reference<T>& lhs, nullref_t) noexcept -> bool;

protected:
    T* m_ptr = nullptr;
};

template<typename T>
constexpr Reference<T>::Reference(const Reference& other) noexcept
    : m_ptr(other.m_ptr)
{}

template<typename T>
constexpr Reference<T>::Reference(T& value) noexcept
    : m_ptr(std::addressof(value))
{}

template<typename T>
constexpr Reference<T>::Reference(nullref_t) noexcept
    : m_ptr(nullptr)
{}

template<typename T>
constexpr auto Reference<T>::operator=(const Reference& other) -> Reference&
{
    if (this != &other)
    {
        m_ptr = other.m_ptr;
    }

    return *this;
}

template<typename T>
constexpr auto Reference<T>::operator=(T& value) noexcept -> Reference&
{
    m_ptr = std::addressof(value);
    return *this;
}

template<typename T>
constexpr auto Reference<T>::operator=(nullref_t) noexcept -> Reference&
{
    m_ptr = nullptr;
    return *this;
}

template<typename T>
constexpr auto Reference<T>::get() -> T&
{
    if (!m_ptr)
        throw NullPointerException("Attempt to dereference a null reference");

    return *m_ptr;
}

template<typename T>
constexpr auto Reference<T>::get() const -> const T&
{
    if (!m_ptr)
        throw NullPointerException("Attempt to dereference a null reference");

    return *m_ptr;
}

template<typename T>
constexpr auto Reference<T>::isNull() const noexcept -> bool
{
    return !static_cast<bool>(m_ptr);
}

template<typename T>
constexpr Reference<T>::operator bool() const noexcept
{
    return static_cast<bool>(m_ptr);
}

template<typename T>
constexpr Reference<T>::operator T&()
{
    if (!m_ptr)
        throw NullPointerException("Attempt to dereference a null reference");

    return *m_ptr;
}

template<typename T>
constexpr Reference<T>::operator const T&() const
{
    if (!m_ptr)
        throw NullPointerException("Attempt to dereference a null reference");

    return *m_ptr;
}

template<typename T>
constexpr auto Reference<T>::operator->() noexcept -> T*
{
    return m_ptr;
}

template<typename T>
constexpr auto Reference<T>::operator->() const noexcept -> const T*
{
    return m_ptr;
}

template<typename T>
constexpr auto operator==(const Reference<T>& lhs, const Reference<T>& rhs) noexcept -> bool
{
    return lhs.m_ptr == rhs.m_ptr;
}

template<typename T, typename U>
constexpr auto operator==(const Reference<T>& lhs, const U& value) -> bool
{
    return lhs.isNull()
         ? false
         : (lhs.get() == value);
}

template<typename T>
constexpr auto operator==(const Reference<T>& lhs, nullref_t) noexcept -> bool
{
    return !static_cast<bool>(lhs.m_ptr);
}

template<typename T>
constexpr auto ref(T& value) -> Reference<T>
{
    return Reference<T>(value);
}

template<typename T>
constexpr auto cref(T& value) -> Reference<const T>
{
    return Reference<const T>(value);
}

template<typename T>
constexpr auto unwrap(Reference<T>& ref) -> Maybe<T&>
{
    Maybe<T&> maybe {};
    if (ref)
    {
        maybe = ref.get();
    }
    return maybe;
}

} // namespace ext

#endif // EXT_REFERENCE_HPP
