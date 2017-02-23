#include <catch.hpp>
#include <io/io.h>
#include <io/randomly_accessible.h>
#include <testsupport/static_checks.h>


TEST_CASE("randomly_accessible<T>") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<randomly_accessible<int>>);
        STATIC_ASSERT(is_move_constructible<randomly_accessible<int>>);
        STATIC_ASSERT(is_move_assignable<randomly_accessible<int>>);
        STATIC_ASSERT(is_copy_constructible<randomly_accessible<int>>);
        STATIC_ASSERT(is_copy_assignable<randomly_accessible<int>>);
    }

    GIVEN("An implementation `test_read` of `randomly_accessible<int>` as well as an `instance` of this implementation") {
        static constexpr int some_value = 1234;

        struct test_read final : randomly_accessible<int> {
            struct Impl final : randomly_accessible<int>::Impl {
                int m_val;
                io<int> read_impl() const override {
                    return io_constant<int>(m_val);
                }
                io<unit_t> write_impl(int) const override {
                    return io_constant<unit_t>({});
                }
                Impl(int val) : m_val(val) {}
            };
            test_read(int val) : randomly_accessible<int>(std::make_shared<Impl>(val)) {}
        };
        test_read instance(some_value);

        WHEN("`instance.read()` is executed") {
            auto io_action = instance.read();
            THEN("the returned `io_action` produces a value of type `int`") {
                REQUIRE(some_value == io_action.run());
            }
        }

        WHEN("`instance.write(some_value)` is executed") {
            int some_value = 123444;
            auto io_action = instance.write(some_value);
            THEN("the returned `io_action` produces a value of type `unit_t`") {
                REQUIRE(unit_t{} == io_action.run());
            }
        }
    }
}