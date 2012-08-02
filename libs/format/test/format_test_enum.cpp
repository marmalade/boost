// ------------------------------------------------------------------------------
//  format_test_enum.cpp :  test format use with enums
// ------------------------------------------------------------------------------

//  Copyright Steven Watanabe 2009.
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page

// ------------------------------------------------------------------------------

#include "boost/format.hpp"

#define BOOST_INCLUDE_MAIN 
#include <boost/test/test_tools.hpp>
#include "IwTest.h"

#undef BOOST_STATIC_ASSERT
#define BOOST_STATIC_ASSERT

#undef BOOST_TEST
#define BOOST_TEST(COND) IW_TEST_ASSERT_ACTION(COND, 1 == 1, (""))

#undef BOOST_CHECK
#define BOOST_CHECK(COND) BOOST_TEST(COND)

#undef BOOST_ERROR
#define BOOST_ERROR(MSG) IwTestError(MSG)

#undef BOOST_TEST_EQ
#define BOOST_TEST_EQ(VAL1, VAL2) IW_TEST_ASSERT_EQ(VAL1, VAL2)

#undef BOOST_TEST_NE
#define BOOST_TEST_NE(VAL1, VAL2) IW_TEST_ASSERT_NOTEQ(VAL1, VAL2)

#undef BOOST_CHECK_EQUAL
#define BOOST_CHECK_EQUAL(VAL1, VAL2) IW_TEST_ASSERT_COMPARATION_IMPL(VAL1, == , VAL2, 1==1)

#undef BOOST_CHECK_THROW
#define BOOST_CHECK_THROW(S, E)                                                  \
    try {                                                                        \
        S;                                                                       \
        BOOST_ERROR("Exception was expected but not thrown - " #S);              \
    }                                                                            \
    catch( E const& ex ) {                                                       \
        ::boost::unit_test::ut_detail::ignore_unused_variable_warning( ex );     \
    }

#undef BOOST_CHECK_CLOSE_FRACTION
#define BOOST_CHECK_CLOSE_FRACTION( L, R, T )             \
    BOOST_CHECK(::boost::test_tools::check_is_close(L, R, \
        ::boost::test_tools::fraction_tolerance(T)))


/*<boost.py:auto_insert>*/ namespace format_test_enum_ns {/*<boost.py:auto_insert>*/


enum enum_plain { PLAIN };
enum { ANONYMOUS };
enum enum_overloaded { OVERLOADED };
typedef enum { OVERLOADED_TYPEDEF } enum_overloaded_typedef;

std::ostream& operator<<(std::ostream& os, enum_overloaded) {
    os << "overloaded";
    return(os);
}

std::ostream& operator<<(std::ostream& os, enum_overloaded_typedef) {
    os << "overloaded";
    return(os);
}

int test_main(int, char*[]) {
    // in this case, we should implicitly convert to int
    BOOST_CHECK_EQUAL((boost::format("%d") % PLAIN).str(), "0");
    BOOST_CHECK_EQUAL((boost::format("%d") % ANONYMOUS).str(), "0");

    // but here we need to use the overloaded operator
    BOOST_CHECK_EQUAL((boost::format("%s") % OVERLOADED).str(), "overloaded");
    BOOST_CHECK_EQUAL((boost::format("%s") % OVERLOADED_TYPEDEF).str(), "overloaded");

    return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(format_test_enum)
{
    format_test_enum_ns::test_main(0, NULL);
    return S3E_TRUE;
}
