#pragma once

namespace funcpp::typeclass::monoid {

template <class T, typename, typename = void> struct
monoid_class;

template <
	typename T,
	typename Op = std::plus<>,
    typename = std::enable_if_t<monoid_class<T, Op>::value>
>
auto
mempty() {
	return monoid_class<T, Op>::mempty();
}

template <
	typename T,
	typename Op = std::plus<>,
    typename = std::enable_if_t<monoid_class<T, Op>::value>
>
auto
mappend(T const& a, T const& b) {
	return monoid_class<T, Op>::mappend(a,b);
}

template <
	typename T,
    typename Op = std::plus<>,
    typename = std::enable_if_t<monoid_class<T, Op>::value>
>
auto
operator + (T const& a, T const& b) {
	return monoid_class<T, Op>::mappend(a,b);
}

template <
	typename T,
    typename Op = std::multiplies<>,
    typename = std::enable_if_t<monoid_class<T, Op>::value>
>
auto
operator * (T const& a, T const& b) {
	return monoid_class<T, Op>::mappend(a,b);
}


}