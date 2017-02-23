#include <catch.hpp>
#include <basetypes/unit.h>
#include <io/io.h>
#include <io/file_system.h>
#include <testsupport/static_checks.h>

#include <iostream>

TEST_CASE("filesystem") {
    using namespace funcpp::basetypes;
    using namespace funcpp::io;
    using namespace funcpp::testsupport;

    GIVEN("An instance of file_system") {
        file_system fs;

        std::size_t testdata_size = 1024ull*1024ull;
        std::vector<uint8_t> testdata(testdata_size);
        for(std::size_t i = 0u; i<testdata_size; ++i)
            testdata[i] = i % 256;

        THEN("we can actually write into the file") {
            file_name filename("/tmp/testfile");
            {
                fs.access(filename).then([](auto h){
                    return h.open();
                }).then([&testdata](auto file){ 
                    return file.write(testdata); 
                }).run();
            }


            AND_THEN("reading from the same file produces the written date") {
                auto result = fs.access(filename).then([](auto h){
                    return h.open();
                }).then([](auto file){ 
                    return file.read(); 
                }).run();

                REQUIRE(result == testdata);
            }

        }

    }
}