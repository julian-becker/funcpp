#pragma once
#include <type_traits>



namespace funcpp::typeclass::functor {

template <template <typename...> class F> struct
functor_class;

template <
	template <typename...> class Container,
    typename ValueType,
    typename Fn,
    typename = std::enable_if_t<functor_class<Container>::value>
>
auto
fmap(Fn&& fn, Container<ValueType> const& container) {
	return functor_class<Container>::fmap(std::forward<Fn>(fn), container);
}

}