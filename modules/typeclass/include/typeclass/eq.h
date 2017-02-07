#pragma once

namespace funcpp::typeclass::eq {

template <class T, typename = void> struct
is_instance : std::false_type {};

template <class T> constexpr bool
is_instance_v = is_instance<T>::value;

template <class T,typename = void> struct
instance;

template <
	typename T,
    typename = std::enable_if_t<is_instance_v<T>>
>
bool
operator == (T const& a, T const& b) {
	return instance<T>::equal(a,b);
}


template <
	typename T,
    typename = std::enable_if_t<is_instance_v<T>>
>
bool 
operator != (T const& a, T const& b) {
	return !instance<T>::equal(a,b);
}

}