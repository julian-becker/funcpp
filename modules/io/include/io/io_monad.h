#pragma once
#include <io/io.h>
#include <io/io_functor.h>
#include <typeclass/monad.h>
#include <future>
#include <type_traits>

namespace funcpp::typeclass::monad {

template <> struct
monad_class<io::io> : std::true_type {

	template <typename A>
	static io::io<A> mreturn(A&& value) {
        return io::io_constant<std::decay_t<A>>(std::forward<A>(value));
	}

    template <typename A, typename Fn>
    static auto mbind(io::io<A> x, Fn&& f) {
		return x.then(std::forward<Fn>(f));
    }
    
};

}