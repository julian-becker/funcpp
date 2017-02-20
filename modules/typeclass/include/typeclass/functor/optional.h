#pragma once
#include <boost/optional.hpp>
#include <type_traits>

namespace funcpp::typeclass::functor {

template <> struct
functor_class<boost::optional> : std::true_type {

    template <typename Fn, typename A>
    static auto fmap(Fn&& fn, const boost::optional<A>& container) -> boost::optional<std::result_of_t<Fn(A)>> {
    	if(container)
            return boost::make_optional(fn(*container));
    	return {};
    }

};

}