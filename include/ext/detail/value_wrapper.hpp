/**
 * @file    value_wrapper.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    20.01.2024
 */

#pragma once

#include <type_traits>

namespace ext
{

template<typename T, typename Tag, template<typename> typename... Extensions>
class value_wrapper;

namespace detail
{

template<typename T>
struct is_value_wrapper : std::false_type {};

template<typename T, typename Tag, template<typename> typename... Extensions>
struct is_value_wrapper<value_wrapper<T, Tag, Extensions...>> : std::true_type {};

} // namespace detail
} // namespace ext
