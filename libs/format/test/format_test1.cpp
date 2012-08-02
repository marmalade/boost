// ------------------------------------------------------------------------------
// libs/format/test/format_test1.cpp :  test constructing objects and basic parsing
// ------------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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


/*<boost.py:auto_insert>*/ namespace format_test1_ns {/*<boost.py:auto_insert>*/



int test_main(int, char* [])
{

  using boost::format;
  using boost::str;

  if(str( format("  %%  ") ) != "  %  ")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("nothing") ) != "nothing")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("%%  ") ) != "%  ")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("  %%") ) != "  %")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("  %n  ") ) != "    ")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("%n  ") ) != "  ")
      BOOST_ERROR("Basic parsing without arguments Failed");
  if(str( format("  %n") ) != "  ")
      BOOST_ERROR("Basic parsing without arguments Failed");

  if(str( format("%%##%%##%%1 %1%00") % "Escaped OK" ) != "%##%##%1 Escaped OK00")
      BOOST_ERROR("Basic parsing Failed");
  if(str( format("%%##%#x ##%%1 %s00") % 20 % "Escaped OK" ) != "%##0x14 ##%1 Escaped OK00")
      BOOST_ERROR("Basic p-parsing Failed") ;

  return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(format_test1)
{
    format_test1_ns::test_main(0, NULL);
    return S3E_TRUE;
}
