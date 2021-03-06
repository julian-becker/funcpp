#include <io/file_system.h>
#include <io/io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>

using namespace funcpp;
using namespace funcpp::io;
using namespace funcpp::basetypes;

static auto read_extern_c = &read;
static auto write_extern_c = &write;

io<std::vector<uint8_t>> 
file::Impl::read_impl() const {
    return io_action<std::vector<uint8_t>>([path=m_path]{
        static constexpr std::size_t buffer_size = 1024ull * 1024ull;
        auto fd = open(path.c_str(), O_RDONLY);
        if(fd == -1)
            throw std::runtime_error("couldn't open file");

        auto e = errno;
        std::vector<uint8_t> buffer;
        try {
            ssize_t s = 0, ss = 0;
            do {
                s += ss;
                buffer.resize(s + buffer_size);
                ss = read_extern_c(fd, buffer.data() + s, buffer_size);
            } while(0 < ss);
            buffer.resize(s);
        } catch(...) {
            close(fd);
            throw;
        }
        close(fd);
        return buffer;
    });
}

io<basetypes::unit_t> 
file::Impl::write_impl(std::vector<uint8_t> data) const {
    return io_action<unit_t>([path=m_path, data=std::move(data)]{
        auto fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);

        if(fd == -1)
            throw std::runtime_error("couldn't open file");
        
        if(-1 == fchmod(fd,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))
            throw std::runtime_error("couldn't set file permissions");

        try {
            ssize_t s = 0, ss = 0;
            do {
                s += ss;
                auto ss = write_extern_c(fd, data.data() + s, data.size());
            } while(0 < ss);
        }
        catch(...) {
            close(fd);
            throw;
        }
        close(fd);
        return unit_t{};
    });
}


io<file> 
file_handle::open() const
{
    return io_action<file>([name = m_name]{
        return file{ std::move(name) };
    });
}

file_system::file_system()
    : m_impl{ std::make_shared<Impl>() }
{}


io<file_handle> 
file_system::Impl::access_impl(file_name name) const {
    return io_constant<file_handle>(std::move(name));
}
