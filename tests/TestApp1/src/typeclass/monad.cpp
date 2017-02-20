#include <catch.hpp>
#include <typeclass/eq/list.h>
#include <typeclass/eq/optional.h>
#include <typeclass/eq/vector.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/monad/list.h>

TEST_CASE("typeclass monad") {
    using namespace funcpp::typeclass::monad;
    using namespace funcpp::typeclass::eq;

    GIVEN("two lists of ints") {
        std::list<int> a{1,2,3};
        auto fn = [](int x) -> std::list<int> { 
            return std::list<int>(x,(size_t)x); 
        };
        
        auto result = mbind(a,fn);
        REQUIRE((result == std::list<int>{1,2,2,3,3,3}));
    }

    GIVEN("two lists of ints") {
        std::list<int> a{2,3};
        
        auto result = a >> std::list<int>{10,11,12};
        REQUIRE((result == std::list<int>{10,11,12,  10,11,12}));
    }
}