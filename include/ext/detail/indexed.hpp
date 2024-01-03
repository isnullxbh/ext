/**
 * @file    indexed.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    03.01.2024
 */

#pragma once

namespace ext::detail
{

template<auto Index>
struct indexed
{
    static constexpr auto index = Index;
};

} // namespace ext::detail
