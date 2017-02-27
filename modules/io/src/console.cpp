#include <io/console.h>
#include <basetypes/unit.h>
#include <io/io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>

using namespace funcpp;
using namespace funcpp::io;
using namespace funcpp::basetypes;

io<basetypes::unit_t> 
console::write(std::string value) const {
    using namespace funcpp::basetypes;
    return io_action<unit_t>([value=std::move(value)]() -> unit_t {
        std::cout << value;
        return {};
    });
}

io<std::string> console::read() const {
    return io_action<std::string>([]{
        std::string value;
        std::cin >> value;
        return value;
    });
}
