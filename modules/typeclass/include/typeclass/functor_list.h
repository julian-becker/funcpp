#pragma once
#include <typeclass/functor.h>
#include <list>
#include <type_traits>

namespace funcpp::typeclass::functor {

template <> struct
is_instance<std::list> : std::true_type {};

template <> struct
instance<std::list> {

    template <typename Fun, typename A>
    static auto fmap(Fun&& f, const std::list<A>& l) {
		std::list<std::result_of_t<Fun(A)>> result;
		std::transform(l.begin(), l.end(), std::back_inserter(result), std::forward<Fun>(f));
		return result;
    }
    
};

}