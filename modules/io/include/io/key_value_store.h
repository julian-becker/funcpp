#pragma once
#include <io/io.h>
#include <io/rw_accessible.h>
#include <memory>

namespace funcpp::io {

template 
    < typename Key
    , typename Value
> class
key_value_store {
protected:
    struct Impl {
        virtual 
        ~Impl() {}
        
        virtual io<rw_accessible<Value>> 
        access_impl(Key) const = 0;
    };

private:
    std::shared_ptr<Impl> m_impl;

public:
    key_value_store(std::shared_ptr<Impl> impl)
        : m_impl{ std::move(impl) }
    {}

    io<rw_accessible<Value>> 
    access(Key const& key) const {
        return m_impl->access_impl(key);
    }
};


}