#pragma once

#include <typeclass/show.h>
#include <type_traits>
#include <string>

namespace funcpp::typeclass::show {

template <> struct
show_class<std::string>  : std::true_type {
    static std::string show(std::string const& s) {
        return "\"" + s + "\"";
    }
};

}