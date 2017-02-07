#pragma once

namespace funcpp::typeclass::monoid {

template <typename T> struct
mappend_add_op {
	using type = std::plus<T>;
};

template <typename T> using
mappend_add_op_t = typename mappend_add_op<T>::type;

template <typename T> struct
mappend_multiplies_op {
	using type = std::multiplies<T>;
};

template <typename T> using
mappend_multiplies_op_t = typename mappend_multiplies_op<T>::type;



template <class T, typename Op = mappend_add_op<T>, typename = void> struct
is_instance : std::false_type {};

template <class T, typename Op = mappend_add_op_t<T>> constexpr bool
is_instance_v = is_instance<T, Op>::value;

template <class T, typename, typename = void> struct
instance;

template <
	typename T,
	typename Op = mappend_add_op_t<T>,
    typename = std::enable_if_t<is_instance_v<T,Op>>
>
auto
mempty() {
	return instance<T, Op>::mempty();
}

template <
	typename T,
	typename Op = mappend_add_op_t<T>,
    typename = std::enable_if_t<is_instance_v<T,Op>>
>
auto
mappend(T const& a, T const& b) {
	return instance<T, Op>::mappend(a,b);
}

template <
	typename T,
    typename = std::enable_if_t<is_instance_v<T,mappend_add_op_t<T>>>
>
auto
operator + (T const& a, T const& b) {
	return instance<T, mappend_add_op_t<T>>::mappend(a,b);
}

template <
	typename T,
    typename = std::enable_if_t<is_instance_v<T,mappend_multiplies_op_t<T>>>
>
auto
operator * (T const& a, T const& b) {
	return instance<T, mappend_multiplies_op_t<T>>::mappend(a,b);
}


}