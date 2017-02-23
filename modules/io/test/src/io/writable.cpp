#include <catch.hpp>
#include <io/io.h>
#include <io/writable.h>
#include <testsupport/static_checks.h>


TEST_CASE("writable<T>") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<writable<int>>);
        STATIC_ASSERT(is_move_constructible<writable<int>>);
        STATIC_ASSERT(is_move_assignable<writable<int>>);
        STATIC_ASSERT(is_copy_constructible<writable<int>>);
        STATIC_ASSERT(is_copy_assignable<writable<int>>);
    }

    GIVEN("An implementation `test_write` of `writable<int>` as well as an `instance` of this implementation") {
        struct test_write final : writable<int> {
            struct Impl final : writable<int>::Impl {
                io<unit_t> write_impl(int) const override {
                    return io_constant<unit_t>({});
                }
            };
            test_write() : writable<int>(std::make_shared<Impl>()) {}
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