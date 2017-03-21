#pragma once
#include <type_traits>

namespace funcpp::mpl {
	
template <typename T, T Value> struct
singleton {
    using type = T;
    static constexpr type value = Value;
    constexpr operator T() const { return value; }
};

template <typename T, T Value>
constexpr T singleton<T, Value>::value;

template <uint8_t Value> using
U8 = singleton<uint8_t, Value>;

template <int8_t Value> using
S8 = singleton<int8_t, Value>;

template <uint16_t Value> using
U16 = singleton<uint16_t, Value>;

template <int16_t Value> using
S16 = singleton<int16_t, Value>;

template <uint32_t Value> using
U32 = singleton<uint32_t, Value>;

template <int32_t Value> using
S32 = singleton<int32_t, Value>;

template <uint64_t Value> using
U64 = singleton<uint64_t, Value>;

template <int64_t Value> using
S64 = singleton<int64_t, Value>;

using 
true_type = singleton<bool, true>;

using 
false_type = singleton<bool, false>;




template 
    < typename T
    , T Vt
    , typename U
    , U Vu
    >
constexpr 
std::enable_if_t<!std::is_same<T,U>::value, false_type>
operator== (singleton<T,Vt>, singleton<U,Vu>) {
    return {};
}


template 
    < typename T
    , T Vt
    , typename U
    , U Vu
>
constexpr
std::enable_if_t<std::is_same<T,U>::value, singleton<bool,Vt == Vu>>
operator== (singleton<T,Vt>, singleton<U,Vu>) {
    return {};
}




template 
    < typename T
    , T Vt
    , typename U
    , U Vu
    >
constexpr 
std::enable_if_t<!std::is_same<T,U>::value, true_type>
operator!= (singleton<T,Vt>, singleton<U,Vu>) {
    return {};
}


template 
    < typename T
    , T Vt
    , typename U
    , U Vu
>
constexpr
std::enable_if_t<std::is_same<T,U>::value, singleton<bool,Vt != Vu>>
operator!= (singleton<T,Vt>, singleton<U,Vu>) {
    return {};
}



}