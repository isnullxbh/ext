/**
 * @file    fixed_string.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    06.01.2024
 */

#pragma once

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string_view>

#include <ext/detail/fixed_string.hpp>

/// @defgroup ext-strings-fixstr ext::basic_fixed_string
/// @ingroup  ext-strings

namespace ext
{

template<typename Char, std::size_t N>
class basic_fixed_string;

/// Creates fixed string from the specified character array.
/// @tparam  Char Character type.
/// @tparam  N    Length.
/// @param   rhs  Character array.
/// @return  Created fixed string.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<typename Char, std::size_t N>
constexpr auto make_fixed_string(const Char(&rhs)[N]) -> basic_fixed_string<Char, N - 1>;

/// Creates fixed string from the specified character sequence.
/// @tparam  Str Fixed string.
/// @return  Created fixed string.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<basic_fixed_string Str>
constexpr auto operator ""_fs() noexcept -> decltype(Str);

/// Compares strings for equality.
/// @tparam  Char Character type.
/// @tparam  N    Length.
/// @param   lhs  String to compare.
/// @param   rhs  String to compare.
/// @return  If the contents of the string are the same - true, otherwise - false.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<typename Char, std::size_t N>
constexpr auto operator==(const basic_fixed_string<Char, N>& lhs, const basic_fixed_string<Char, N>& rhs) noexcept -> bool;

/// Compares string to char array for equality.
/// @tparam  Char Character type.
/// @tparam  N    Length.
/// @param   lhs  String to compare.
/// @param   rhs  Character array to compare.
/// @return  If contents of the string and character array are the same - true, otherwise - false.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<typename Char, std::size_t N>
constexpr auto operator==(const basic_fixed_string<Char, N>& lhs, const Char(&rhs)[N + 1]) noexcept -> bool;

/// Fixed-length string.
/// @tparam  Char Character type.
/// @tparam  N    Length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
/// @todo    Add support for backward iteration.
/// @todo    Implement the following operations: replace, copy, find, contains, substr.
template<typename Char, std::size_t N>
class basic_fixed_string
{
public:
    using value_type      = Char;               ///< Character type.
    using size_type       = std::size_t;        ///< Size type.
    using reference       = value_type&;        ///< A type of reference to character.
    using const_reference = const value_type&;  ///< A type of const reference to character.
    using pointer         = Char*;              ///< A type of pointer to character.
    using const_pointer   = const Char*;        ///< A type of const pointer to character.
    using iterator        = pointer;            ///< Iterator type.
    using const_iterator  = const_pointer;      ///< Const iterator type.

public:
    /// Constructs an empty string.
    constexpr basic_fixed_string() noexcept;

    /// Constructs string from the specified string literal.
    /// @param rhs String literal.
    constexpr basic_fixed_string(const value_type(&rhs)[N + 1]) noexcept;

    /// Constructs a copy of the specified string.
    /// @param rhs String to copy.
    constexpr basic_fixed_string(const basic_fixed_string& rhs) noexcept = default;

    /// Assignes the content of the specified character array to the string.
    /// @param  rhs Character array.
    /// @return A reference to the current object.
    constexpr auto operator=(const value_type(&rhs)[N + 1]) noexcept -> basic_fixed_string&;

    /// Copy assignment operator.
    /// Replaces the content of the string to content of the specified string.
    /// @param  rhs String to assign.
    /// @return A reference to the current object.
    constexpr auto operator=(const basic_fixed_string& rhs) noexcept -> basic_fixed_string& = default;

    /// Gets a reference to the character at the specified index.
    /// @tparam Pos Character index.
    /// @return A reference to the character.
    template<std::size_t Pos>
        requires (Pos < N)
    constexpr auto at() noexcept -> reference;

    /// Gets a constant reference to the character at the specified index.
    /// @tparam Pos Character index.
    /// @return A constant reference to the character.
    template<std::size_t Pos>
        requires (Pos < N)
    constexpr auto at() const noexcept -> const_reference;

    /// Gets a reference to the character at the specified index.
    /// @param  pos Character index.
    /// @return A reference to the character.
    /// @throws std::out_of_range Will be thrown if the specified index greater or equal to the string size.
    constexpr auto at(size_type pos) -> reference;

    /// Gets a constant reference to the character at the specified index.
    /// @param  pos Character index.
    /// @return A constant reference to the character.
    /// @throws std::out_of_range Will be thrown if the specified index greater or equal to the string size.
    constexpr auto at(size_type pos) const -> const_reference;

    /// Gets a reference to the character at the specified index.
    /// @param  pos Character index.
    /// @return A reference to the character.
    constexpr auto operator[](size_type pos) noexcept -> reference;

    /// Gets a constant reference to the character at the specified index.
    /// @param  pos Character index.
    /// @return A constant reference to the character.
    constexpr auto operator[](size_type pos) const noexcept -> const_reference;

    /// Gets a reference to the first character in the string.
    /// This member-function is not accessible if the string is empty (i.e. N equal to 0).
    /// @return A reference to character.
    constexpr auto front() noexcept -> reference requires (N > 0);

