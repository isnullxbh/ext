/**
 * @file    exceptions.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.06.2021
 */

#ifndef EXT_EXCEPTIONS_HPP
#define EXT_EXCEPTIONS_HPP

#include <exception>
#include <string>

#define EXT_DEFINE_EXCEPTION(BASE, EXCEPTION) \
class EXCEPTION : public BASE \
{ \
public: \
    using BASE::BASE; \
};

namespace ext {

/**
 * @class   Exception
 * @brief   Serves as a base class for other classes representing project-defined exceptions.
 * @since   0.1.4
 */

class Exception
    : public std::exception
{
public:
    Exception() = default;
    Exception(Exception& other) noexcept;
    explicit Exception(std::string message) noexcept;

public:
    auto message() const noexcept -> const std::string&;

    /// @copydoc std::exception::what()
    auto what() const noexcept -> const char* override;

protected:
    std::string m_message {};
};

/**
 * @defgroup    Exceptions
 * @brief       Project-defined exceptions.
 * @{
 */

EXT_DEFINE_EXCEPTION(Exception, RuntimeException)
EXT_DEFINE_EXCEPTION(RuntimeException, NullPointerException)
EXT_DEFINE_EXCEPTION(RuntimeException, CastException)

/**
 * @}
 */

} // namespace ext

#endif // EXT_EXCEPTIONS_HPP

#ifdef EXT_HEADER_ONLY
#   include <ext/impl/exceptions.ipp>
#endif
