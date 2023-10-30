#pragma once

#include <type_traits>

namespace ext
{
    template<typename... Ts>
    class type_list;
}

namespace ext::detail
{

template<typename T>
struct is_type_list_impl : std::false_type {};

template<typename... Ts>
struct is_type_list_impl<type_list<Ts...>> : std::true_type {};

class type_list_base
{
public:
    static constexpr auto npos = static_cast<std::size_t>(-1);
};

template<typename T>
struct pop_front_op;

template<typename T, typename... Ts>
struct pop_front_op<type_list<T, Ts...>>
{
    using type = type_list<Ts...>;
};

template<>
struct pop_front_op<type_list<>>
{
    // TODO: error?
    using type = type_list<>;
};

template<typename... Ts>
class type_list_ops : public type_list_base
{
public:
    template<typename I, template<typename...> typename F, typename... Us>
    struct foldl_op;

    template<typename I, template<typename...> typename F, typename U, typename... Us>
    struct foldl_op<I, F, U, Us...>
    {
        using type = foldl_op<typename F<I, U>::type, F, Us...>::type;
    };

    template<typename I, template<typename...> typename F>
    struct foldl_op<I, F>
    {
        using type = I;
    };

    template<typename I, template<typename...> typename F, typename... Us>
    struct foldr_op;

    template<typename I, template<typename...> typename F, typename... Us>
    struct foldr_op<I, F, type_list<Us...>>
    {
        using type = foldl_op<I, F, Us...>::type;
    };

    template<typename... Us>
    struct reverse_op
    {
        template<typename I, typename T>
        struct front_inserter
        {
            using type = I::template push_front<T>;
        };

        using type = foldl_op<type_list<>, front_inserter, Us...>::type;
    };

    template<typename... Us>
    struct push_back_op
    {
        using type = type_list<Ts..., Us...>;
    };

    template<typename... Us>
    struct push_front_op
    {
        using type = type_list<Us..., Ts...>;
    };

    template<template<typename> typename Predicate>
    struct filter_op
    {
        template<typename I, typename T>
        struct conditional_inserter
        {
            using type = std::conditional_t<Predicate<T>::value, typename I::template push_back<T>, I>;
        };

        using type = foldl_op<type_list<>, conditional_inserter, Ts...>::type;
    };

    template<template<typename> typename Predicate>
    struct remove_if_op
    {
        template<typename I, typename T>
        struct conditional_inserter
        {
            using type = std::conditional_t<!Predicate<T>::value, typename I::template push_back<T>, I>;
        };

        using type = foldl_op<type_list<>, conditional_inserter, Ts...>::type;
    };

    template<std::size_t N, typename... Us>
    struct get_op;

    template<std::size_t N, typename U, typename... Us>
    struct get_op<N, U, Us...>
    {
        static_assert(N < sizeof...(Ts));
        using type = get_op<N - 1, Us...>::type;
    };

    template<typename U, typename... Us>
    struct get_op<0, U, Us...>
    {
        using type = U;
    };

    template<std::size_t N, typename U>
    struct set_op
    {
        static_assert(N < sizeof...(Ts));

        template<typename L, std::size_t I>
        struct init
        {
            using list_type = L;
            static constexpr std::size_t index = I;
        };

        template<typename I, typename T>
        struct inserter
        {
            using type =
                init<std::conditional_t<I::index == N, typename I::list_type::template push_back<U>, typename I::list_type::template push_back<T>>,
                     I::index + 1>;
        };

        using type = foldl_op<init<type_list<>, 0>, inserter, Ts...>::type::list_type;
    };

    template<std::size_t from, std::size_t size>
    struct slice_op
    {
        static_assert((from + size) <= sizeof...(Ts));

        template<typename L, std::size_t I>
        struct init
        {
            using list_type = L;
            static constexpr std::size_t index = I;
        };

