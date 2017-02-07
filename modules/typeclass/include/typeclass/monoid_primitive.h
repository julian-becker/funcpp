#pragma once
#include <typeclass/monoid.h>
#include <type_traits>

namespace funcpp::typeclass::monoid {
namespace detail {

	template <typename T> 
	struct instance_base_plus {
	    static auto mempty() {
	    	return T{};
	    }
	    static auto mappend(T const& a, T const& b) {
	    	return a + b;
	    }
	};

	template <typename T> 
	struct instance_base_times {
	    static auto mempty() {
	    	return T{1};
	    }
	    static auto mappend(T const& a, T const& b) {
	    	return a * b;
	    }
	};

}

template <typename T> struct
is_instance<T, std::plus<T>, std::enable_if_t<std::is_scalar<T>::value>> : std::true_type {};

template <typename T> struct
instance<T, std::plus<T>, std::enable_if_t<std::is_scalar<T>::value>> : detail::instance_base_plus<T> {};


template <typename T> struct
is_instance<T, std::multiplies<T>, std::enable_if_t<std::is_scalar<T>::value>> : std::true_type {};

template <typename T> struct
instance<T, std::multiplies<T>, std::enable_if_t<std::is_scalar<T>::value>> : detail::instance_base_times<T> {};

}