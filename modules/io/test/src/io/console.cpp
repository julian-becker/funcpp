#include <catch.hpp>
#include <basetypes/unit.h>
#include <io/io.h>
#include <io/console.h>
#include <testsupport/static_checks.h>
#include <iostream>

TEST_CASE("console") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;
    std::string some_text = "some string data\n";

    GIVEN("An instance `c` of console") {
        console c;
        WHEN("we write to it") {
            auto action = c.write(some_text);

            THEN("the returned action will write the text to std::cout") {
                action.run();
                /// TODO: figure out how to verify that something gets written
                ///       to std::cout, or how to mock std::cout.
            }

        }

    }

    /// TODO: figure out how to mock std::cin for the purpose of testing
    ///       if console::read() works correctly.
}