#pragma once

namespace funcpp::typeclass::monad {

template <template <typename...> class M> struct
is_instance : std::false_type {};


template <template <typename...> class M> constexpr bool
is_instance_v = is_instance<M>::value;

template <template <typename...> class M> struct
instance;

template <
	template <typename...> class M,
    typename ValueType,
    typename = std::enable_if_t<is_instance_v<M>>
>
M<std::decay_t<ValueType>> 
mreturn(ValueType&& value) {
	return instance<M>::mreturn(std::forward<ValueType>(value));
}

template <
	template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<is_instance_v<M>>
>
M<std::result_of_t<Fn(A)>> 
mbind(M<A> const& ma, Fn&& fn) {
	return instance<M>::mbind(ma, std::forward<Fn>(fn));
}

template <
	template <typename...> class M,
    typename A,
    typename Fn,
    typename = std::enable_if_t<is_instance_v<M>>
>
auto
operator >>= (M<A> const& ma, Fn&& fn) {
  return instance<M>::mbind(ma, std::forward<Fn>(fn));
}


template <
	template <typename...> class M,
    typename A,
    typename B,
    typename = std::enable_if_t<is_instance_v<M>>
>
auto
operator >> (M<A> const& ma, M<B> const& mb) {
  return instance<M>::mbind(ma, [mb](auto&&){ return mb; });
}



}