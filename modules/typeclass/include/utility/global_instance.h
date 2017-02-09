
#pragma once

namespace funcpp::utility {

template <typename Fn> struct
global_instance {
	static constexpr Fn value {};
};

template <typename Fn>
constexpr Fn global_instance<Fn>::value;


}