#pragma once

#include <typeclass/eq.h>
#include <type_traits>
#include <vector>

namespace funcpp::typeclass::eq {

template <typename T> struct
eq_class<std::vector<T>, std::enable_if_t<eq_class<T>::value>>  : std::true_type {
    static bool equal(std::vector<T> const& a, std::vector<T> const& b) {
    	if(a.size() != b.size())
    		return false;

    	for(auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
    		if(!eq_class<T>::equal(*itA, *itB))
    			return false;
    	
    	return true;
    }
};

}