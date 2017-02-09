#pragma once
#include <vector>
#include <type_traits>

namespace funcpp::typeclass::functor::detail {

template <typename Fn, typename A>
auto fmap(Fn&& fn, const std::vector<A>& container) {
	std::vector<std::result_of_t<Fn(A)>> result;
	std::transform(container.begin(), container.end(), std::back_inserter(result), std::forward<Fn>(fn));
	return result;
}


}