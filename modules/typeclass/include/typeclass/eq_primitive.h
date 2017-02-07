#pragma once
#include <typeclass/eq.h>
#include <type_traits>

namespace funcpp::typeclass::eq {
namespace detail {

	template <typename T> 
	struct instance_base {
	    static bool equal(T const& a, T const& b) {
	    	return a == b;
	    }
	};

}

/// unsigned long
template <typename T> struct
is_instance<T, std::enable_if_t<std::is_scalar<T>::value>> : std::true_type {};

template <typename T> struct
instance<T, std::enable_if_t<std::is_scalar<T>::value>> : detail::instance_base<T> {};

}