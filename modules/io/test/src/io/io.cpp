#include <catch.hpp>
#include <io/io.h>

TEST_CASE("io<T>") {
    using namespace funcpp::io;

    GIVEN("An implementation `test_io` of `io<int>` as well as an `instance` of this implementation") {
        struct test_io final : io<int> {
            struct Impl final : io<int>::Impl {
                int run_impl() override {
                    return 42;
                }
            };
            test_io() : io<int>(std::make_unique<Impl>()) {}
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
}