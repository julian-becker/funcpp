
#include <iostream>
#include <fstream>
#include <vector>
#include <type_traits>
#include <map>
#include <experimental/optional>

namespace std { 
    using namespace std::experimental;

    template< class, class = std::void_t<> >
    struct is_callable : std::false_type { };

    template< class T >
    struct is_callable<T, std::void_t<decltype(std::declval<T>()())>> : std::true_type { };

}

template <typename T> struct with_type { using type = T; };

struct unit_t : std::tuple<> {};

constexpr unit_t unit {};

template <typename T>
class IO : public with_type<T> {
    std::function<T()> m_action;

public:
    template <typename Fn>
    IO(Fn&& action)
        : m_action{ std::forward<Fn>(action) }
    {}

    template <typename U>
    IO(IO<U> & action)
        : m_action{ action.m_action }
    {}

    template <typename U>
    IO(IO<U> const & action)
        : m_action{ action.m_action }
    {}

    template <typename U>
    IO(IO<U>&& action)
        : m_action{ std::move(action.m_action) }
    {}

    IO(IO&&) = default;
    IO(IO const&) = default;

    IO& operator= (IO&&) = default;
    IO& operator= (IO const&) = default;

    template <typename Fn>
    auto then(Fn&& cont) const {
        using res_t = decltype(cont(m_action()));
        return res_t{ [cont=std::forward<Fn>(cont),action=m_action]() mutable {
            return cont(action()).run();
        }};
    }

    T run() { return m_action(); }
};


template <typename U>
auto yield(U&& val) -> IO<std::decay_t<U>> {
    return IO<std::decay_t<U>> { [val=std::forward<U>(val)]() -> std::decay_t<U> { 
        return val; 
    }};
}

class SharedResource {
protected:
    struct Details {
        virtual ~Details() {}
    };

private:
    std::shared_ptr<Details> m_impl;

public:
    SharedResource(std::shared_ptr<Details> impl) 
        : m_impl{ std::move(impl) }
    {}
};


template <typename T>
class ReadStore {
protected:
    struct Details {
        virtual IO<std::optional<T>> read_impl() const = 0;
        virtual ~Details() {}
    };

private:
    std::shared_ptr<Details> m_impl;

public:
    ReadStore(std::shared_ptr<Details> impl) : m_impl{ std::move(impl) } {}

    IO<std::optional<T>>      
    read() { return m_impl->read_impl(); }
};

template <typename T>
class WriteStore {
protected:
    struct Details {
        virtual IO<std::optional<unit_t>> write_impl(T value) const = 0;
        virtual ~Details() {}
    };

private:
    std::shared_ptr<Details> m_impl;
public:
    WriteStore(std::shared_ptr<Details> impl) : m_impl{ std::move(impl) } {}

    IO<std::optional<unit_t>> 
    write(T value) { return m_impl->write_impl(std::move(value)); }
};

template <typename T>
class Store 
    : public ReadStore<T>
    , public WriteStore<T> 
{
protected:
    struct Details : ReadStore<T>::Details, WriteStore<T>::Details {};

public:
    Store(std::shared_ptr<Details> impl)
        : ReadStore<T>(impl)
        , WriteStore<T>(impl)
    {}
};


class SDCardMounted : SharedResource {
    struct Details : SharedResource::Details {
        std::string m_dev;

        virtual ~Details() override {
            std::cout << "Executing SD-card unmount action for device " << m_dev << std::endl;
        }

        Details(std::string dev)
            : m_dev{ std::move(dev) }
        {
            std::cout << "Executing SD-card mount action for device " << m_dev << std::endl;
        }
    };
public:
    SDCardMounted(std::string dev) 
        : SharedResource(std::make_shared<Details>(std::move(dev))) {}
};


class FileStore final
    : public Store<std::vector<uint8_t>> 
{
    struct Details : Store<std::vector<uint8_t>>::Details {
        std::string m_filename;

        virtual IO<std::optional<unit_t>> write_impl(std::vector<uint8_t> value) const override {
            return IO<std::optional<unit_t>>([fname=m_filename, value=std::move(value)]() mutable -> std::optional<unit_t> { 
                try {
                    std::ofstream(fname, std::ios::binary).write(reinterpret_cast<char const*>(value.data()),value.size());
                    return std::make_optional(unit);
                } catch(...) {
                    return std::nullopt;
                }
            });
        }

        virtual IO<std::optional<std::vector<uint8_t>>> read_impl() const override {
            return IO<std::optional<std::vector<uint8_t>>>([fname=m_filename]() mutable -> std::optional<std::vector<uint8_t>> { 
                try {
                    std::ifstream file(fname, std::ios::binary | std::ios::ate);
                    auto const size = file.tellg();
                    file.seekg(0, std::ios::beg);

                    std::vector<uint8_t> buffer(size);
                    if(!file.read(reinterpret_cast<char*>(buffer.data()), size))
                        return std::nullopt;
                    return std::make_optional(std::move(buffer));
                }
                catch(...) { 
                    return std::nullopt;
                }
            });
        }

        Details(std::string filename) 
            : m_filename{std::move(filename)} {}
    };


public:
    FileStore(std::string filename)
        : Store<std::vector<uint8_t>>(std::make_shared<Details>(std::move(filename)))
    {}
};


