/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE:        recursion_test.cpp
  *   VERSION:     see <boost/version.hpp>
  *   DESCRIPTION: Test for indefinite recursion and/or stack overrun.
  */

#include <string>
#include <boost/regex.hpp>
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


/*<boost.py:auto_insert>*/ namespace recursion_test_ns {/*<boost.py:auto_insert>*/


#ifdef BOOST_INTEL
#pragma warning(disable:1418 981 983 383)
#endif

int test_main( int , char* [] )
{
   // this regex will recurse twice for each whitespace character matched:
   boost::regex e("([[:space:]]|.)+");

   std::string bad_text(1024*1024*4, ' ');
   std::string good_text(200, ' ');

   boost::smatch what;

   //
   // Over and over: We want to make sure that after a stack error has
   // been triggered, that we can still conduct a good search and that
   // subsequent stack failures still do the right thing:
   //
   BOOST_CHECK(boost::regex_search(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_search(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_search(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_search(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_search(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_search(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_search(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_search(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_search(good_text, what, e));

   BOOST_CHECK(boost::regex_match(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_match(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_match(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_match(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_match(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_match(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_match(good_text, what, e));
   BOOST_CHECK_THROW(boost::regex_match(bad_text, what, e), std::runtime_error);
   BOOST_CHECK(boost::regex_match(good_text, what, e));

   return 0;
}

//#include <boost/test/included/test_exec_monitor.hpp>

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(recursion_test)
{
    recursion_test_ns::test_main(0, NULL);
    return S3E_TRUE;
}
