#include <catch.hpp>
#include <io/io.h>
#include <testsupport/static_checks.h>
#include <future>
#include <typeclass/monad.h>

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

        WHEN("a `continuation` is chained with the `instance`") {
            auto result = instance.then([](int value) -> io<double> {
                return io_constant<double>(value + 1000.0);
            });

            THEN("running the `result` will chain the result of the "
                 "execution of `instance` into the execution of the "
                 "continuation") {
                REQUIRE(result.run() == 1042);
            }
        }


        WHEN("a `continuation` returning an `io_constant<T>` is chained with the `instance`") {
            auto result = instance.then([](int value) -> io_constant<double> {
                return io_constant<double>(value + 1000.0);
            });

            THEN("running the `result` will chain the result of the "
                 "execution of `instance` into the execution of the "
                 "continuation") {
                REQUIRE(result.run() == 1042);
            }
        }
    }



    GIVEN("An `instance` of `io_constant<int>`") {
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







TEST_CASE("io<T> conversion to io<U>") {
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    GIVEN("Two types A and B, where A is convertible to B") {
        struct B {
            int b_val = 42;
        };

        struct A {
            int a_val = 77;
            operator B() {
                return { 43 };
            }
        };

        WHEN("an `instance_a` of type `io<A>` is assigned to `instance_b` of type `io<B>`") {
            io<A> instance_a = io_constant<A>({});
            io<B> instance_b = instance_a;
            THEN("running the action of `instance_b` results in the conversion/assignment "
                 "from the result of the action of `instance_a`") 
            {
                REQUIRE(instance_b.run().b_val == 43);
            }
        }

        WHEN("an `instance_b` of type `io<B>` is assigned to `instance_a` of type `io<A>` "
             "where B is not convertible to A") 
        {
            io<B> instance_b = io_constant<B>({});
            THEN("Things don't compile...") 
            {
                /// Not possible presently to check for compilation errors
                /// REQUIRE_FAILS_COMPILATION(io<A> instance_a = instance_b);
            }
        }
    }
}








TEST_CASE("io_action<T>")  {
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    GIVEN("an `instance` of io_action<int>") {
        io_action<int> instance([]{ return 123; });
        WHEN("the action is run") {
            auto result = instance.run();
            THEN("the result is the return value of the lambda given "
                 "to the constructor of the `instance`") {
                REQUIRE(result == 123);
            }
        }
    }

}