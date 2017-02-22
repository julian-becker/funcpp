#include <catch.hpp>
#include <io/io.h>
#include <io/write.h>
#include <testsupport/static_checks.h>


TEST_CASE("write<T>") {
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<write<int>>);
        STATIC_ASSERT(is_move_constructible<write<int>>);
        STATIC_ASSERT(is_move_assignable<write<int>>);
        STATIC_ASSERT(is_copy_constructible<write<int>>);
        STATIC_ASSERT(is_copy_assignable<write<int>>);
    }

    GIVEN("An implementation `test_write` of `write<int>` as well as an `instance` of this implementation") {
        struct test_write final : io<int> {
            struct Impl final : io<int>::Impl {
                io<int> write_impl(int value) const override {
                    return io_constant<int>{value};
                }
            };
            test_write() : write<int>(std::make_shared<Impl>()) {}
        };
        test_write instance;

        WHEN("`instance.write(some_value)` is executed") {
            int some_value = 123;
            auto io = instance.run(some_value);
            THEN("the implementation test_write::Impl::write_impl() is called") {
                REQUIRE(some_value == io.run());
            }
        }
    }
}