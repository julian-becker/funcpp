#include <catch.hpp>
#include <typeclass/eq/list.h>
#include <typeclass/eq/optional.h>
#include <typeclass/eq/vector.h>
#include <typeclass/eq/scalar.h>
#include <typeclass/functor.h>
#include <typeclass/functor/list.h>
#include <typeclass/functor/optional.h>
#include <typeclass/functor/vector.h>
#include <typeclass/functor/future.h>
#include <boost/optional.hpp>
#include <future>


TEST_CASE("typeclass functor") {
    using namespace funcpp::typeclass::functor;
    using namespace funcpp::typeclass::eq;
    GIVEN("a list of ints") {
        std::list<int> a{1,2,3};
        auto result = fmap([](auto x){ return x*2; }, a);
        REQUIRE((std::list<int>{2,4,6} == result));
    }
    GIVEN("a vector of ints") {
        std::vector<int> a{1,2,3};
        auto result = fmap([](auto x){ return x*2; }, a);
        REQUIRE((std::vector<int>{2,4,6} == result));
    }
    GIVEN("an optional int") {
        boost::optional<int> a{10};
        auto result = fmap([](auto x){ return x*2; }, a);
        REQUIRE((boost::optional<int>{20} == result));
    }
    GIVEN("a future of int") {
        std::promise<int> a;
        auto result = fmap([](auto x){ return x*2; }, a.get_future());
        a.set_value(10);
        REQUIRE(20 == result.get());
    }
}