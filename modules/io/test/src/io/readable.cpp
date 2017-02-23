#include <catch.hpp>
#include <io/io.h>
#include <io/readable.h>
#include <testsupport/static_checks.h>


TEST_CASE("readable<T>") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<readable<int>>);
        STATIC_ASSERT(is_move_constructible<readable<int>>);
        STATIC_ASSERT(is_move_assignable<readable<int>>);
        STATIC_ASSERT(is_copy_constructible<readable<int>>);
        STATIC_ASSERT(is_copy_assignable<readable<int>>);
    }

    GIVEN("An implementation `test_read` of `readable<int>` as well as an `instance` of this implementation") {
        static constexpr int some_value = 1234;

        struct test_read final : readable<int> {
            struct Impl final : readable<int>::Impl {
                int m_val;
                io<int> read_impl() const override {
                    return io_constant<int>(m_val);
                }
                Impl(int val) : m_val(val) {}
            };
            test_read(int val) : readable<int>(std::make_shared<Impl>(val)) {}
        };
        test_read instance(some_value);

        WHEN("`instance.read()` is executed") {
            auto io_action = instance.read();
            THEN("the returned `io_action` produces a value of type `int`") {
                REQUIRE(some_value == io_action.run());
            }
        }
    }
}