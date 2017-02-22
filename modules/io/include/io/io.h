#pragma once

#include <memory>

namespace funcpp::io {


template <typename T> class
io {
protected:
    struct 
    Impl {
        virtual ~Impl() {}
        virtual T run_impl() = 0;
    };

    io(std::unique_ptr<Impl> impl) : m_impl{ std::move(impl) } {}

private:
    std::unique_ptr<Impl> m_impl;

public:
    T 
    run() { return m_impl->run_impl(); }
};

}