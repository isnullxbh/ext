/**
 * @file    holder.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.05.2021
 */

#ifndef EXT_HOLDER_HPP
#define EXT_HOLDER_HPP

#include <type_traits>
#include <utility>

namespace ext {

template<typename T>
class Holder;

/**
 * @brief   Compares two holders for equality.
 *          For details, see the Holder::equal() member function.
 *
 * @tparam  T    Value type.
 * @param   lhs  Holder to compare.
 * @param   rhs  Holder to compare.
 *
 * @return  If holders are equal - true, otherwise - false.
 */

template<typename T>
constexpr auto operator==(const Holder<T>& lhs, const Holder<T>& rhs) -> bool;

/**
 * @brief   Compares holder with the given value.
 *
 * @tparam  T    Holder value type.
 * @tparam  U    Value type.
 * @param   lhs  Holder to compare.
 * @param   rhs  Value to compare.
 *
 * @return  If the value of the holder is equal to the given one - true, otherwise - false.
 */

template<typename T, typename U>
constexpr auto operator==(const Holder<T>& lhs, const U& rhs) -> bool;

/**
 * @class   Holder
 * @brief   A holder for an object value.
 * @tparam  T  Value type.
 * @since   0.1.0
 */

template<typename T>
class Holder
{
public:

    /**
     * @brief   Constructor.
     *          Default constructor.
     *
     * @throw   Any exception thrown by the default constructor of the type T.
     */

    constexpr Holder() noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>;

    /**
     * @brief   Constructor.
     *          Copy constructor.
     *
     * @param   other  Other holder.
     *
     * @throw   Any exception thrown by the copy constructor of the type T.
     */

