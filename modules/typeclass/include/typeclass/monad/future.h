#pragma once
#include <typeclass/monad.h>
#include <typeclass/functor/future.h>
#include <future>
#include <type_traits>

namespace funcpp::typeclass::monad {

template <> struct
monad_class<std::future> : std::true_type {

	template <typename A>
	static std::future<std::decay_t<A>> mreturn(A&& value) {
        std::promise<std::decay_t<A>> p;
        p.set_value(std::forward<A>(value));
		return p.get_future();
	}

    template <typename A, typename Fn>
    static auto mbind(std::future<A> x, Fn&& f) {
		return std::async([x = std::move(x), f=std::forward<Fn>(f)]() mutable {
            return f(x.get()).get();
        });
    }
    
};

}