class MountedFileStore final
    : public Store<std::vector<uint8_t>>
{
    struct Details : Store<std::vector<uint8_t>>::Details {
        IO<std::optional<SDCardMounted>> m_mountedState;
        FileStore m_file;

        virtual IO<std::optional<unit_t>> write_impl(std::vector<uint8_t> value) const override {
            return m_mountedState.then([file=m_file, val=std::move(value)](std::optional<SDCardMounted> mounted) mutable {
                if(!mounted) return yield<std::optional<unit_t>>(std::nullopt); // TODO: monad transformers
                return file.write(std::move(val));
            });
        }

        virtual IO<std::optional<std::vector<uint8_t>>> read_impl() const override {
            return m_mountedState.then([file=m_file](std::optional<SDCardMounted> mounted) mutable { 
                if(!mounted) return yield<std::optional<std::vector<uint8_t>>>(std::nullopt); // TODO: monad transformers
                return file.read();
            });
        }

        Details(IO<std::optional<SDCardMounted>> mountedState, FileStore file) 
            : m_mountedState{ std::move(mountedState) }
            , m_file{std::move(file)} {}
    };


public:
    MountedFileStore(IO<std::optional<SDCardMounted>> mountedState, std::string filename)
        : Store<std::vector<uint8_t>>(std::make_shared<Details>(std::move(mountedState), FileStore(filename)))
    {
    }
};


class Console {
public:
    IO<std::ostream&> cout() const {
        return { []() -> std::ostream& { return { std::cout }; }};
    }
};


class SDCardMounter {
    using device_id_t = std::size_t;
    std::map<device_id_t, std::string> m_devMap;

public:
    SDCardMounter() 
        : m_devMap{ {0, "/dev/mmcblk1p1"}, {1, "/dev/sda1"} }
    {}

    IO<std::optional<SDCardMounted>> mounted(device_id_t devId) const {
        return { [device=m_devMap.at(devId)]() -> std::optional<SDCardMounted> {
            return std::make_optional(SDCardMounted{device});
        }};
    }
};





template <typename T>
class StreamWriteStore final
    : public WriteStore<T>
{
    struct Details 
        : WriteStore<T>::Details
    {
        IO<std::ostream&> m_stream;

        virtual IO<std::optional<unit_t>> write_impl(T value) const override {
            return m_stream.then([value=std::move(value)](std::ostream & stream) mutable -> IO<std::optional<unit_t>> { 
                try {
                    stream << std::move(value);
                    return yield<std::optional<unit_t>>(std::make_optional(unit));
                } catch(...) {
                    return yield<std::optional<unit_t>>(std::nullopt);
                }
            });
        }

        Details(IO<std::ostream&> stream) 
            : m_stream{std::move(stream)} {}
    };


public:
    StreamWriteStore(IO<std::ostream&> stream)
        : WriteStore<T>(std::make_shared<Details>(std::move(stream)))
    {}
};




int main() {
    auto const mountedCard = SDCardMounter().mounted(/*devId*/0);
    Store<std::vector<uint8_t>> store = MountedFileStore{mountedCard, "file1"};

    WriteStore<std::vector<uint8_t>> writeStore = store; // slicing has no effect :-)
    ReadStore<std::vector<uint8_t>> readStore = store;

    {
        std::vector<uint8_t> data{1,2,3,4,5};
        auto action = writeStore.write(data).then([](auto res) -> IO<unit_t> {
            if(res)
                std::cout << "successfully wrote file" << std::endl;
            return yield(unit);
        });

        action.run();
    }

    {
        std::vector<uint8_t> dataIn;
        auto readAction = readStore.read();
        
        if(auto readResult = readAction.run()) {
            std::cout << "successfully read file" << std::endl;
            for(auto const& i : *readResult)
                std::cout << (int)i << std::endl;
        }
        else {
            std::cout << "failure reading file" << std::endl;
        }
    }

    {
        auto c = Console{}.cout();
        StreamWriteStore<double> doubles(c);
        StreamWriteStore<std::string> strings(c);
        std::vector<IO<std::optional<unit_t>>> actions {
            strings.write("Hello "),
            strings.write("World\n"),
            doubles.write(3.14159)
        };
        for(auto& action : actions)
            action.run();
    }
}