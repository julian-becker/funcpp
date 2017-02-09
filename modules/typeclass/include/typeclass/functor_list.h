#pragma once
#include <list>
#include <type_traits>

namespace funcpp::typeclass::functor::detail {

template <typename Fun, typename A>
auto fmap(Fun&& f, const std::list<A>& l) {
	std::list<std::result_of_t<Fun(A)>> result;
	std::transform(l.begin(), l.end(), std::back_inserter(result), std::forward<Fun>(f));
	return result;
}

}