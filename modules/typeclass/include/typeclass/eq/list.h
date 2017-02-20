#pragma once

#include <typeclass/eq.h>
#include <type_traits>
#include <list>

namespace funcpp::typeclass::eq {

template <typename T> struct
eq_class<std::list<T>, std::enable_if_t<eq_class<T>::value>>  : std::true_type {
    static bool equal(std::list<T> const& a, std::list<T> const& b) {
    	if(a.size() != b.size())
    		return false;

    	for(auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
    		if(!eq_class<T>::equal(*itA, *itB))
    			return false;
    	
    	return true;
    }
};

}