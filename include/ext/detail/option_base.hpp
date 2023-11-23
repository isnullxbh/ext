/**
 * @file    option_base.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    21.11.2023
 */

#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

namespace ext::detail
{

template<typename T, auto = std::is_trivially_destructible_v<T>>
class option_storage
{
public:
    constexpr option_storage() noexcept
        : _empty()
        , _has_value(false)
    {}

    template<typename U>
    constexpr explicit option_storage(U&& value)
        : _value(std::forward<U>(value))
        , _has_value(true)
    {}

    template<typename... Args>
    constexpr explicit option_storage(std::in_place_t, Args&&... args)
        : _value(std::forward<Args>(args)...)
        , _has_value(true)
    {}

    ~option_storage() = default;

public:
    union
    {
        struct {} _empty;
        T         _value;
    };
    bool _has_value;
};

template<typename T>
class option_storage<T, false>
{
public:
    constexpr option_storage() noexcept
        : _empty()
        , _has_value(false)
    {}

    template<typename U>
    constexpr explicit option_storage(U&& value)
        : _value(std::forward<U>(value))
        , _has_value(true)
    {}

    template<typename... Args>
    constexpr explicit option_storage(std::in_place_t, Args&&... args)
        : _value(std::forward<Args>(args)...)
        , _has_value(true)
    {}

    constexpr ~option_storage() noexcept
    {
        if (_has_value)
        {
            _value.~T();
        }
    }

public:
    union
    {
        struct {} _empty;
        T         _value;
    };
    bool _has_value;
};

template<typename T, auto = std::is_trivially_copy_constructible_v<T>>
class option_copy_ctor : public option_storage<T>
{
public:
    using option_storage<T>::option_storage;
    option_copy_ctor(const option_copy_ctor&) = default;
};

template<typename T>
class option_copy_ctor<T, false> : public option_storage<T>
{
public:
    using option_storage<T>::option_storage;

    constexpr option_copy_ctor(const option_copy_ctor& rhs) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires std::is_copy_constructible_v<T>
        : option_storage<T>()
    {
        if (rhs._has_value)
        {
            std::construct_at(&this->_value, rhs._value);
            this->_has_value = true;
        }
    }
};

template<typename T, auto = std::is_trivially_move_constructible_v<T>>
class option_move_ctor : public option_copy_ctor<T>
{
public:
    using option_copy_ctor<T>::option_copy_ctor;
    option_move_ctor(const option_move_ctor&) = default;
    option_move_ctor(option_move_ctor&&) = default;
    auto operator=(const option_move_ctor&) -> option_move_ctor& = default;
};

template<typename T>
class option_move_ctor<T, false> : public option_copy_ctor<T>
{
public:
    using option_copy_ctor<T>::option_copy_ctor;
    option_move_ctor(const option_move_ctor&) = default;
    auto operator=(const option_move_ctor&) -> option_move_ctor& = default;

    constexpr option_move_ctor(option_move_ctor&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T>
        : option_copy_ctor<T>()
    {
        if (rhs._has_value)
        {
            std::construct_at(&this->_value, std::move(rhs._value));
            this->_has_value = true;
        }
    }
};

template<typename T,
         auto = std::is_trivially_copy_assignable_v<T>
             && std::is_trivially_copy_constructible_v<T>
             && std::is_trivially_destructible_v<T>>
class option_copy_assignment : public option_move_ctor<T>
{
public:
    using option_move_ctor<T>::option_move_ctor;
    auto operator=(const option_copy_assignment&) -> option_copy_assignment& = default;
};

template<typename T>
class option_copy_assignment<T, false> : public option_move_ctor<T>
{
public:
    using option_move_ctor<T>::option_move_ctor;

    constexpr auto operator=(const option_copy_assignment& rhs) -> option_copy_assignment&
        requires std::is_copy_constructible_v<T>
              && std::is_copy_assignable_v<T>
    {
        if (this->_has_value && rhs._has_value)
        {
            if (this != &rhs)
            {
                this->_value = rhs._value;
            }
        }
        else if (this->_has_value && !rhs._has_value)
        {
            std::destroy_at(&this->_value);
            this->_has_value = false;
        }
        else if (!this->_has_value && rhs._has_value)
        {
            std::construct_at(&this->_value, rhs._value);
            this->_has_value = true;
        }
        return *this;
    }
};

template<typename T,
         auto = std::is_trivially_move_assignable_v<T>
             && std::is_trivially_move_constructible_v<T>
             && std::is_trivially_destructible_v<T>>
class option_move_assignment : public option_copy_assignment<T>
{
public:
    using option_copy_assignment<T>::option_copy_assignment;
    option_move_assignment(const option_move_assignment&) = default;
    auto operator=(const option_move_assignment&) -> option_move_assignment& = default;
    auto operator=(option_move_assignment&&) -> option_move_assignment& = default;
};

template<typename T>
class option_move_assignment<T, false> : public option_copy_assignment<T>
{
public:
    using option_copy_assignment<T>::option_copy_assignment;
    option_move_assignment(const option_move_assignment&) = default;
    auto operator=(const option_move_assignment&) -> option_move_assignment& = default;

    constexpr auto operator=(option_move_assignment&& rhs) noexcept(
        std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>) -> option_move_assignment&
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T>
    {
        if (this->_has_value && rhs._has_value)
        {
            if (this != &rhs)
            {
                this->_value = std::move(rhs._value);
            }
        }
        else if (this->_has_value && !rhs._has_value)
        {
            std::destroy_at(&this->_value);
            this->_has_value = false;
        }
        else if (!this->_has_value && rhs._has_value)
        {
            std::construct_at(&this->_value, std::move(rhs._value));
            this->_has_value = true;
        }
        return *this;
    }
};

template<typename T>
class option_base : public option_move_assignment<T>
{
public:
    using option_move_assignment<T>::option_move_assignment;

    template<typename U>
    constexpr void construct(U&& value)
    {
        if constexpr (std::is_trivially_constructible_v<T, U>)
        {
            this->_value = std::forward<U>(value);
            this->_has_value = true;
        }
        else
        {
            std::construct_at(&this->_value, std::forward<U>(value));
            this->_has_value = true;
        }
    }

    constexpr auto has_value() const noexcept
    {
        return this->_has_value;
    }

    constexpr auto get_value() noexcept -> T&
    {
        assert(this->_has_value);
        return this->_value;
    }

    constexpr auto get_value() const noexcept -> const T&
    {
        assert(this->_has_value);
        return this->_value;
    }
};

template<typename T>
class option_base<T&>
{
public:
    constexpr option_base() noexcept
        : _ptr(nullptr)
    {}

    option_base(const option_base&) = default;

    option_base(option_base&&) = default;

    template<typename U>
    constexpr explicit option_base(U& value)
        : _ptr(std::addressof(value))
    {}

    auto operator=(const option_base&) -> option_base& = default;

    auto operator=(option_base&&) -> option_base& = default;

    constexpr auto has_value() const noexcept
    {
        return _ptr != nullptr;
    }

    constexpr auto get_value() const noexcept -> T&
    {
        assert(_ptr != nullptr);
        return *_ptr;
    }

    constexpr void assign(T& value) noexcept
    {
        _ptr = std::addressof(value);
    }

private:
    T* _ptr;
};

} // namespace ext::detail
