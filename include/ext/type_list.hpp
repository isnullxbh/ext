#pragma once

#include <ext/detail/type_list_ops.hpp>

namespace ext
{

/// @defgroup ext-metaprogramming-typelist Type list
/// @ingroup  ext-metaprogramming

/// Checks if the specified type @a T is a type list.
/// @tparam  T Type to check.
/// @ingroup ext-metaprogramming-typelist
/// @since   0.1.0
template<typename T>
struct is_type_list : detail::is_type_list_impl<T> {};

/// A list of types.
/// @tparam  Ts Types.
/// @ingroup ext-metaprogramming-typelist
/// @since   0.1.0
template<typename... Ts>
class type_list : private detail::type_list_ops<Ts...>
{
private:
    using base = detail::type_list_ops<Ts...>;

public:
    /// Reverse types in list.
    using reverse = base::template reverse_op<Ts...>::type;

    /// Checks if the type list contains the specified type @a U.
    /// @tparam U Type to check.
    template<typename U>
    static constexpr auto contains = base::template contains_op<U>::value;

    /// Gets an index of the specified type @a U in the list.
    /// @tparam U   Type to search.
    /// @tparam Pos The position in the list at which the search begins.
    template<typename U, std::size_t Pos = 0>
    static constexpr auto index_of = base::template index_of_op<U, Pos>::value;

    /// Gets an index of the type that meets the specified predicate.
    /// @tparam Predicate Predicate.
    /// @tparam Pos       The position in the list at which the search begins.
    template<template<typename> typename Predicate, std::size_t Pos = 0>
    static constexpr auto find_if = base::template find_if_op<Predicate, Pos>::value;

    /// Gets the N-th element from the type list.
    /// @tparam N Type index.
    template<std::size_t N>
    using get = base::template get_op<N, Ts...>::type;

    /// Sets the element at the @a N position to @a U.
    /// @tparam N Position in list.
    /// @tparam U Type to set.
    template<std::size_t N, typename U>
    using set = base::template set_op<N, U>::type;

    /// Removes the last element.
    using pop_back = detail::pop_front_op<type_list::reverse>::type::reverse;

    /// Removes the first element.
    using pop_front = detail::pop_front_op<type_list>::type;

    /// Adds the Us types to the end.
    /// @tparam Us Types.
    template<typename... Us>
    using push_back = base::template push_back_op<Us...>::type;

    /// Adds the Us types to the beginning.
    /// @tparam Us Types.
    template<typename... Us>
    using push_front = base::template push_front_op<Us...>::type;

    /// Adds types from the specified type list @a L to the end.
    /// @tparam L Type list.
    template<typename L>
    using concat = base::template concat_op<L>::type;

    /// Left-associative fold.
    /// @tparam I Accumulator.
    /// @tparam F Metafunction.
    template<typename I, template<typename...> typename F>
    using foldl = base::template foldl_op<I, F, Ts...>::type;

    /// Right-associative fold.
    /// @tparam I Accumulator.
    /// @tparam F Metafunction.
    template<typename I, template<typename...> typename F>
    using foldr = base::template foldr_op<I, F, typename base::template reverse_op<Ts...>::type>::type;

    /// Filters type list elements.
    /// @tparam Predicate Predicate type.
    template<template<typename> typename Predicate>
    using filter = base::template filter_op<Predicate>::type;

    /// Applies the specified metafunction @a Mapper to each list's element.
    /// @tparam Mapper Metafunction.
    template<template<typename...> typename Mapper>
    using map = base::template map_op<Mapper>::type;

    /// Removes elements that meets the specified predicate @a Predicate.
    /// @tparam Predicate Predicate type.
    template<template<typename> typename Predicate>
    using remove_if = base::template remove_if_op<Predicate>::type;

    /// Gets a new type list that contains @a Count types starts from the @a From position.
    /// @tparam From  The position in the list at which the slicing begins.
    /// @tparam Count The number of types to slice.
    template<std::size_t From, std::size_t Count = sizeof...(Ts) - From>
    using slice = base::template slice_op<From, Count>::type;

    /// Splits the type list on two lists.
    /// @tparam N The position at which the second list begins.
    template<std::size_t N>
    using split = base::template split_op<N>::type;

    /// Sorts the type list.
    /// @tparam Comparator Comparator type.
    template<template<typename...> typename Comparator>
    using sort = base::template sort_op<Comparator>::type;

    /// Checks if type lists are equal.
    /// @tparam U Type list to compare.
    template<typename U>
    static constexpr auto is_equal = std::is_same_v<U, type_list<Ts...>>;

    /// The number of elements in the type list.
    static constexpr auto size = sizeof...(Ts);

    /// Indicates that type lists is empty.
    static constexpr auto empty = (size == 0);

    /// Integer value that represent the invalid index.
    using base::npos;
};

} // namespace ext
