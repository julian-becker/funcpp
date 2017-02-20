#pragma once
#include <typeclass/functor.h>
#include <future>
#include <type_traits>

namespace funcpp::typeclass::functor {

template <> struct
functor_class<std::future> : std::true_type {
    template <typename Fun, typename A>
    static auto fmap(Fun&& f, std::future<A> l) {
    	return std::async([f=std::forward<Fun>(f), l=std::move(l)]() mutable {
            return f(l.get());
        });
    }
};

}