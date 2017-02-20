#include <catch.hpp>
#include <typeclass/eq.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/eq/list.h>
#include <typeclass/eq/vector.h>
#include <typeclass/eq/optional.h>

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
        int a = 42, b = 42;
        REQUIRE(equal(a,b));
        REQUIRE((a == b));
    }
    GIVEN("two unequal ints") {
        int a = 42, b = 43;
        REQUIRE(!equal(a,b));
        REQUIRE((a != b));
    }
    GIVEN("two equal lists of int") {
        std::list<int> a{1,2,3}, b{1,2,3};
        REQUIRE(equal(a,b));
        REQUIRE((a == b));
    }
    GIVEN("two different lists of int") {
        std::list<int> a{2,2,3}, b{1,2,3};
        REQUIRE(!equal(a,b));
        REQUIRE((a != b));
    }
    GIVEN("two equal vectors of int") {
        std::vector<int> a{1,2,3}, b{1,2,3};
        REQUIRE(equal(a,b));
        REQUIRE((a == b));
    }
    GIVEN("two different vectors of int") {
        std::vector<int> a{2,2,3}, b{1,2,3};
        REQUIRE(!equal(a,b));
        REQUIRE((a != b));
    }
    GIVEN("two equal optionals of int") {
        boost::optional<int> a{1}, b{1};
        REQUIRE(equal(a,b));
        REQUIRE((a == b));
    }
    GIVEN("two different optionals of int") {
        boost::optional<int> a{1}, b{2};
        REQUIRE(!equal(a,b));
        REQUIRE((a != b));
    }
}
