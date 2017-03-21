#include <catch.hpp>
#include <iostream>
#include <type_traits>
#include <boost/optional.hpp>
#include <typeindex>
#include <future>
#include <map>

struct unit_t : std::tuple<> {} unit;

template <typename T>
class Eff;

template <typename T, typename...Es>
class Eff<T(Es...)> : private std::function<T(std::decay_t<Es>&...)> {
    using base = std::function<T(std::decay_t<Es>&...)>;
public:
    template <typename Fn>
    Eff(Fn&& action) 
        : base{ std::forward<Fn>(action) }
    {}

    using base::operator();
};


template
    < typename T
    , typename Fn
    , typename...Es
    , typename res = decltype(std::declval<Fn>()(std::declval<T>()))
    > 
auto
bind(Eff<T(Es...)> a, Fn&& fn) -> res {
    return res{
        [a=std::move(a), fn=std::forward<Fn>(fn)](Es&...es) {
            return fn(a(es...))(es...);
        }
    };
}

template
    < typename T
    , typename Fn
    , typename...Es
    , typename res = decltype(std::declval<Fn>()(std::declval<T>()))
    > 
auto
operator >= (Eff<T(Es...)> a, Fn&& fn) -> res {
    return bind(std::move(a), std::forward<Fn>(fn));
}

namespace console {

    struct CONSOLE {
        void writeln(std::string s) {
            std::cout << s << std::endl;
        }

        std::string readln() {
            std::string s;
            std::cin >> s;
            return s;
        }
    };

    auto writeln(std::string s) {
        return Eff<void(CONSOLE)>{
            [s=std::move(s)](CONSOLE& c) {
                c.writeln(s);
            }
        };
    }

    auto readln() {
        return Eff<std::string(CONSOLE)>{
            [](CONSOLE& c) {
                std::string s;
                return c.readln();
            }
        };
    }
}


class 
LockableDevice {
    mutable std::mutex mutex;
    std::shared_ptr<void> const device;

public:
    template <typename Device>
    LockableDevice(std::unique_ptr<Device> device)
        : device(std::move(device))
    {
    }

    void 
    lock() const {
        mutex.lock();
    }

    void 
    unlock() const {
        mutex.unlock();
    }

    bool 
    try_lock() const {
        return mutex.try_lock();
    }

private:    
    friend class ResourceRegistry;

    /// nobody should access the device unprotected except for the ResourceRegistry
    template <typename Device>
    Device* 
    access () {
        return static_cast<Device*>(device.get());
    }
};

template <typename T1, typename T2, typename...Ts>
auto 
lock(T1& t1, T2& t2, Ts&... ts) {
    std::lock(t1, t2, ts...);
    return std::shared_ptr<void>(nullptr,[&](void*){
        t1.unlock(), t2.unlock();
        int unlockRest[]{(ts.unlock(),1)...};
    });
}

template <typename T1>
auto 
lock(T1& t1) {
    t1.lock();
    return std::shared_ptr<void>(nullptr,[&](void*){ t1.unlock(); });
}


template <typename...Devices> struct
requires {
    static constexpr size_t size = sizeof...(Devices);
};

template <typename T> struct
decay_args_of;

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...)> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) const> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) const volatile> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) &> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) const& > {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) const volatile &> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) &&> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts> struct
decay_args_of<Ret(Class::*)(Ts...) const&&> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Ret,typename Class,typename...Ts>
struct decay_args_of<Ret(Class::*)(Ts...) const volatile &&> {
    using type = requires<std::decay_t<Ts>...>;
};

template <typename Cmd> struct
requirements_of;

template <typename Cmd> struct
requirements_of {
    using type = typename decay_args_of<decltype(&std::decay_t<Cmd>::operator())>::type;
};

template <typename Cmd> using 
requirements_of_t = typename requirements_of<Cmd>::type;


class 
ResourceRegistry {
    std::mutex 
    mutex;

    std::map<std::type_index, std::unique_ptr<LockableDevice>> 
    deviceMap;

public:
    /// @brief returns a pair consisting of a lock-scope object and a tuple containing pointers to the locked resources
    template <typename...Ts>
    auto 
    acquire(requires<Ts...>) {
        std::lock_guard<std::mutex> selfProtect(mutex);
        auto lockScope = lock(*deviceMap.at(std::type_index(typeid(Ts)))...);
        return std::make_pair(
            std::move(lockScope),
            std::make_tuple(
                deviceMap.at(std::type_index(typeid(Ts)))->access<Ts>()...
            )
        );
    }

    /// @brief Trivial specialization for the empty case where no resources are required
    ///        The lock-scope is a noop and the tuple of resources is empty.
    auto 
    acquire(requires<>) {
        return std::make_pair(std::shared_ptr<void>(),std::tuple<>());
    }

    /// @brief Hand over ownership for the given device to the registry
    template <typename Device>
    void 
    registerDevice(std::unique_ptr<Device> device) {
        std::lock_guard<std::mutex> selfProtect(mutex);
        auto lockDev = std::make_unique<LockableDevice>(std::move(device));
        deviceMap.emplace(std::make_pair(std::type_index(typeid(Device)), std::move(lockDev)));
    }
};

