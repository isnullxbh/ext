/**
 * @file    exceptions.ipp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.06.2021
 */

#ifndef EXT_EXCEPTIONS_IPP
#define EXT_EXCEPTIONS_IPP

#include <ext/exceptions.hpp>

#include <utility>

namespace ext {

EXT_INLINE Exception::Exception(std::string message) noexcept
    : m_message(std::move(message))
{}

EXT_INLINE Exception::Exception(Exception& other) noexcept
{
    try
    {
        m_message = other.m_message;
    }
    catch (...)
    {
        m_message = "<broken>";
    }
}

EXT_INLINE auto Exception::message() const noexcept -> const std::string&
{
    return m_message;
}

EXT_INLINE auto Exception::what() const noexcept -> const char*
{
    return m_message.c_str();
}

} // namespace ext

#endif // EXT_EXCEPTIONS_IPP
