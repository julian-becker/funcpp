#pragma once
#include <boost/optional.hpp>
#include <type_traits>

namespace funcpp::typeclass::functor::detail {

template <typename Fn, typename A>
auto fmap(Fn&& fn, const boost::optional<A>& container) {
	boost::optional<std::result_of_t<Fn(A)>> result;
	std::transform(container.begin(), container.end(), std::back_inserter(result), std::forward<Fn>(fn));
	return result;
}
    
}