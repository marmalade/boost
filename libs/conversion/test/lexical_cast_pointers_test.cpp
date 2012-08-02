//  Unit test for boost::lexical_cast.
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Antony Polukhin, 2012.
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


/*<boost.py:auto_insert>*/ namespace lexical_cast_pointers_test_ns {/*<boost.py:auto_insert>*/


using namespace boost;

#if defined(BOOST_NO_STRINGSTREAM)
            typedef std::strstream ss_t;
#else
            typedef std::stringstream ss_t;
#endif

void test_void_pointers_conversions()
{
    void *p_to_null = NULL;
    const void *cp_to_data = "Some data";
    char nonconst_data[5];
    void *p_to_data = nonconst_data;
    ss_t ss;

    ss << p_to_null;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p_to_null), ss.str());
    ss.str(std::string());

    ss << cp_to_data;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cp_to_data), ss.str());
    ss.str(std::string());

    ss << p_to_data;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p_to_data), ss.str());
    ss.str(std::string());
}

struct incomplete_type;

void test_incomplete_type_pointers_conversions()
{
    incomplete_type *p_to_null = NULL;
    const incomplete_type *cp_to_data = NULL;
    char nonconst_data[5];
    incomplete_type *p_to_data = reinterpret_cast<incomplete_type*>(nonconst_data);
    ss_t ss;

    ss << p_to_null;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p_to_null), ss.str());
    ss.str(std::string());

    ss << cp_to_data;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cp_to_data), ss.str());
    ss.str(std::string());

    ss << p_to_data;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p_to_data), ss.str());
    ss.str(std::string());
}

struct ble;
typedef struct ble *meh;
std::ostream& operator <<(std::ostream &o, meh) {
  o << "yay";
  return o;
}

void test_inomplete_type_with_overloaded_ostream_op() {
    meh heh = NULL;
    ss_t ss;
    ss << heh;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(heh), ss.str());
}

unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("lexical_cast pinters test");
    suite->add(BOOST_TEST_CASE(&test_void_pointers_conversions));
    suite->add(BOOST_TEST_CASE(&test_incomplete_type_pointers_conversions));
    suite->add(BOOST_TEST_CASE(&test_inomplete_type_with_overloaded_ostream_op));
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

IW_TEST(lexical_cast_pointers_test)
{
    iw_test_tree_visitor v;
    boost::unit_test::test_suite* suite = lexical_cast_pointers_test_ns::init_unit_test_suite(0, NULL);
    const boost::unit_test::test_suite& s = *suite;
    boost::unit_test::traverse_test_tree(s, v);
    boost::unit_test::framework::clear();
    return S3E_TRUE;
}
