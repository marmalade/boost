//  Unit test for boost::lexical_cast.
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Alexander Nasonov, 2006.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).
//
//  Test round-tripping conversion FPT -> string -> FPT,
//  where FPT is Floating Point Type.

#include <boost/config.hpp>

#if defined(__INTEL_COMPILER)
#pragma warning(disable: 193 383 488 981 1418 1419)
#elif defined(BOOST_MSVC)
#pragma warning(disable: 4097 4100 4121 4127 4146 4244 4245 4511 4512 4701 4800)
#endif

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
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


/*<boost.py:auto_insert>*/ namespace lexical_cast_loopback_test_ns {/*<boost.py:auto_insert>*/


using namespace boost;

void test_round_conversion_float();
void test_round_conversion_double();
void test_round_conversion_long_double();

unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("lexical_cast unit test");
    suite->add(BOOST_TEST_CASE(&test_round_conversion_float));
    suite->add(BOOST_TEST_CASE(&test_round_conversion_double));
    suite->add(BOOST_TEST_CASE(&test_round_conversion_long_double));

    return suite;
}

template<class T>
void test_round_conversion()
{
    T epsilon = std::numeric_limits<T>::epsilon();
    std::string const epsilon_s = boost::lexical_cast<std::string>(epsilon);
    BOOST_CHECK(epsilon == lexical_cast<T>(epsilon_s));

    T max_ = (std::numeric_limits<T>::max)();
    std::string const max_s = boost::lexical_cast<std::string>(max_);
    BOOST_CHECK(max_ == lexical_cast<T>(max_s));

    T min_ = (std::numeric_limits<T>::min)();
    std::string const min_s = boost::lexical_cast<std::string>(min_);
    BOOST_CHECK(min_ == lexical_cast<T>(min_s));

    T max_div137 = max_ / 137;
    std::string max_div137_s = boost::lexical_cast<std::string>(max_div137);
    BOOST_CHECK(max_div137 == lexical_cast<T>(max_div137_s));

    T epsilon_mult137 = epsilon * 137;
    std::string epsilon_mult137_s(lexical_cast<std::string>(epsilon_mult137));
    BOOST_CHECK(epsilon_mult137 == lexical_cast<T>(epsilon_mult137_s));

}

// See bug http://tinyurl.com/vhpvo
template<class T>
void test_msvc_magic_values()
{
    T magic_msvc = 0.00010000433948393407;
    std::string magic_msvc_s = boost::lexical_cast<std::string>(magic_msvc);
    BOOST_CHECK(magic_msvc == lexical_cast<T>(magic_msvc_s));
}

void test_round_conversion_float()
{
    test_round_conversion<float>();
}

void test_round_conversion_double()
{
    test_round_conversion<double>();
    test_msvc_magic_values<double>();
}

void test_round_conversion_long_double()
{
    test_round_conversion<long double>();
    test_msvc_magic_values<long double>();
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

IW_TEST(lexical_cast_loopback_test)
{
    iw_test_tree_visitor v;
    boost::unit_test::test_suite* suite = lexical_cast_loopback_test_ns::init_unit_test_suite(0, NULL);
    const boost::unit_test::test_suite& s = *suite;
    boost::unit_test::traverse_test_tree(s, v);
    boost::unit_test::framework::clear();
    return S3E_TRUE;
}
