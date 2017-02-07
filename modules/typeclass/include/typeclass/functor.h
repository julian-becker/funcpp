#pragma once

namespace funcpp::typeclass::functor {

template <template <typename...> class F> struct
is_instance : std::false_type {};


template <template <typename...> class F> constexpr bool
is_instance_v = is_instance<F>::value;

template <template <typename...> class F> struct
instance;

template <
	template <typename...> class Container,
    typename ValueType,
    typename Fn,
    typename = std::enable_if_t<is_instance_v<Container>>
>
Container<std::result_of_t<Fn(ValueType)>> 
fmap(Fn&& fn, const Container<ValueType>& container) {
	return instance<Container>::fmap(std::forward<Fn>(fn), container);
}

}