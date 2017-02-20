#pragma once
#include <typeclass/functor.h>

namespace funcpp::typeclass::monad {

template <template <typename...> class M> struct
monad_class;


template <
	template <typename...> class M,
    typename ValueType,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>
>
M<std::decay_t<ValueType>> 
mreturn(ValueType&& value) {
	return monad_class<M>::mreturn(std::forward<ValueType>(value));
}

template <
	template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>
>
auto
mbind(M<A> const& ma, Fn&& fn) {
	return monad_class<M>::mbind(ma, std::forward<Fn>(fn));
}


template <
    template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>,
    typename = std::enable_if_t<!std::is_copy_constructible<M<A>>::value>
>
auto
mbind(M<A> ma, Fn&& fn) {
    return monad_class<M>::mbind(std::move(ma), std::forward<Fn>(fn));
}

template <
	template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>,
    typename = std::enable_if_t<std::is_copy_constructible<M<A>>::value>
>
auto
operator >>= (M<A> const& ma, Fn&& fn) {
  return monad_class<M>::mbind(ma, std::forward<Fn>(fn));
}

template <
    template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>,
    typename = std::enable_if_t<!std::is_copy_constructible<M<A>>::value>
>
auto
operator >>= (M<A> ma, Fn&& fn) {
  return monad_class<M>::mbind(std::move(ma), std::forward<Fn>(fn));
}


template <
	template <typename...> class M,
    typename A,
    typename B,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>,
    typename = std::enable_if_t<std::is_copy_constructible<M<A>>::value>,
    typename = std::enable_if_t<std::is_copy_constructible<M<B>>::value>
>
auto
operator >> (M<A> const& ma, M<B> const& mb) {
  return monad_class<M>::mbind(ma, [mb](auto&&){ return mb; });
}


template <
    template <typename...> class M,
    typename A,
    typename B,
    typename = std::enable_if_t<functor::functor_class<M>::value>,
    typename = std::enable_if_t<monad_class<M>::value>,
    typename = std::enable_if_t<!std::is_copy_constructible<M<A>>::value>,
    typename = std::enable_if_t<!std::is_copy_constructible<M<B>>::value>
>
auto
operator >> (M<A> ma, M<B> mb) {
    return monad_class<M>::mbind(std::move(ma), [mb=std::move(mb)](auto&&) mutable {
        return std::move(mb);
    });
}


}