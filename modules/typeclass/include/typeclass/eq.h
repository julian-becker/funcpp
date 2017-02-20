#pragma once
#include <type_traits>

namespace funcpp::typeclass::eq {

template <typename T, typename = void> struct
eq_class;

template <
	typename T,
	typename U,
	typename TD = std::decay_t<T>,
	typename = std::enable_if_t<eq_class<TD>::value>
>
bool
equal (T&& a, U&& b) {
	return eq_class<TD>::equal(std::forward<T>(a),std::forward<U>(b));
}

namespace operators {

	template <
		typename T,
		typename U,
		typename TD = std::decay_t<T>,
		typename = std::enable_if_t<eq_class<T>::value>
	>
	bool
	operator == (T&& a, U&& b) {
		return eq_class<TD>::equal(std::forward<T>(a),std::forward<U>(b));
	}


	template <
		typename T,
		typename U,
		typename TD = std::decay_t<T>,
		typename = std::enable_if_t<eq_class<T>::value>
	>
	bool 
	operator != (T&& a, U&& b) {
		return !eq_class<TD>::equal(std::forward<T>(a),std::forward<U>(b));
	}
}

}