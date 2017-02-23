#pragma once

#include <io/io.h>
#include <basetypes/unit.h>

namespace funcpp::io {

template <typename T> class
readable {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual io<T>
        read_impl() const = 0;
    };

private:
    std::shared_ptr<Impl> m_impl;

public:
    readable(std::shared_ptr<Impl> impl) : m_impl{ std::move(impl) } {}

    io<T>
    read() const { return m_impl->read_impl(); }
};


}