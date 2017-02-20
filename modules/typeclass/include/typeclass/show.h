#pragma once
#include <type_traits>
#include <ostream>

namespace funcpp::typeclass::show {

template <typename T, typename = void> struct
show_class;

template <
	typename T,
	typename = std::enable_if_t<show_class<std::decay_t<T>>::value>
>
std::string 
show (T&& a) {
	return show_class<std::decay_t<T>>::show(std::forward<T>(a));
}

template <
	typename T,
	typename = std::enable_if_t<show_class<std::decay_t<T>>::value>
>
std::ostream& operator << (std::ostream& ostr, T&& value) {
	ostr << show_class<std::decay_t<T>>::show(std::forward<T>(value));
	return ostr;
}

}