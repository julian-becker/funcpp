#pragma once

namespace funcpp::typeclass::monoid {

template <class T, typename Op = std::plus<T>, typename = void> struct
is_instance : std::false_type {};

template <class T, typename Op = std::plus<T>> constexpr bool
is_instance_v = is_instance<T, Op>::value;

template <class T, typename, typename = void> struct
instance;

template <
	typename T,
	typename Op = std::plus<T>,
    typename = std::enable_if_t<is_instance_v<T>>
>
auto
mempty() {
	return instance<T, Op>::mempty();
}

template <
	typename T,
	typename Op = std::plus<T>,
    typename = std::enable_if_t<is_instance_v<T>>
>
auto
mappend(T const& a, T const& b) {
	return instance<T, Op>::mappend(a,b);
}

}