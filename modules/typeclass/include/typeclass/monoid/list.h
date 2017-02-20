#pragma once
#include <typeclass/monoid.h>
#include <type_traits>
#include <list>

namespace funcpp::typeclass::monoid {

template <typename T> struct 
monoid_class<std::list<T>, std::plus<void>> : std::true_type {
    
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