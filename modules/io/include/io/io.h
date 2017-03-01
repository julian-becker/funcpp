#pragma once

#include <type_traits>
#include <memory>
#include <functional>

namespace funcpp::io {

template <typename T, typename = void> class
io_constant;

template <typename T, typename = void> class
io;

template <typename T> class
io<T, std::enable_if_t<std::is_copy_constructible<T>::value>> {
public:
    using value_type = T;
private:
    template <typename, typename> friend class io;

    template <typename U>
    io<U> 
    then_internal(std::function<io<U>(T)> fn) const {
        struct 
        ThenImpl : io<U>::Impl {
            std::shared_ptr<Impl> m_impl;
            std::function<io<U>(T)> m_cont;

            virtual U
            run_impl() const override {
                return m_cont(m_impl->run_impl()).m_impl->run_impl();
            }

            ThenImpl(std::shared_ptr<Impl> impl, std::function<io<U>(T)> cont)
                : m_impl{ std::move(impl) }
                , m_cont{ std::move(cont) }
            {}
        };

        return io<U>(std::make_shared<ThenImpl>(m_impl, std::move(fn)));
    }


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

    template
        < typename U
        , typename = std::enable_if_t<std::is_convertible<T,U>::value>
        >
    operator io<U> () const {
        return then([this](auto&& val_T) -> io<U> {
            return io_constant<U>(static_cast<U>(val_T));
        });
    }


    template 
        < typename Fn
        , typename result_t = std::result_of_t<Fn(T)> 
    >
    auto
    then(Fn&& fn) const -> io<typename result_t::value_type> {
        return then_internal(std::function<io<typename result_t::value_type>(T)>(std::forward<Fn>(fn)));
    }

    T 
    run() const { return m_impl->run_impl(); }
};


template <typename T> class
io<T, std::enable_if_t<!std::is_copy_constructible<T>::value>> {
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



template <typename T> class
io_constant<T, std::enable_if_t<std::is_copy_constructible<T>::value>> final : public io<T> {
    struct 
    Impl final : io<T>::Impl {
        T m_value;

        T 
        run_impl() const override {
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
io_constant<T, std::enable_if_t<!std::is_copy_constructible<T>::value>> final : public io<T> {
    struct 
    Impl final : io<T>::Impl {
        mutable T m_value;
        
        T 
        run_impl() override {
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



template <typename T> class
io_action final : public io<T> {
    struct 
    Impl final : io<T>::Impl {
        std::function<T()> m_action;

        T 
        run_impl() const override {
            return m_action();
        }

        template <typename Fn>
        Impl(Fn&& action)
            : m_action{ std::forward<Fn>(action) }
        {}
    };

public:

    template <typename Fn>
    io_action(Fn&& action)
        : io<T>(std::make_shared<Impl>(std::forward<Fn>(action)))
    {}
};

}