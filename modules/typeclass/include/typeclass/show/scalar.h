#pragma once

#include <typeclass/show.h>
#include <type_traits>
#include <list>
#include <sstream>

namespace funcpp::typeclass::show {

template <typename T> struct
show_class<T, std::enable_if_t<std::is_scalar<T>::value>>  : std::true_type {
    static std::string show(T const& a) {
        std::ostringstream ss;
        ss << a;
    	return ss.str();
    }
};

}