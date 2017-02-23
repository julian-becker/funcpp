#pragma once

#include <io/io.h>
#include <basetypes/unit.h>

namespace funcpp::io {

template <typename T> class
writable {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual io<basetypes::unit_t>
        write_impl(T value) const = 0;
    };

private:
    std::shared_ptr<Impl> m_impl;

public:
    writable(std::shared_ptr<Impl> impl) : m_impl{ std::move(impl) } {}

    io<basetypes::unit_t>
    write(T value) const { return m_impl->write_impl(std::move(value)); }
};


}