#pragma once
#include <basetypes/unit.h>
#include <io/io.h>
#include <string>
#include <io/rw_accessible.h>
#include <io/key_value_store.h>
#include <vector>

namespace funcpp::io {

class console final {    
public:
    io<basetypes::unit_t> write(std::string value) const;
    
    io<std::string> read() const;
};

}