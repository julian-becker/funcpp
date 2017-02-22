#pragma once

#include <io/io.h>

namespace funcpp::io {


template <typename T> class
write {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual T 
        write_impl() const = 0;
    };

    write(std::shared_ptr<Impl> impl) : m_impl{ std::move(impl) } {}

private:
    std::shared_ptr<Impl> m_impl;

public:
    io<T>
    write() const { return m_impl->write_impl(); }
};


}