#pragma once
#include <type_traits>

namespace funcpp::typeclass::eq::detail {

template <
	typename T,
	typename = std::enable_if_t<std::is_scalar<T>::value>
>
bool 
equal(T const& a, T const& b) {
	return a == b;
}

}