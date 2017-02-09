#pragma once

#include <type_traits>
#include <list>

namespace funcpp::typeclass::eq::detail {

template <
	typename T
>
bool equal(std::list<T> const& a, std::list<T> const& b) {
	if(a.size() != b.size())
		return false;

	for(auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
		if(!equal(*itA, *itB))
			return false;
	
	return true;
}

}