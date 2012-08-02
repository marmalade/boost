//  Unit test for boost::lexical_cast.
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Antony Polukhin, 2011.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>

#if defined(__INTEL_COMPILER)
#pragma warning(disable: 193 383 488 981 1418 1419)
#elif defined(BOOST_MSVC)
#pragma warning(disable: 4097 4100 4121 4127 4146 4244 4245 4511 4512 4701 4800)
#endif

#include <boost/lexical_cast.hpp>

#include <boost/cstdint.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
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


/*<boost.py:auto_insert>*/ namespace lexical_cast_wchars_test_ns {/*<boost.py:auto_insert>*/

using namespace boost;

void test_char_types_conversions()
{
//#ifndef BOOST_LCAST_NO_WCHAR_T
#if 0
    const char c_arr[]            = "Test array of chars";
    const unsigned char uc_arr[]  = "Test array of chars";
    const signed char sc_arr[]    = "Test array of chars";
    const wchar_t wc_arr[]        =L"Test array of chars";

    // Following tests depend on realization of std::locale
    // and pass for popular compilers and STL realizations
    BOOST_CHECK(boost::lexical_cast<wchar_t>(c_arr[0]) == wc_arr[0]);
    BOOST_CHECK(boost::lexical_cast<std::wstring>(c_arr) == std::wstring(wc_arr));

    BOOST_CHECK(boost::lexical_cast<std::wstring>(sc_arr) == std::wstring(wc_arr) );
    BOOST_CHECK(boost::lexical_cast<std::wstring>(uc_arr) == std::wstring(wc_arr) );

    BOOST_CHECK_EQUAL(boost::lexical_cast<wchar_t>(uc_arr[0]), wc_arr[0]);
    BOOST_CHECK_EQUAL(boost::lexical_cast<wchar_t>(sc_arr[0]), wc_arr[0]);
#endif
    BOOST_CHECK(1);
}

unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("lexical_cast char<->wchar_t unit test");
    suite->add(BOOST_TEST_CASE(&test_char_types_conversions));

    return suite;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/


class iw_test_tree_visitor : public boost::unit_test::test_tree_visitor 
{
public:
    virtual void visit( boost::unit_test::test_case const& tc) 
    { 
        try 
        {

            tc.test_func()(); 
        }
        catch (...)
        {
            BOOST_ERROR("Unhandled exception was thrown");
        }
    }
};

IW_TEST(lexical_cast_wchars_test)
{
    iw_test_tree_visitor v;
    boost::unit_test::test_suite* suite = lexical_cast_wchars_test_ns::init_unit_test_suite(0, NULL);
    const boost::unit_test::test_suite& s = *suite;
    boost::unit_test::traverse_test_tree(s, v);
    boost::unit_test::framework::clear();
    return S3E_TRUE;
}
