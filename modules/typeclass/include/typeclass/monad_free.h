#pragma once
#include <typeclass/functor.h>
#include <typeclass/monad.h>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>
#include <list>
#include <type_traits>

namespace funcpp {

namespace free {

template <template <typename...> class F, typename A> struct 
free;

template <template <typename...> class F, typename A> struct 
mreturn_t {
  A a;
};

template <template <typename...> class F, typename A> struct 
mbind_t {
  F<free<F, A>> x;
};

template <template <typename...> class F, typename A> struct 
free {
  using value_type = A;
  using result_type = mreturn_t<F, A>;
  boost::variant<boost::recursive_wrapper<mreturn_t<F, A>>,
                 boost::recursive_wrapper<mbind_t<F, A>>>
      v;
};

template <template <typename> class F, typename A>
free<F, A> 
make_mreturn(A const& x) {
  return {mreturn_t<F, A>{x}};
}

template <typename FA>
FA 
make_mreturn(typename FA::value_type const& x) {
  return {typename FA::result_type{x}};
}

template <template <typename> class F, typename A>
free<F, A> 
make_bbind(F<free<F, A>> const& x) {
  return {mbind_t<F, A>{x}};
}
}
}
