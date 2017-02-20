#pragma once
#include <type_traits>



namespace funcpp::typeclass::functor {

template <template <typename...> class F> struct
functor_class;

template <
	template <typename...> class Container,
    typename ValueType,
    typename Fn,
    typename = std::enable_if_t<functor_class<Container>::value>,
    typename = std::enable_if_t<std::is_copy_constructible<Container<ValueType>>::value>
>
auto
fmap(Fn&& fn, Container<ValueType> const& container) {
	return functor_class<Container>::fmap(std::forward<Fn>(fn), container);
}

template <
    template <typename...> class Container,
    typename ValueType,
    typename Fn,
    typename = std::enable_if_t<functor_class<Container>::value>,
    typename = std::enable_if_t<!std::is_copy_constructible<Container<ValueType>>::value>,
    typename = std::enable_if_t<std::is_move_constructible<Container<ValueType>>::value>
>
auto
fmap(Fn&& fn, Container<ValueType> container) {
    return functor_class<Container>::fmap(std::forward<Fn>(fn), std::move(container));
}

}