#pragma once

#include <typeclass/show.h>
#include <type_traits>
#include <list>
#include <sstream>

namespace funcpp::typeclass::show {

template <typename T> struct
show_class<std::list<T>, std::enable_if_t<show_class<T>::value>>  : std::true_type {
    static std::string show(std::list<T> const& l) {
        std::ostringstream ss;
        ss << "{";
        bool first = true;
    	for(auto const& v : l) {
    		ss << (first? "" : ", ") << show_class<T>::show(v);
            first = false;
        }
        ss  << "}";

    	return ss.str();
    }
};

}