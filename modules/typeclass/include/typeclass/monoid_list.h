#pragma once
#include <typeclass/monoid.h>
#include <type_traits>
#include <list>

namespace funcpp::typeclass::monoid {

template <typename T> struct
mappend_add_op<std::list<T>> {
	using type = std::plus<void>;
};

template <typename T> struct
mappend_multiplies_op<std::list<T>> {
	// multiplication not defined
};

template <typename T> struct 
is_instance<std::list<T>, std::plus<void>> : std::true_type {};

template <typename T> struct 
instance<std::list<T>, std::plus<void>> {
    
    static std::list<T> mempty() {
    	return {};
    }

    static auto mappend(std::list<T> const& a, std::list<T> const& b) {
    	std::list<T> result(a.begin(), a.end());
    	std::copy(b.begin(),b.end(), std::back_inserter(result));
    	return result;
    }

};

}