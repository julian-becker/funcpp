#pragma once
#include <basetypes/unit.h>
#include <io/io.h>
#include <string>
#include <io/rw_accessible.h>
#include <io/key_value_store.h>
#include <vector>

namespace funcpp::io {


class
file_name {
    std::string m_id;

public:
    file_name(std::string id) 
        : m_id{ std::move(id) } 
    {}

    operator std::string () const { return m_id; }
};

class
mounted_device {};


class file 
    : public rw_accessible<std::vector<uint8_t>> 
{    
protected:
    struct Impl : rw_accessible<std::vector<uint8_t>>::Impl {
        std::string m_path;
        io<std::vector<uint8_t>> read_impl() const override;
        io<basetypes::unit_t> write_impl(std::vector<uint8_t>) const override;

        Impl(std::string path)
            : m_path{ std::move(path) }
        {}
    };


    // io<mounted_device> m_device;
public:
    file(std::string path)
        : rw_accessible<std::vector<uint8_t>>(std::make_shared<Impl>(std::move(path)))
    {}

    // io<unit_t> write()
    // io<T> read<T>();
};

class file_handle {
    file_name m_name;

public:
    file_handle(file_name name)
        : m_name{ std::move(name) }
    {}

    io<file> open() const;
};

class
file_system 
{
protected:
    struct Impl {
        virtual ~Impl() {}
        virtual io<file_handle> access_impl(file_name) const;
    };
private:
    std::shared_ptr<Impl> m_impl;

public:
    file_system();

    io<file_handle> access(file_name name) const {
        return m_impl->access_impl(std::move(name));
    }
};


}