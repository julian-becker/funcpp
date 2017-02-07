#pragma once
#include <typeclass/eq.h>
#include <type_traits>
#include <list>

namespace funcpp::typeclass::eq {

template <typename T> struct
is_instance<std::list<T>, std::enable_if_t<is_instance_v<T>>> : std::true_type {};

template <typename T> struct
instance<std::list<T>, std::enable_if_t<is_instance_v<T>>> {

    static bool equal(std::list<T> const& a, std::list<T> const& b) {
    	if(a.size() != b.size())
    		return false;

    	for(auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
    		if(!instance<T>::equal(*itA, *itB))
    			return false;
    	
    	return true;
    }
    
};

}