    constexpr Holder(const Holder& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires std::is_copy_constructible_v<T>;

    /**
     * @brief   Constructor.
     *          Move constructor.
     *
     * @param   other  Other holder.
     *
     * @throw   Any exception thrown by the move constructor of the type T.
     */

    constexpr Holder(Holder&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T>;

    /**
     * @brief   Constructor.
     *          Constructs holder instance with the given value.
     *
     * @tparam  U      Value type.
     * @param   value  Value.
     *
     * @throw   Any exception thrown by the constructor of the type T.
     */

    template<typename U = T>
    constexpr explicit Holder(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>)
        requires std::is_constructible_v<T, U>;

    /// Destructor
    ~Holder() noexcept(std::is_nothrow_destructible_v<T>) = default;

    /**
     * @brief   Copy assignment operator.
     *
     * @param   other  Other holder.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by the copy assignment operator of the type T.
     */

    constexpr auto operator=(const Holder& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Holder&
        requires std::is_copy_assignable_v<T>;

    /**
     * @brief   Move assignment operator.
     *
     * @param   other  Other holder.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by the move assignment operator of the type T.
     */

    constexpr auto operator=(Holder&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> Holder&
        requires std::is_move_assignable_v<T>;

    /**
     * @brief   Assignment operator.
     *          Assigns the given value to the holder.
     *
     * @tparam  U      Value type.
     * @param   value  Value.
     *
     * @return  A reference to itself.
     *
     * @throw   Any exception thrown by the assignment operator of the type T.
     */

    template<typename U = T>
    constexpr auto operator=(U&& value) noexcept(std::is_nothrow_assignable_v<std::add_lvalue_reference_t<T>, U>) -> Holder&
        requires std::is_assignable_v<std::add_lvalue_reference_t<T>, U>;

public:

    /**
     * @brief   Gets the value of the holder.
     *
     * @return  A reference to the stored value.
     */

    constexpr auto value() & noexcept -> T&;

    /**
     * @brief   Gets the value of the holder.
     *
     * @return  A constant reference to the stored value.
     */

    constexpr auto value() const & noexcept -> const T&;

    /**
     * @brief   Gets the value of the holder.
     *
     * @return  An r-value reference to the stored value.
     */

    constexpr auto value() && noexcept -> T&&;

    /**
     * @brief   Converts the value of the holder to the boolean value.
     *
     * @return  If T is value type - always true.
     *          If T is pointer type - true, if pointer is not nullptr, otherwise - false.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Indicates whether other holder is equal to this one.
     *
     * @param   other  Holder to compare.
     *
     * @return  If holder values are equals - true, otherwise - false.
     *
     * @throw   Any exception thrown by equality operator of the type T.
     */

    constexpr auto equal(const Holder& other) const -> bool;

    /**
     * @brief   Indicates whether the given value is equal to the value stored in the holder.
     *
     * @param   value  Value to compare.
     *
     * @return  If the value of the holder is equal to the given value - true,
     *          otherwise - false.
     *
     * @throw   Any exception thrown by equality operator of the type T.
     */

    constexpr auto equal(const T& value) const -> bool;

protected:
    T m_value;
};

template<typename T>
constexpr Holder<T>::Holder() noexcept(std::is_nothrow_default_constructible_v<T>)
    requires std::is_default_constructible_v<T>
    : m_value()
{}

template<typename T>
constexpr Holder<T>::Holder(const Holder& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
    requires std::is_copy_constructible_v<T>
    : m_value(other.m_value)
{}

template<typename T>
constexpr Holder<T>::Holder(Holder&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T>
    : m_value(std::move(other.m_value))
{}

template<typename T>
template<typename U>
constexpr Holder<T>::Holder(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>)
    requires std::is_constructible_v<T, U>
    : m_value(std::forward<U>(value))
{}

template<typename T>
constexpr auto Holder<T>::operator=(const Holder& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Holder&
    requires std::is_copy_assignable_v<T>
{
    if (this != &other)
    {
        m_value = other.m_value;
    }

    return *this;
}

template<typename T>
constexpr auto Holder<T>::operator=(Holder&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> Holder&
    requires std::is_move_assignable_v<T>
{
    if (this != &other)
    {
        m_value = std::move(other.m_value);
    }

    return *this;
}

template<typename T>
template<typename U>
constexpr auto Holder<T>::operator=(U&& value) noexcept(std::is_nothrow_assignable_v<std::add_lvalue_reference_t<T>, U>) -> Holder&
    requires std::is_assignable_v<std::add_lvalue_reference_t<T>, U>
{
    m_value = std::forward<U>(value);
    return *this;
}

template<typename T>
constexpr auto Holder<T>::value() & noexcept -> T&
{
    return m_value;
}

template<typename T>
constexpr auto Holder<T>::value() const & noexcept -> const T&
{
    return m_value;
}

template<typename T>
constexpr auto Holder<T>::value() && noexcept -> T&&
{
    if constexpr (std::is_pointer_v<T>)
    {
        return std::exchange(m_value, nullptr);
    }
    else
    {
        return std::move(m_value);
    }
}

template<typename T>
constexpr Holder<T>::operator bool() const noexcept
{
    if constexpr (std::is_pointer_v<T>)
    {
        return m_value;
    }
    else
    {
        return true;
    }
}

template<typename T>
constexpr auto Holder<T>::equal(const Holder& other) const -> bool
{
    return m_value == other.m_value;
}

template<typename T>
constexpr auto Holder<T>::equal(const T& value) const -> bool
{
    return m_value == value;
}

/**
 * @class   Holder<T&>
 * @brief   A holder for values of reference types.
 * @tparam  T  Value type.
 */

template<typename T>
class Holder<T&>
{
public:

    /**
     * @brief   Constructor.
     *          Default constructor.
     */

    constexpr Holder() noexcept;

    /**
     * @brief   Constructor.
     *          Copy constructor.
     *
     * @param   other  Other holder.
     */

    constexpr Holder(const Holder& other) noexcept;

    /**
     * @brief   Constructor.
     *          Move constructor.
     *
     * @param   other  Other holder.
     */

    constexpr Holder(Holder&& other) noexcept;

    /**
     * @brief   Constructor.
     *          Constructs holder instance with reference to the given value.
     *
     * @param   value  Value.
     */

    constexpr Holder(T& value) noexcept;

    /// Destructor.
    ~Holder() = default;

    /**
     * @brief   Copy assignment operator.
     *
     * @param   other  Other holder.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(const Holder& other) noexcept -> Holder&;

    /**
     * @brief   Move assignment operator.
     *
     * @param   other  Other holder.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(Holder&& other) noexcept -> Holder&;

    /**
     * @brief   Assignment operator.
     *          Assigns reference to the given value to the holder.
     *
     * @param   value  Value.
     *
     * @return  A reference to itself.
     */

    constexpr auto operator=(T& value) noexcept -> Holder&;

public:

    /**
     * @brief   Gets the value of the holder.
     *
     * @return  A reference to the stored value.
     *
     * @throw   std::runtime_error - will be thrown if the holder is initialized
     *          by a null pointer.
     */

    constexpr auto value() -> T&;

    /**
     * @brief   Gets the value of the holder.
     *
     * @return  A constant reference to the stored value.
     *
     * @throw   std::runtime_error - will be thrown if the holder is initialized
     *          by a null pointer.
     */

    constexpr auto value() const -> const T&;

    /**
     * @brief   Converts the value of the holder to the boolean value.
     *
     * @return  If the value of the holder is not a null pointer - true,
     *          otherwise - false.
     */

    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Indicates whether other holder is equal to this one.
     *
     * @param   other  Holder to compare.
     *
     * @return  If holder values are equals - true, otherwise - false.
     */

    constexpr auto equal(const Holder& other) const -> bool;

    /**
     * @brief   Indicates whether the given value is equal to the value stored in the holder.
     *
     * @param   value  Value to compare.
     *
     * @return  If the value of the holder is equal to the given value - true,
     *          otherwise - false.
     *
     * @throw   std::runtime_error - will be thrown if the holder is initialized
     *          by a null pointer.
     * @throw   Any exception thrown by equality operator of the type T.
     */

    constexpr auto equal(const T& value) const -> bool;

protected:
    T* m_value;
};

template<typename T>
constexpr Holder<T&>::Holder() noexcept
    : m_value(nullptr)
{}

template<typename T>
constexpr Holder<T&>::Holder(const Holder& other) noexcept
    : m_value(other.m_value)
{}

template<typename T>
constexpr Holder<T&>::Holder(Holder&& other) noexcept
    : m_value(std::exchange(other.m_value, nullptr))
{}

template<typename T>
constexpr Holder<T&>::Holder(T& value) noexcept
    : m_value(std::addressof(value))
{}

template<typename T>
constexpr auto Holder<T&>::operator=(const Holder& other) noexcept -> Holder&
{
    if (this != &other)
    {
        m_value = other.m_value;
    }

    return *this;
}

template<typename T>
constexpr auto Holder<T&>::operator=(Holder&& other) noexcept -> Holder&
{
    if (this != &other)
    {
        m_value = std::exchange(other.m_value, nullptr);
    }

    return *this;
}

template<typename T>
constexpr auto Holder<T&>::operator=(T& value) noexcept -> Holder&
{
    m_value = std::addressof(value);
    return *this;
}

template<typename T>
constexpr auto Holder<T&>::value() -> T&
{
    if (!m_value)
    {
        throw std::runtime_error("Attempt to dereference a nullptr");
    }

    return *m_value;
}

template<typename T>
constexpr auto Holder<T&>::value() const -> const T&
{
    if (!m_value)
    {
        throw std::runtime_error("Attempt to dereference a nullptr");
    }

    return *m_value;
}

template<typename T>
constexpr Holder<T&>::operator bool() const noexcept
{
    return static_cast<bool>(m_value);
}

template<typename T>
constexpr auto Holder<T&>::equal(const Holder& other) const -> bool
{
    return m_value == other.m_value;
}

template<typename T>
constexpr auto Holder<T&>::equal(const T& value) const -> bool
{
    return m_value && *m_value == value;
}

template<typename T>
constexpr auto operator==(const Holder<T>& lhs, const Holder<T>& rhs) -> bool
{
    return lhs.equal(rhs);
}

template<typename T, typename U>
constexpr auto operator==(const Holder<T>& lhs, const U& rhs) -> bool
{
    return lhs.equal(rhs);
}

} // namespace ext

#endif // EXT_HOLDER_HPP
