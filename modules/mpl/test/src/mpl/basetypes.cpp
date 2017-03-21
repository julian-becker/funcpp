#include <catch.hpp>
#include <mpl/basetypes.h>
#include <testsupport/static_checks.h>

using namespace funcpp::mpl;

TEST_CASE("expressions") {
    SECTION("values") {
        static_assert(U8<16>{} == U8<16>{});
        static_assert(U8<16>{} == U8<16>{});
        static_assert(U8<16>{} != U8<17>{});
        static_assert(U8<16>{} != U16<16>{});
        static_assert(U32<16>{} != U16<16>{});
    }
}