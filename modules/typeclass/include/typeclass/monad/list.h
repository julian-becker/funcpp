#pragma once
#include <typeclass/monad.h>
#include <typeclass/functor/list.h>
#include <list>
#include <type_traits>

namespace funcpp::typeclass::monad {

template <> struct
monad_class<std::list> : std::true_type {

	template <typename A>
	static std::list<std::decay_t<A>> mreturn(A&& value) {
		return { std::forward<A>(value) };
	}

    template <typename A, typename Fn>
    static auto mbind(std::list<A> const& l, Fn&& f) {
		using result_type = std::result_of_t<Fn(A)>;
		result_type result;
		for (A const& x : l) {
			auto fx = std::forward<Fn>(f)(x);
			std::copy(fx.begin(), fx.end(), std::back_inserter(result));
		}
		return result;
    }
    
};

}