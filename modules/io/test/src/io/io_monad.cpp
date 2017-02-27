#include <catch.hpp>
#include <io/io.h>
#include <io/io_monad.h>
#include <basetypes/unit.h>
#include <testsupport/static_checks.h>
#include <future>
#include <typeclass/monad.h>

TEST_CASE("io<T> is a monad")  {
    using namespace funcpp::io;
    using namespace funcpp::testsupport;
    using namespace funcpp::typeclass::monad;
    using namespace funcpp::typeclass::monad::operators;

    GIVEN("an `instance` of io<int>") {
        io<int> instance = io_action<int>([]{ return 123; });
        WHEN("we chain the output into a continuation") {
            auto result = mbind(instance, [](int result) {
                THEN("The continuation is called with the output of instance") {
                    REQUIRE(result == 123);
                }
                return io_constant<funcpp::basetypes::unit_t>({});
            }).run();
        }
    }


    GIVEN("an `instance` of io<int>") {
        io<int> instance = io_action<int>([]{ return 123; });
        WHEN("we chain the output into a continuation using overloaded operators") {
            auto action = instance >>= [](int result) {
                THEN("The continuation is called with the output of instance") {
                    REQUIRE(result == 123);
                }
                return io_constant<funcpp::basetypes::unit_t>({});
            };
            action.run();
        }
    }

}
