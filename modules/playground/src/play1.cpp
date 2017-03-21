#include <catch.hpp>
#include <string>
#include <iostream>
#include <boost/optional.hpp>

#include <functional>
#include <list>
#include <vector>
#include <numeric>

template <typename BinaryFunction>
struct make_infix;

template <
    typename BinaryFunction
> struct
make_infix {
    private: template <typename Arg> struct
    defer {
        Arg m_arg1;
    };
        
    public: enum class OPERATOR {} static constexpr
    result{};
    
    public: template <typename Arg1> 
    friend constexpr defer<Arg1>
    operator < (
        Arg1&& arg1,
        const OPERATOR&)
    {
        return defer<Arg1>{ std::forward<Arg1>(arg1) };
    }
    
    public: template <typename Arg1, typename Arg2> 
    friend constexpr auto
    operator > (
        defer<Arg1> deferred,
        Arg2&& arg2)
    {
        return BinaryFunction{}(std::forward<Arg1>(deferred.m_arg1),std::forward<Arg2>(arg2));
    }
};


template <typename BinaryFunction>
constexpr auto 
make_infix_v = make_infix<BinaryFunction>::result;

#define DECLARE_PROPERTY(M)                                        \
  struct M##_t {                                                   \
    using type_info = M##_t;                                       \
    constexpr auto name() const { return #M; }                     \
    constexpr M##_t() = default;                                   \
    template <typename T>                                          \
    struct instance {                                              \
      using value_type = T;                                        \
      using type_info = M##_t::type_info;                          \
                                                                   \
      T M;                                                         \
      template <typename U>                                        \
      constexpr instance(U&& value) : M{std::forward<U>(value)} {} \
                                                                   \
      T const& _access() const { return M; }                       \
      T& _access() { return M; }                                   \
    };                                                             \
                                                                   \
    template <typename U>                                          \
    constexpr auto operator=(U&& value) const {                    \
      return instance<std::decay_t<U>>{std::forward<U>(value)};    \
    }                                                              \
  } constexpr _##M{};

template <typename> struct _ {};

template <typename...Ts> struct 
product_type {                                                                  
};

template <typename T> struct 
product_type<T> : T {
    using T::T;
    using T::_access;
    
    typename T::value_type& 
    get(_<T>) { return _access(); }
};

template <typename T, typename...Ts> struct 
product_type<T,Ts...> : T, product_type<Ts...> {
    using T::_access;
    using product_type<Ts...>::get;

    typename T::value_type& 
    get(_<T>) { return _access(); }

    template <typename T1, typename...Ts1>
    product_type(T1&& value, Ts1&&...values)
        : T(std::forward<T1>(value))
        , product_type<Ts...>(std::forward<Ts1>(values)...)
    {}
};

template <typename...Instances>
product_type<Instances...> create(Instances&&...value) {
    return {std::forward<Instances>(value)...};
}


struct for_each_fn {
    template <typename...Ts, typename Fn>
    auto operator()(product_type<Ts...>& inst, Fn&& fn) const {
        int aiae[sizeof...(Ts)]{(fn(inst.get(_<Ts>{}), typename Ts::type_info{}),0)...};
    }
};

auto FOR_EACH = make_infix_v<for_each_fn>;




/// ------------------------------------------------------------------


DECLARE_PROPERTY(name);
DECLARE_PROPERTY(surname);
DECLARE_PROPERTY(hobby);
DECLARE_PROPERTY(age);



TEST_CASE("Properties") {
    auto record = create
        ( _name    = std::string{"julian"}
        , _surname = std::string{"becker"}
        , _hobby   = "abusing C++'s typesystem for reflection"
        , _age     = 31
        );
    REQUIRE(record.name    == "julian");
    REQUIRE(record.surname == "becker");
    REQUIRE(record.hobby   == "abusing C++'s typesystem for reflection");
    REQUIRE(record.age     == 31);

    record <FOR_EACH> [](auto x, auto prop) {
        std::cout << prop.name() << " := " << x << std::endl;
    };
}