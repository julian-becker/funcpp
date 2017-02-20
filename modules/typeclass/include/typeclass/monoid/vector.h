#pragma once
#include <typeclass/monoid.h>
#include <type_traits>
#include <vector>

namespace funcpp::typeclass::monoid {

template <typename T> struct 
monoid_class<std::vector<T>, std::plus<void>> : std::true_type {
    
    static std::vector<T> mempty() {
    	return {};
    }

    static auto mappend(std::vector<T> const& a, std::vector<T> const& b) {
    	std::vector<T> result(a.begin(), a.end());
    	std::copy(b.begin(),b.end(), std::back_inserter(result));
    	return result;
    }

};

}