#pragma once
#include <utility/global_instance.h>
//#include <typeclass/eq_list.h>
//#include <typeclass/eq_primitive.h>


namespace funcpp::typeclass::eq {

struct 
__eq {

	template <
		typename T,
		typename U,
		typename = decltype(std::declval<T>() == std::declval<U>())
	>
	bool
	constexpr operator() (T&& t, U&& u) const {
		using namespace detail;
		return equal(std::forward<T>(t), std::forward<U>(u));
	}

};

template <
	typename T,
	typename U,
	typename = decltype(std::declval<T>() == std::declval<U>())
>
bool
operator == (T&& a, U&& b) {
	return equal(std::forward<T>(a),std::forward<U>(b));
}


template <
	typename T,
	typename U,
	typename = decltype(std::declval<T>() != std::declval<U>())
>
bool 
operator != (T&& a, U&& b) {
	return !equal(std::forward<T>(a),std::forward<U>(b));
}

namespace {

	constexpr auto const& equal = funcpp::utility::global_instance<__eq>::value;
}

}