#pragma once
#include <type_traits>
#include <catch.hpp>

#define STATIC_ASSERT(...) do {             \
        static_assert(__VA_ARGS__::value);  \
        REQUIRE((__VA_ARGS__::value));      \
    } while(false)


namespace funcpp::testsupport {

}