        template<typename I, typename T>
        struct inserter
        {
            using type =
                init<std::conditional_t<I::index >= from && I::index < (from + size),
                                        typename I::list_type::template push_back<T>, typename I::list_type>,
                     I::index + 1>;
        };

        using type = foldl_op<init<type_list<>, 0>, inserter, Ts...>::type::list_type;
    };

    template<std::size_t N>
    struct split_op
    {
        static_assert(N <= sizeof...(Ts));

        template<typename L, typename R, std::size_t I>
        struct init
        {
            using l_list_type = L;
            using r_list_type = R;
            static constexpr std::size_t index = I;
        };

        template<typename I, typename T>
        struct inserter
        {
            using type =
                init<std::conditional_t<I::index < N, typename I::l_list_type::template push_back<T>, typename I::l_list_type>,
                     std::conditional_t<I::index < N, typename I::r_list_type, typename I::r_list_type::template push_back<T>>,
                     I::index + 1>;
        };

        using r = foldl_op<init<type_list<>, type_list<>, 0>, inserter, Ts...>::type;

        using type = type_list<typename r::l_list_type, typename r::r_list_type>;
    };

    template<template<typename...> typename Mapper>
    struct map_op
    {
        template<typename I, typename U>
        struct inserter
        {
            using type = typename I::template push_back<typename Mapper<U>::type>;
        };

        using type = foldl_op<type_list<>, inserter, Ts...>::type;
    };

    template<typename U>
    struct contains_op
    {
        static constexpr auto value = (std::is_same_v<U, Ts> || ...);
    };

    template<typename... Us>
    struct concat_op;

    template<typename... Us>
    struct concat_op<type_list<Us...>>
    {
        using type = type_list<Ts..., Us...>;
    };

    template<template<typename> typename Predicate, std::size_t Pos>
    struct find_if_op
    {
        template<std::size_t I, std::size_t N>
        struct init
        {
            static constexpr auto index = I;
            static constexpr auto type_index = N;
        };

        template<typename I, typename T>
        struct checker
        {
            using type = init<I::index + 1, Predicate<T>::value && I::type_index == npos && I::index >= Pos ? I::index : I::type_index>;
        };

        static constexpr auto value = foldl_op<init<0, npos>, checker, Ts...>::type::type_index;
    };

    template<typename U>
    struct make_index_of_predicate
    {
        template<typename V>
        struct type : std::is_same<U, V>
        {};
    };

    template<typename U, std::size_t Pos>
    struct index_of_op : find_if_op<make_index_of_predicate<U>::template type, Pos> {};

    template<template<typename...> typename Comparator>
    struct sort_op
    {
        template<typename L, int I, typename Ref>
        struct step
        {
            using Cur = L::template get<I>;

            using type =
                std::conditional_t<!Comparator<Cur, Ref>::value,
                    typename step<typename L::template set<I + 1, Cur>, I - 1, Ref>::type,
                    typename L::template set<I + 1, Ref>>;
        };

        template<typename L, typename Ref>
        struct step<L, -1, Ref>
        {
            using type = L::template set<0, Ref>;
        };

        template<typename L, std::size_t I>
        struct type_iterator
        {
            using type = type_iterator<typename step<L, I - 1, typename L::template get<I>>::type, I + 1>::type;
        };

        template<typename L>
        struct type_iterator<L, sizeof...(Ts)>
        {
            using type = L;
        };

        template<typename I, typename P = void>
        struct switcher;

        template<typename... Us>
        struct switcher<type_list<Us...>, std::enable_if_t<(sizeof...(Us) > 1)>>
        {
            using type = typename type_iterator<type_list<Us...>, 1>::type;
        };

        template<typename... Us>
        struct switcher<type_list<Us...>, std::enable_if_t<(sizeof...(Us) <= 1)>>
        {
            using type = type_list<Us...>;
        };

        using type = typename switcher<type_list<Ts...>>::type;
    };
};

} // namespace ext::detail
