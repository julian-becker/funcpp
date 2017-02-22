#include <catch.hpp>
#include <io/io.h>
#include <io/writer.h>
#include <testsupport/static_checks.h>


TEST_CASE("writer<T>") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<writer<int>>);
        STATIC_ASSERT(is_move_constructible<writer<int>>);
        STATIC_ASSERT(is_move_assignable<writer<int>>);
        STATIC_ASSERT(is_copy_constructible<writer<int>>);
        STATIC_ASSERT(is_copy_assignable<writer<int>>);
    }

    GIVEN("An implementation `test_write` of `writer<int>` as well as an `instance` of this implementation") {
        struct test_write final : writer<int> {
            struct Impl final : writer<int>::Impl {
                io<unit_t> write_impl(int) const override {
                    return io_constant<unit_t>({});
                }
            };
            test_write() : writer<int>(std::make_shared<Impl>()) {}
        };
        test_write instance;

        WHEN("`instance.write(some_value)` is executed") {
            int some_value = 123444;
            auto io_action = instance.write(some_value);
            THEN("the returned `io_action` produces a value of type `unit_t`") {
                REQUIRE(unit_t{} == io_action.run());
            }
        }
    }
}