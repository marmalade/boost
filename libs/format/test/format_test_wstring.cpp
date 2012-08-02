// ------------------------------------------------------------------------------
//  format_test_wstring.cpp :  test wchar_t format use (if supported)
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


/*<boost.py:auto_insert>*/ namespace format_test_wstring_ns {/*<boost.py:auto_insert>*/



int test_main(int, char* [])
{

  using boost::format;
  using boost::str;

#if !defined(BOOST_NO_STD_WSTRING) && !defined(BOOST_NO_STD_WSTREAMBUF)
  using boost::wformat;
  wformat wfmter(L"%%##%%##%%1 %1%00");
  if(str( wfmter % L"Escaped OK" ) != L"%##%##%1 Escaped OK00")
      BOOST_ERROR("Basic w-parsing Failed");
  if(str( wformat(L"%%##%#x ##%%1 %s00") % 20 % L"Escaped OK" ) != L"%##0x14 ##%1 Escaped OK00")
      BOOST_ERROR("Basic wp-parsing Failed") ;
#endif // wformat tests


  return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(format_test_wstring)
{
    format_test_wstring_ns::test_main(0, NULL);
    return S3E_TRUE;
}
