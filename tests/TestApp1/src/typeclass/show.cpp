#include <catch.hpp>
#include <typeclass/show/list.h>
#include <typeclass/show/map.h>
#include <typeclass/show/vector.h>
#include <typeclass/show/string.h>
#include <typeclass/show/scalar.h>

TEST_CASE("typeclass show") {
    using namespace funcpp::typeclass::show;

    SECTION("std::list<T>") {
        std::list<int> a{1,2,3};
        REQUIRE(show(a) == "{1, 2, 3}");
    }

    SECTION("non-empty std::list<T>") {
        std::list<int> a{1,2,3};
        REQUIRE(show(a) == "{1, 2, 3}");
    }

    SECTION("empty std::list<T>") {
        std::list<int> a{};
        REQUIRE(show(a) == "{}");
    }

    SECTION("std::vector<T>") {
        std::vector<int> a{1,2,3};
        REQUIRE(show(a) == "{1, 2, 3}");
    }

    SECTION("non-empty std::vector<T>") {
        std::vector<int> a{1,2,3};
        REQUIRE(show(a) == "{1, 2, 3}");
    }

    SECTION("empty std::vector<T>") {
        std::vector<int> a{};
        REQUIRE(show(a) == "{}");
    }


    SECTION("std::string") {
        std::string s = "uiae";
        REQUIRE(show(s) == "\"uiae\"");
    }


    SECTION("empty std::vector<std::vector<T>>") {
        std::vector<std::vector<int>> a{{1,2,3},{4,5,6},{7,8,9}};
        REQUIRE(show(a) == "{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}");
    }


    SECTION("map<K,V>") {
        std::map<int,std::string> a{{10, "zehn"}, {20, "zwanzig"}};
        REQUIRE(show(a) == "{10: \"zehn\", 20: \"zwanzig\"}");
    }
}