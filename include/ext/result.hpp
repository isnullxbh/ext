/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    22.02.2024
 */

#pragma once

#include <ext/detail/result/result_storage.hpp>

namespace ext
{

template<typename T, typename E>
class result : private detail::result_storage<T, E>
{
public:
    result() = default;

    ~result() = default;
};

} // namespace ext
