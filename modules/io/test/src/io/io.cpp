#include <catch.hpp>
#include <io/io.h>
#include <testsupport/static_checks.h>
#include <future>


TEST_CASE("io<T>") {
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    SECTION("type-properties") {
        using namespace std;
        STATIC_ASSERT(is_destructible<io<int>>);
        STATIC_ASSERT(is_move_constructible<io<int>>);
        STATIC_ASSERT(is_move_assignable<io<int>>);
        STATIC_ASSERT(is_copy_constructible<io<int>>);
        STATIC_ASSERT(is_copy_assignable<io<int>>);
    }

    GIVEN("An implementation `test_io` of `io<int>` as well as an `instance` of this implementation") {
        struct test_io final : io<int> {
            struct Impl final : io<int>::Impl {
                int run_impl() const override {
                    return 42;
                }
            };
            test_io() : io<int>(std::make_shared<Impl>()) {}
        };
        test_io instance;

        WHEN("`instance.run()` is executed") {
            auto value = instance.run();
            THEN("the implementation test_io::Impl::run_impl() is called") {
                REQUIRE(value == 42);
            }
        }

        WHEN("the `instance` is move-assigned to an instance of type `io<int>`") {
            io<int> base_instance = std::move(instance);
            THEN("calling `base_instance.run()` will yield the same result") {
                auto value = base_instance.run();
                REQUIRE(value == 42);
            }
        }
    }

    GIVEN("An `instance` of `io_const<int>`") {
        double some_value = 23123.445;
        io_constant<double> instance(some_value);

        SECTION("static properties") {
            using namespace std;
            STATIC_ASSERT(is_copy_constructible<io_constant<double>>);
            STATIC_ASSERT(is_move_constructible<io_constant<double>>);
        }

        WHEN("`instance.run()` is executed") {
            auto value = instance.run();
            THEN("it returns the value given in the constructor") {
                REQUIRE(some_value == value);
            }
        }
    }


    GIVEN("An `instance` of move-only type `io_const<std::future<int>>`") {
        int some_value = 23123;
        std::promise<int> f;
        f.set_value(some_value);
        io_constant<std::future<int>> instance(f.get_future());

        SECTION("static properties") {
            using namespace std;
            STATIC_ASSERT(!is_copy_constructible<io_constant<future<int>>>);
            STATIC_ASSERT(is_move_constructible<io_constant<future<int>>>);
        }

        WHEN("`instance.run()` is executed") {
            auto value = instance.run();
            THEN("it returns the value given in the constructor") {
                REQUIRE(some_value == value.get());
            }
        }
    }
}
