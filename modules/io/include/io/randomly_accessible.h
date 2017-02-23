#pragma once

#include <io/io.h>
#include <io/readable.h>
#include <io/writable.h>
#include <basetypes/unit.h>

namespace funcpp::io {

template <typename T> class
randomly_accessible 
    : public readable<T>
    , public writable<T>
{
protected:
    struct 
    Impl : readable<T>::Impl, writable<T>::Impl {};

private:
    std::shared_ptr<Impl> m_impl;

public:
    using ImplForTestsOnly = Impl;

    randomly_accessible(std::shared_ptr<Impl> impl) 
        : readable<T>(impl)
        , writable<T>(impl)
    {}
};


}