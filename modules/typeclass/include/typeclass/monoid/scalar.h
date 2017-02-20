#pragma once
#include <typeclass/monoid.h>
#include <type_traits>

namespace funcpp::typeclass::monoid {

	namespace detail {
		template <typename T> 
		struct monoid_class_scalar_plus : std::true_type {
		    static auto mempty() {
		    	return T{};
		    }
		    static auto mappend(T const& a, T const& b) {
		    	return a + b;
		    }
		};

		template <typename T> 
		struct monoid_class_scalar_times : std::true_type {
		    static auto mempty() {
		    	return T{1};
		    }
		    static auto mappend(T const& a, T const& b) {
		    	return a * b;
		    }
		};
	}

	template <typename T> struct
	monoid_class<T, std::plus<>, std::enable_if_t<std::is_scalar<T>::value>> 
		: detail::monoid_class_scalar_plus<T> {};

	template <typename T> struct
	monoid_class<T, std::multiplies<>, std::enable_if_t<std::is_scalar<T>::value>> 
		: detail::monoid_class_scalar_times<T> {};

}