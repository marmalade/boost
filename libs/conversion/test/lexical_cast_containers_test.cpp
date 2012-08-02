//  Testing boost::lexical_cast with boost::container::string.
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Antony Polukhin, 2011.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/container/string.hpp>
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


/*<boost.py:auto_insert>*/ namespace lexical_cast_containers_test_ns {/*<boost.py:auto_insert>*/


void testing_boost_containers_basic_string();

using namespace boost;

boost::unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("Testing boost::lexical_cast with boost::container::string");
    suite->add(BOOST_TEST_CASE(testing_boost_containers_basic_string));

    return suite;
}

void testing_boost_containers_basic_string()
{       
    BOOST_CHECK("100" == lexical_cast<boost::container::string>("100"));
    BOOST_CHECK(L"100" == lexical_cast<boost::container::wstring>(L"100"));

    BOOST_CHECK("100" == lexical_cast<boost::container::string>(100));
    boost::container::string str("1000");
    BOOST_CHECK(1000 == lexical_cast<int>(str));
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

IW_TEST(lexical_cast_containers_test)
{
    iw_test_tree_visitor v;
    boost::unit_test::test_suite* suite = lexical_cast_containers_test_ns::init_unit_test_suite(0, NULL);
    const boost::unit_test::test_suite& s = *suite;
    boost::unit_test::traverse_test_tree(s, v);
    boost::unit_test::framework::clear();
    return S3E_TRUE;
}
