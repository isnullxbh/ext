/**
 * @file    tuple.hpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    23.06.2021
 */

#ifndef EXT_TUPLE_HPP
#define EXT_TUPLE_HPP

#include <type_traits>
#include <tuple>

namespace ext {

/**
 * @brief   Applies the given function to the each element of the tuple.
 *
 * @tparam  Tuple  Tuple type.
 * @tparam  F      Function type.
 * @param   tuple  Tuple.
 * @param   f      Function.
 *
 * @since   0.1.4
 */

template<typename Tuple, typename F>
constexpr auto foreach(Tuple& tuple, F&& f) -> void;

/**
 * @brief   Find an element that matches the specified predicate.
 *
 * @tparam  Tuple      Tuple type.
 * @tparam  Predicate  Predicate type.
 * @param   tuple      Tuple.
 * @param   predicate  Predicate.
 *
 * @return  If element was found (true, index), otherwise - (false, 0).
 *
 * @since   0.1.4
 */

template<typename Tuple, typename Predicate>
constexpr auto findIf(Tuple& tuple, const Predicate& predicate) -> std::pair<bool, std::size_t>;

/**
 * @brief   Transforms the tuple using the specified function.
 *
 * @tparam  F      Function type.
 * @tparam  Ts     Tuple element types.
 * @param   tuple  Tuple.
 * @param   f      Function.
 *
 * @return  A new tuple.
 *
 * @since   0.1.4
 */

template<typename F, typename... Ts>
constexpr auto transform(const std::tuple<Ts...>& tuple, F&& f) -> decltype(auto);

/**
 * @brief   Checks if all elements of a tuple matches the specified predicate.
 *
 * @tparam  Predicate  Predicate type.
 * @tparam  Ts         Tuple element types.
 * @param   tuple      A reference to a tuple.
 * @param   predicate  Predicate.
 *
 * @return  If all elements match the specified predicate - true, otherwise - false.
 *
 * @since   0.1.4
 */

template<typename Predicate, typename... Ts>
constexpr auto allOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool;

/**
 * @brief   Checks if any element of a tuple matches the specified predicate.
 *
 * @tparam  Predicate  Predicate type.
 * @tparam  Ts         Tuple element types.
 * @param   tuple      A reference to tuple.
 * @param   predicate  Predicate.
 *
 * @return  If any element matches the specified predicate - true, otherwise - false.
 *
 * @since   0.1.4
 */

template<typename Predicate, typename... Ts>
constexpr auto anyOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool;

/**
 * @brief   Checks if no elements of a tuple that match the specified predicate.
 *
 * @tparam  Predicate  Predicate type.
 * @tparam  Ts         Tuple element size.
 * @param   tuple      A reference to tuple.
 * @param   predicate  Predicate.
 *
 * @return  If no elements that match the specified predicate - true, otherwise - false.
 *
 * @since   0.1.4
 */

template<typename Predicate, typename... Ts>
constexpr auto noneOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool;

template<typename Tuple, typename F>
constexpr auto foreach(Tuple& tuple, F&& f) -> void
{
    const auto seq = std::make_index_sequence<std::tuple_size_v<Tuple>>();
    []<std::size_t... I>(Tuple& tuple, F&& f, const std::index_sequence<I...>&)
        {
            (f(std::get<I>(tuple)), ...);
        }(tuple, std::forward<F>(f), seq);
}

template<typename Tuple, typename Predicate>
constexpr auto findIf(Tuple& tuple, const Predicate& predicate) -> std::pair<bool, std::size_t>
{
    std::pair<bool, std::size_t> res { false, 0 };
    std::size_t idx = 0;
    foreach(tuple, [&](const auto& value)
        {
            if (predicate(value))
            {
                res.first = true;
                res.second = idx;
            }
            ++idx;
        });

    return res;
}

template<typename F, typename... Ts>
constexpr auto transform(const std::tuple<Ts...>& tuple, F&& f) -> decltype(auto)
{
    using OutTuple = std::tuple<std::invoke_result_t<F, Ts>...>;
    OutTuple out {};

    const auto seq = std::index_sequence_for<Ts...>();
    [&out]<std::size_t... I>(const std::tuple<Ts...>& tuple, F&& f, const std::index_sequence<I...>&)
        {
            out = std::make_tuple(f(std::get<I>(tuple))...);
        }(tuple, std::forward<F>(f), seq);

    return out;
}

template<typename Predicate, typename... Ts>
constexpr auto allOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool
{
    const auto seq = std::index_sequence_for<Ts...>();
    return []<std::size_t... I>(const std::tuple<Ts...>& tuple, Predicate&& predicate, const std::index_sequence<I...>&)
        {
            return (predicate(std::get<I>(tuple)) && ...);
        }(tuple, std::forward<Predicate>(predicate), seq);
}

template<typename Predicate, typename... Ts>
constexpr auto anyOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool
{
    const auto seq = std::index_sequence_for<Ts...>();
    return []<std::size_t... I>(const std::tuple<Ts...>& tuple, Predicate&& predicate, const std::index_sequence<I...>&)
        {
            return (predicate(std::get<I>(tuple)) || ...);
        }(tuple, std::forward<Predicate>(predicate), seq);
}

template<typename Predicate, typename... Ts>
constexpr auto noneOf(const std::tuple<Ts...>& tuple, Predicate&& predicate) -> bool
{
    const auto seq = std::index_sequence_for<Ts...>();
    return []<std::size_t... I>(const std::tuple<Ts...>& tuple, Predicate&& predicate, const std::index_sequence<I...>&)
        {
            return (!predicate(std::get<I>(tuple)) && ...);
        }(tuple, std::forward<Predicate>(predicate), seq);
}

} // namespace ext

#endif // EXT_TUPLE_HPP
