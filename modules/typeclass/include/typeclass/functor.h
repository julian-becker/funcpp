#pragma once
#include <typeclass/functor_list.h>
#include <typeclass/functor_optional.h>
#include <typeclass/functor_vector.h>
#include <utility/global_instance.h>
#include <type_traits>

namespace funcpp::typeclass::functor {

struct 
__fmap {

	template <
		template <typename...> class Container,
	    typename ValueType,
	    typename Fn
	>
	constexpr auto
	operator() (Fn&& fn, Container<ValueType> const& container) const -> Container<std::result_of_t<Fn(ValueType)>> {
		using namespace detail;
		return fmap(std::forward<Fn>(fn), container);
	}

};

namespace {

	constexpr auto const& fmap = funcpp::utility::global_instance<__fmap>::value;
}

}