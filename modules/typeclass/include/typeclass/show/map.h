#pragma once

#include <typeclass/show.h>
#include <type_traits>
#include <map>
#include <sstream>

namespace funcpp::typeclass::show {

template <typename K, typename V> struct
show_class<std::map<K,V>, std::enable_if_t<show_class<K>::value && show_class<V>::value>>  : std::true_type {
    static std::string show(std::map<K,V> const& m) {
        std::ostringstream ss;
        ss << "{";
        bool first = true;
    	for(auto const& v : m) {
    		ss << (first? "" : ", ") << show_class<K>::show(v.first) << ": " << show_class<V>::show(v.second);
            first = false;
        }
        ss  << "}";

    	return ss.str();
    }
};

}