#pragma once

#include <type_traits>
#include <memory>

namespace funcpp::io {


template <typename T, bool = std::is_copy_constructible<T>::value> class
io;

template <typename T> class
io<T, true> {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual T 
        run_impl() const = 0;
    };

private:
    std::shared_ptr<Impl> m_impl;

public:
    io(std::shared_ptr<Impl> impl) 
        : m_impl{ std::move(impl) } 
    {}

    T 
    run() const { return m_impl->run_impl(); }
};


template <typename T> class
io<T, false> {
protected:
    struct 
    Impl {
        virtual 
        ~Impl() {}

        virtual T 
        run_impl() = 0;
    };

private:
    std::unique_ptr<Impl> m_impl;

public:
    io(std::unique_ptr<Impl> impl) 
        : m_impl{ std::move(impl) } 
    {}

    T 
    run() { return m_impl->run_impl(); }
};


template <typename T, bool = std::is_copy_constructible<T>::value> class
io_constant;

template <typename T> class
io_constant<T, true> final : public io<T> {
    struct 
    Impl final : io<T>::Impl {
        T m_value;

        T run_impl() const override {
            return m_value;
        }

        Impl(T value)
            : m_value{ std::move(value) }
        {}
    };

public:

    io_constant(T value)
        : io<T>(std::make_shared<Impl>(std::move(value)))
    {}
};

template <typename T> class
io_constant<T, false> final : public io<T> {
    struct 
    Impl final : io<T>::Impl {
        mutable T m_value;
        T run_impl() override {
            return std::move(m_value);
        }

        Impl(T value)
            : m_value{ std::move(value) }
        {}
    };
public:
    io_constant(T value)
        : io<T>(std::make_unique<Impl>(std::move(value)))
    {}
};

}