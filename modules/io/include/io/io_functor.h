#pragma once
#include <io/io.h>
#include <typeclass/functor.h>
#include <future>
#include <type_traits>

namespace funcpp::typeclass::functor {

template <> struct
functor_class<io::io> : std::true_type {
    template <typename Fun, typename A>
    static auto fmap(Fun&& f, io::io<A> l) {
        return io::io_action<A>([f=std::forward<Fun>(f), l=std::move(l)]() mutable {
            return f(l.get());
        });
    }
};

}