#include <catch.hpp>
#include <typeclass/eq/list.h>
#include <typeclass/eq/optional.h>
#include <typeclass/eq/vector.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/monoid/list.h>
#include <typeclass/monoid/vector.h>
#include <typeclass/monoid/scalar.h>
#include <boost/optional.hpp>


TEST_CASE("typeclass monoid") {
    using namespace funcpp::typeclass::monoid;
    using namespace funcpp::typeclass::monoid::operators;
    using namespace funcpp::typeclass::eq;
    using namespace funcpp::typeclass::eq::operators;

    GIVEN("two lists of ints") {
        std::list<int> a{1,2,3}, b{4,5,6,7};

        THEN("Addition equal concatenation") {
            REQUIRE((std::list<int>{1,2,3,4,5,6,7} == a + b));
            REQUIRE((std::list<int>{1,2,3,4,5,6,7} == mappend(a,b)));
        }
    }

    GIVEN("a list of ints") {
        std::list<int> a{1,2,3};

        THEN("Adding mempty() doesn't change anything") {
            REQUIRE((a == a + mempty<std::list<int>>()    ));
            REQUIRE((a ==     mempty<std::list<int>>() + a));

            REQUIRE( a == mappend(a, mempty<std::list<int>>()    ));
            REQUIRE( a == mappend(   mempty<std::list<int>>(),  a));
        }
    }

    GIVEN("two vectors of ints") {
        std::vector<int> a{1,2,3}, b{4,5,6,7};

        THEN("Addition equal concatenation") {
            REQUIRE((std::vector<int>{1,2,3,4,5,6,7} == a + b));
            REQUIRE((std::vector<int>{1,2,3,4,5,6,7} == mappend(a,b)));
        }
    }

    GIVEN("a vectors of ints") {
        std::vector<int> a{1,2,3};

        THEN("Adding mempty() doesn't change anything") {
            REQUIRE((a == a + mempty<std::vector<int>>()    ));
            REQUIRE((a ==     mempty<std::vector<int>>() + a));

            REQUIRE( a == mappend(a, mempty<std::vector<int>>()    ));
            REQUIRE( a == mappend(   mempty<std::vector<int>>(),  a));
        }
    }

    GIVEN("two ints") {
        int a = 7, b = 10;

        THEN("mappend(a,b) is just the sum a+b") {
            REQUIRE(a+b == mappend(a,b));
        }

        THEN("mappend(a,mempty()) is just a") {
            REQUIRE(a == mappend(a,mempty<int>()));
            REQUIRE(a == mappend(mempty<int>(),a));
        }

        THEN("Multiplication a*b is the same as mappend<int,std::multiplies<>>(a,b)") {
            REQUIRE(a*b == (mappend<int,std::multiplies<>>(a,b)));
        }
    }
}