class 
Executor {
public:
    void execute(std::function<void()> f) {
        f();
    }
};

template <int,typename> struct 
type_at_impl;

template <typename T, typename...Ts, template <typename...> class F> struct
type_at_impl<0,F<T,Ts...>> {
    using type = T;
};

template <int N, typename T, typename...Ts, template <typename...> class F> struct
type_at_impl<N,F<T,Ts...>> : type_at_impl<N-1, F<Ts...>> {};

template <int N, typename T> using 
type_at = typename type_at_impl<N,T>::type;

class 
ResourceOrchestrator {
    std::unique_ptr<ResourceRegistry> 
    resourceRegistry;

    std::shared_ptr<Executor> 
    executor;

    template <typename Cmd, typename...Ts>
    static auto 
    result_type_helper(Cmd&& cmd,requires<Ts...>) -> decltype(cmd(std::declval<std::add_lvalue_reference_t<Ts>>()...));

    template <typename Command> struct
    result_of {
        using 
        type = decltype(result_type_helper(std::declval<std::decay_t<Command>>(), requirements_of_t<Command>()));
    };

    template <typename T> using 
    result_of_t = typename result_of<T>::type;

public:
    ResourceOrchestrator(std::unique_ptr<ResourceRegistry> resourceRegistry, std::shared_ptr<Executor> executor)
        : resourceRegistry(std::move(resourceRegistry))
        , executor(std::move(executor))
    {
    }


    template<
        typename Result,
        typename Command,
        typename Devices,
        int ...S
    > std::enable_if_t<std::is_same<Result,void>::value, void>
    executeWithDevices(
        std::promise<Result> promise,
        Command&& cmd,
        std::shared_ptr<void> lockScope,
        Devices&& devs,
        std::index_sequence<S...>)
    {
        auto unboundTask =
            [](std::promise<Result>& prom, Command& cmd, type_at<S,Devices>&...devices) {
                try {
                    cmd(*devices...);
                    prom.set_value();
                } catch ( ... ) {
                    prom.set_exception(std::current_exception());
                }
            };
        /// the callable returned by std::bind is not movable, so we wrap it in a std::shared_ptr in a std::function
        /// in order to keep all lockScopes alive for the lifetime of the task.
        std::function<void()> boundTask = shared_function(std::bind(unboundTask, std::move(promise), std::forward<Command>(cmd), std::get<S>(devs)...));
        executor->execute(boundTask);
    }


    template<
        typename Result,
        typename Command,
        typename Devices,
        size_t ...S
    > std::enable_if_t<!std::is_same<Result,void>::value, void>
    executeWithDevices(
        std::promise<Result> promise,
        Command&& cmd,
        std::shared_ptr<void> lockScope,
        Devices&& devs,
        std::index_sequence<S...>)
    {
        auto unboundTask =
            /// accepts args by lvalue reference, because they will be bound into the task alongside this lambda below
            [](std::promise<Result>& prom, Command& cmd, type_at<S,Devices>&...devices) {
                try {
                    auto result = cmd(*devices...);
                    prom.set_value(std::move(result));
                } catch ( ... ) {
                    prom.set_exception(std::current_exception());
                }
            };
        std::function<void()> task = shared_function(std::bind(unboundTask, std::move(promise), std::forward<Command>(cmd), std::get<S>(devs)...));
        executor->execute(task);
    }

    template <
        typename Command,
        typename Result = result_of_t<Command>,
        typename requiredResources = requirements_of_t<Command>
    > auto
    execute(Command&& cmd) -> std::future<Result> {
        auto devices = resourceRegistry->acquire(requiredResources());
        std::promise<Result> promise;
        auto future = promise.get_future();
        executeWithDevices(std::move(promise), std::forward<Command>(cmd), std::move(devices.first), std::move(devices.second), std::make_index_sequence<requiredResources::size>{});
        return future;
    }
};


/// This came in handy above, which I found somwhere on stackoverflow:
template<class F>
auto shared_function( F&& f ) {
  auto pf = std::make_shared<std::decay_t<F>>(std::forward<F>(f));
  return [pf](auto&&... args){
    return (*pf)(decltype(args)(args)...);
  };
}

struct Device1 {};
struct Device2 {};

TEST_CASE("") {
    auto registry = std::make_unique<ResourceRegistry>();
    auto executor = std::make_shared<Executor>();

    auto dev1 = std::make_unique<Device1>();
    registry->registerDevice(std::move(dev1));

    auto dev2 = std::make_unique<Device2>();
    registry->registerDevice(std::move(dev2));  

    auto orchestrator = std::make_shared<ResourceOrchestrator>(std::move(registry), executor);

    struct FooCmdSequence {
        int operator() (Device1& dev1, Device2& dev2) {
            std::cout << "FooCmdSequence\n";
            return 0;
        }
    };

    /// just execute and provide locked resources automatically:
    ///orchestrator->execute(FooCmdSequence());  
}
