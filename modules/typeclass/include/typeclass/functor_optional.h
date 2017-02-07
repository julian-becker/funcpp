#pragma once
#include <typeclass/functor.h>
#include <experimental/optional>
#include <type_traits>

namespace funcpp::typeclass::functor {

template <> struct
is_instance<std::experimental::optional> : std::true_type {};

template <> struct
instance<std::experimental::optional> {

    template <typename Fn, typename A>
    static auto fmap(Fn&& fn, const std::experimental::optional<A>& container) {
		std::experimental::optional<std::result_of_t<Fn(A)>> result;
		std::transform(container.begin(), container.end(), std::back_inserter(result), std::forward<Fn>(fn));
		return result;
    }
    
};

}