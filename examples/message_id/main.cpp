/**
 * @file    main.cpp
 * @author  Oleg E. Vorobiov <isnullxbh(at)gmail.com>
 * @date    20.01.2024
 */

#include <ext/value_wrapper.hpp>

namespace detail
{
    struct message_id_tag {};
}

template<typename T>
class add_increment
{};

template<typename T, typename Tag, template<typename> typename... Extensions>
class add_increment<ext::value_wrapper<T, Tag, Extensions...>>
{
public:
    friend constexpr auto operator++(ext::value_wrapper<T, Tag, Extensions...>& lhs) -> ext::value_wrapper<T, Tag, Extensions...>&
    {
        lhs.value() += 1;
        return lhs;
    }

    friend constexpr auto operator++(ext::value_wrapper<T, Tag, Extensions...>& lhs, int) -> ext::value_wrapper<T, Tag, Extensions...>
    {
        ext::value_wrapper<T, Tag, Extensions...> tmp { lhs };
        lhs.value() += 1;
        return tmp;
    }
};

template<typename T>
class message_id_ops
{
public:
    constexpr auto invalid() const noexcept
    {
        return static_cast<const T*>(this)->value() == 0;
    }
};

// Represents the sequence number of the message.
// Supported operations:
//   - comparison
//   - increment, decrement
using message_id = ext::value_wrapper<std::size_t, detail::message_id_tag, add_increment, message_id_ops>;

int main()
{
    constexpr message_id id1 { 10 };
    constexpr message_id id2 { 11 };
    constexpr message_id id3 { 0 };
    static_assert(id1 != id2);
    static_assert(id3.invalid() && !id1.invalid());

    constexpr auto r1 = []{ message_id id { 1 }; return (++id).value(); }();
    static_assert(r1 == 2);

    constexpr auto r2 = []() -> message_id { message_id id { 1 }; return id++; }();
    static_assert(r2.value() == 1);

    return 0;
}
