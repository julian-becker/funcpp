#include <catch.hpp>
#include <typeclass/eq.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/eq/list.h>
#include <typeclass/eq/vector.h>

namespace {
    struct Foo {};
}

namespace funcpp::typeclass::eq {

template <> struct
eq_class<Foo> : std::true_type {
    static bool equal(Foo,Foo) { return true; }
};

}


TEST_CASE("typeclass eq") {
    using namespace funcpp::typeclass::eq;
    GIVEN("A custom class Foo implementing ") {
        // see definition of Foo above
        REQUIRE(equal(Foo{},Foo{}));
    }
    GIVEN("two equal ints") {
        int x = 42, y = 42;
        REQUIRE(equal(x,y));
    }
    GIVEN("two unequal ints") {
        int x = 42, y = 43;
        REQUIRE(!equal(x,y));
    }
    GIVEN("two equal lists of int") {
        std::list<int> a{1,2,3}, b{1,2,3};
        REQUIRE(equal(a,b));
    }
    GIVEN("two different lists of int") {
        std::list<int> a{2,2,3}, b{1,2,3};
        REQUIRE(!equal(a,b));
    }
    GIVEN("two equal vectors of int") {
        std::vector<int> a{1,2,3}, b{1,2,3};
        REQUIRE(equal(a,b));
    }
    GIVEN("two different vectors of int") {
        std::vector<int> a{2,2,3}, b{1,2,3};
        REQUIRE(!equal(a,b));
    }}
