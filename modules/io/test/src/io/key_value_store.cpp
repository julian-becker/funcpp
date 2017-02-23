#include <catch.hpp>
#include <io/io.h>
#include <io/key_value_store.h>
#include <basetypes/unit.h>
#include <testsupport/static_checks.h>
#include <future>


TEST_CASE("key_value_store<std::string,int>") {
    using namespace funcpp::io;
    using namespace funcpp::basetypes;
    using namespace funcpp::testsupport;

    GIVEN("An implementation `test_kvs` of `key_value_store<std::string,int>` as well as an `instance` of this implementation") {
        struct test_kvs final : key_value_store<std::string,int> {
            struct Impl final : key_value_store<std::string,int>::Impl {
                io<randomly_accessible<int>> access_impl(std::string key) const override {
                    REQUIRE(key == "some_key");

                    struct AccessibleMock final : randomly_accessible<int>::ImplForTestsOnly {
                        io<int> read_impl() const override { return io_constant<int>(123); }
                        io<unit_t> write_impl(int value) const override { 
                            REQUIRE(value == 0xF00BAA5);
                            return io_constant<unit_t>({}); }
                    };
                    return io_constant<randomly_accessible<int>>(randomly_accessible<int>(std::make_shared<AccessibleMock>()));
                }
            };
            
            test_kvs() 
                : key_value_store<std::string,int>(std::make_shared<Impl>()) 
            {}
        };
        test_kvs instance;

        WHEN("`instance.access(\"some_key\")` is executed") {
            auto store = instance.access("some_key");
            THEN("the implementation test_kvs::Impl::access_impl() is called with \"some_key\"") {
                /// see REQUIRES clause in the impl. of test_kvs

                AND_THEN("writing to the returned `store` calls into the implementation") {
                    store.then([](auto s){
                        s.write(0xF00BAA5); /// see REQUIRES-clause above
                        return io_constant<unit_t>({});
                    }).run();
                }

                AND_THEN("reading from the returned `store` calls into the implementation") {
                    store.then([](auto s){
                        REQUIRE(s.read().run() == 123); /// see REQUIRES-clause above
                        return io_constant<unit_t>({});
                    }).run();
                }

            }
        }

    }
}
