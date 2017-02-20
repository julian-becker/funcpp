#pragma once
#include <boost/optional.hpp>
#include <typeclass/eq.h>
#include <type_traits>
#include <vector>

namespace funcpp::typeclass::eq {

template <typename T> struct
eq_class<boost::optional<T>, std::enable_if_t<eq_class<T>::value>>  : std::true_type {
    static bool equal(boost::optional<T> const& a, boost::optional<T> const& b) {
    	if((!a && !b) || (!a && b) || (a && !b))
            return false;
    	
    	return eq_class<T>::equal(*a,*b);
    }
};

}