#pragma once
#include <type_traits>

namespace funcpp::typeclass::eq {

template <typename T>
struct eq_class<T, std::enable_if_t<std::is_scalar<T>::value>> : std::true_type {
    static bool 
    equal(T const& a, T const& b) {
    	return a == b;
    }
};

}