    /// Gets a constant reference to the first character in the string.
    /// This member-function is not accessible if the string is empty (i.e. N equal to 0).
    /// @return A constant reference to character.
    constexpr auto front() const noexcept -> const_reference requires (N > 0);

    /// Gets a reference to the last character in the string.
    /// This member-function is not accessible if the string is empty (i.e. N equal to 0).
    /// @return A reference to character.
    constexpr auto back() noexcept -> reference requires (N > 0);

    /// Gets a constant reference to the last character in the string.
    /// This member-function is not accessible if the string is empty (i.e. N equal to 0).
    /// @return A constant reference to character.
    constexpr auto back() const noexcept -> const_reference requires (N > 0);

    /// Gets a pointer to the first character of a string.
    /// @return A pointer to character.
    constexpr auto data() noexcept -> pointer;

    /// Gets a pointer to the first character of a string.
    /// @return A pointer to character.
    constexpr auto data() const noexcept -> const_pointer;

    /// Gets a pointer to the null-terminated character array with string data.
    /// @return A pointer to character array.
    constexpr auto c_str() const noexcept -> const_pointer;

    /// Creates a std::basic_string_view constructed from the string content.
    /// @return A std::basic_string_view.
    constexpr operator std::basic_string_view<Char>() const noexcept;

    /// Gets an iterator to the first character of the string.
    /// @return An iterator.
    constexpr auto begin() noexcept -> iterator;

    /// Gets an iterator to the character following the last character of the string.
    /// @return An iterator.
    constexpr auto end() noexcept -> iterator;

    /// Gets a constant iterator to the first character of the string.
    /// @return An iterator.
    constexpr auto begin() const noexcept -> const_iterator;

    /// Gets a constant iterator to the character following the last character of the string.
    /// @return An iterator.
    constexpr auto end() const noexcept -> const_iterator;

    /// Gets a constant iterator to the first character of the string.
    /// @return An iterator.
    constexpr auto cbegin() const noexcept -> const_iterator;

    /// Gets a constant iterator to the character following the last character of the string.
    /// @return An iterator.
    constexpr auto cend() const noexcept -> const_iterator;

    /// Checks whether the string is empty.
    /// @return If the string is empty - true, otherwise - false.
    constexpr auto empty() const noexcept -> bool;

    /// Gets the number of characters in the string.
    /// @return Number of characters.
    constexpr auto size() const noexcept -> size_type;

    /// Gets a max number of characters the string is able to hold.
    /// @return Number of characters.
    constexpr auto max_size() const noexcept -> size_type;

    /// Gets a new string constructed by applying the specified function to each character of the current string.
    /// @tparam Mapper Function type.
    /// @param  mapper Function.
    /// @return Constructed string.
    template<typename Mapper>
    constexpr auto map(Mapper&& mapper) const noexcept -> basic_fixed_string<std::invoke_result_t<Mapper, value_type>, N>
        requires std::is_invocable_v<Mapper, value_type>;

    /// Checks if the string starts with the specified prefix.
    /// @param  prefix Prefix.
    /// @return If the string starts with the specified prefix - true, otherwise - false.
    constexpr auto starts_with(std::basic_string_view<value_type> prefix) const noexcept -> bool;

    /// Checks if the string ends with the specified suffix.
    /// @param  suffix Suffix.
    /// @return If the string ends with the specified suffix - true, otherwise - false.
    constexpr auto ends_with(std::basic_string_view<value_type> suffix) const noexcept -> bool;

