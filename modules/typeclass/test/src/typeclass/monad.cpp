#include <catch.hpp>
#include <typeclass/eq/list.h>
#include <typeclass/eq/optional.h>
#include <typeclass/eq/vector.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/monad/list.h>
#include <typeclass/monad/future.h>

TEST_CASE("typeclass monad") {
    using namespace funcpp::typeclass::monad;
    using namespace funcpp::typeclass::monad::operators;
    using namespace funcpp::typeclass::eq;
    using namespace funcpp::typeclass::eq::operators;

    GIVEN("two lists of ints") {
        std::list<int> a{1,2,3};
        auto fn = [](int x) -> std::list<int> { 
            return std::list<int>(x,(size_t)x); 
        };
        
        auto result = mbind(a,fn);
        REQUIRE((result == std::list<int>{1,2,2,3,3,3}));
    }

    GIVEN("a future of int") {
        //std::promise<int> a;
        auto fn = [](int a){
            return std::async([a]{ return a + 100; });
        };
        auto a = mreturn<std::future>(50);
        auto result = std::move(a) >>= fn;
        REQUIRE(result.get() == 150);
    }

    GIVEN("a future of int") {
        std::promise<int> a;
        a.set_value(50);
        auto result = a.get_future() >> mreturn<std::future>(42);
        REQUIRE(result.get() == 42);
    }
}