//  Unit test for boost::lexical_cast.
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Alexander Nasonov, 2007.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).
//
// This tests now must pass on vc8, because lexical_cast
// implementation has changed and it does not use stringstream for casts
// to integral types

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/test/unit_test.hpp>

#include <string>
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


/*<boost.py:auto_insert>*/ namespace lexical_cast_vc8_bug_test_ns {/*<boost.py:auto_insert>*/


using namespace boost;

// See also test_conversion_from_string_to_integral(CharT)
// in libs/conversion/lexical_cast_test.cpp
template<class T, class CharT>
void test_too_long_number(CharT zero)
{
    typedef std::numeric_limits<T> limits;

    std::basic_string<CharT> s;

    std::basic_ostringstream<CharT> o;
    o << (limits::max)() << zero;
    s = o.str();
    BOOST_CHECK_THROW(lexical_cast<T>(s), bad_lexical_cast);
    s[s.size()-1] += 9; // '0' -> '9'
    BOOST_CHECK_THROW(lexical_cast<T>(s), bad_lexical_cast);

    if(limits::is_signed)
    {
        std::basic_ostringstream<CharT> o;
        o << (limits::min)() << zero;
        s = o.str();
        BOOST_CHECK_THROW(lexical_cast<T>(s), bad_lexical_cast);
        s[s.size()-1] += 9; // '0' -> '9'
        BOOST_CHECK_THROW(lexical_cast<T>(s), bad_lexical_cast);
    }
}

void test_vc8_bug()
{
    test_too_long_number<boost::intmax_t>('0');
    test_too_long_number<boost::uintmax_t>('0');
#if !defined(BOOST_LCAST_NO_WCHAR_T)
    test_too_long_number<boost::intmax_t>(L'0');
    test_too_long_number<boost::uintmax_t>(L'0');
#endif
}

unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("lexical_cast vc8 bug unit test");
    suite->add(BOOST_TEST_CASE(test_vc8_bug));
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

IW_TEST(lexical_cast_vc8_bug_test)
{
    iw_test_tree_visitor v;
    boost::unit_test::test_suite* suite = lexical_cast_vc8_bug_test_ns::init_unit_test_suite(0, NULL);
    const boost::unit_test::test_suite& s = *suite;
    boost::unit_test::traverse_test_tree(s, v);
    boost::unit_test::framework::clear();
    return S3E_TRUE;
}