    /// Represents a position in a string that does not exist.
    static constexpr size_type npos = static_cast<size_type>(-1);

public:
    value_type _data[N + 1]; ///< Data (including the terminating null character).
    size_type  _size { N };  ///< Size (without the terminating null character).
};

/// An auxiliary type alias template that defines the fixed string of length N and char as the character type.
/// @tparam  N String length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

/// An auxiliary type alias template that defines the fixed string of length N and wchar_t as the character type.
/// @tparam  N String length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<std::size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

/// An auxiliary type alias template that defines the fixed string of length N and char8_t as the character type.
/// @tparam  N String length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<std::size_t N>
using fixed_u8string = basic_fixed_string<char8_t, N>;

/// An auxiliary type alias template that defines the fixed string of length N and char16_t as the character type.
/// @tparam  N String length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<std::size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;

/// An auxiliary type alias template that defines the fixed string of length N and char32_t as the character type.
/// @tparam  N String length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<std::size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;

/// CTAD for the basic_fixed_string.
/// @tparam  Char Character type.
/// @tparam  N    Length.
/// @ingroup ext-strings-fixstr
/// @since   0.2.0
template<typename Char, std::size_t N>
basic_fixed_string(const Char(&)[N]) -> basic_fixed_string<Char, N - 1>;

template<typename Char, std::size_t N>
constexpr basic_fixed_string<Char, N>::basic_fixed_string() noexcept
    : _data()
{}

template<typename Char, std::size_t N>
constexpr basic_fixed_string<Char, N>::basic_fixed_string(const value_type (&rhs)[N + 1]) noexcept
{
    for (size_type i = 0; i < N; ++i)
    {
        _data[i] = rhs[i];
    }
    _data[N] = static_cast<value_type>(detail::null_character);
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::operator=(const value_type (&rhs)[N + 1]) noexcept -> basic_fixed_string&
{
    for (size_type i = 0; i < N; ++i)
    {
        _data[i] = rhs[i];
    }
    _data[N] = static_cast<value_type>(detail::null_character);
    return *this;
}

template<typename Char, std::size_t N>
template<std::size_t Pos>
    requires(Pos < N)
constexpr auto basic_fixed_string<Char, N>::at() noexcept -> reference
{
    return _data[Pos];
}

template<typename Char, std::size_t N>
template<std::size_t Pos>
    requires(Pos < N)
constexpr auto basic_fixed_string<Char, N>::at() const noexcept -> const_reference
{
    return _data[Pos];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::at(size_type pos) -> reference
{
    if (pos >= size())
    {
        throw std::out_of_range { "Index out of range" };
    }
    return _data[pos];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::at(size_type pos) const -> const_reference
{
    if (pos >= size())
    {
        throw std::out_of_range { "Index out of range" };
    }
    return _data[pos];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::operator[](size_type pos) noexcept -> reference
{
    assert(pos < size());
    return _data[pos];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::operator[](size_type pos) const noexcept -> const_reference
{
    assert(pos <= size());
    return _data[pos];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::front() noexcept -> reference
    requires (N > 0)
{
    return _data[0];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::front() const noexcept -> const_reference
    requires (N > 0)
{
    return _data[0];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::back() noexcept -> reference
    requires (N > 0)
{
    return _data[N - 1];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::back() const noexcept -> const_reference
    requires (N > 0)
{
    return _data[N - 1];
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::data() noexcept -> pointer
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::data() const noexcept -> const_pointer
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::c_str() const noexcept -> const_pointer
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr basic_fixed_string<Char, N>::operator std::basic_string_view<Char>() const noexcept
{
    return std::basic_string_view<Char>{ _data, _data + size() };
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::begin() noexcept -> iterator
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::end() noexcept -> iterator
{
    return _data + N;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::begin() const noexcept -> const_iterator
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::end() const noexcept -> const_iterator
{
    return _data + N;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::cbegin() const noexcept -> const_iterator
{
    return _data;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::cend() const noexcept -> const_iterator
{
    return _data + N;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::empty() const noexcept -> bool
{
    return _size == 0;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::size() const noexcept -> size_type
{
    return _size;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::max_size() const noexcept -> size_type
{
    return _size;
}

template<typename Char, std::size_t N>
template<typename Mapper>
constexpr auto basic_fixed_string<Char, N>::map(Mapper&& mapper) const noexcept
    -> basic_fixed_string<std::invoke_result_t<Mapper, value_type>, N>
    requires std::is_invocable_v<Mapper, Char>
{
    basic_fixed_string<std::invoke_result_t<Mapper, value_type>, N> str {};
    for (size_type i = 0; i < size(); ++i)
    {
        str[i] = mapper(_data[i]);
    }
    return str;
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::starts_with(std::basic_string_view<value_type> prefix) const noexcept -> bool
{
    return std::basic_string_view<value_type>(*this).starts_with(prefix);
}

template<typename Char, std::size_t N>
constexpr auto basic_fixed_string<Char, N>::ends_with(std::basic_string_view<value_type> suffix) const noexcept -> bool
{
    return std::basic_string_view<value_type>(*this).ends_with(suffix);
}

template<basic_fixed_string Str>
constexpr auto operator ""_fs() noexcept -> decltype(Str)
{
    return Str;
}

template<typename Char, std::size_t N>
constexpr auto make_fixed_string(const Char(&rhs)[N]) -> basic_fixed_string<Char, N - 1>
{
    return { rhs };
}

template<typename Char, std::size_t N>
constexpr auto operator==(const basic_fixed_string<Char, N>& lhs, const basic_fixed_string<Char, N>& rhs) noexcept -> bool
{
    using string_type = basic_fixed_string<Char, N>;

    auto equal = true;
    for (typename string_type::size_type i = 0; i < lhs.size(); ++i)
    {
        if (lhs.data()[i] != rhs.data()[i])
        {
            equal = false;
            break;
        }
    }
    return equal;
}

template<typename Char, std::size_t N>
constexpr auto operator==(const basic_fixed_string<Char, N>& lhs, const Char(&rhs)[N + 1]) noexcept -> bool
{
    using string_type = basic_fixed_string<Char, N>;

    auto equal = true;
    for (typename string_type::size_type i = 0; i < lhs.size(); ++i)
    {
        if (lhs.data()[i] != rhs[i])
        {
            equal = false;
            break;
        }
    }
    return equal;
}

} // namespace ext
