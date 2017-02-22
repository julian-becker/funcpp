#pragma once

#include <memory>

namespace funcpp::io {


template <typename T> class
io {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual T 
        run_impl() const = 0;
    };

    io(std::shared_ptr<Impl> impl) : m_impl{ std::move(impl) } {}

private:
    std::shared_ptr<Impl> m_impl;

public:
    T 
    run() const { return m_impl->run_impl(); }
};

}