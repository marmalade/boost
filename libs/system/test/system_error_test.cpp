//  system_error_test.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------// 

//  test without deprecated features
#define BOOST_SYSTEM_NO_DEPRECATED

#include <boost/config/warning_disable.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <boost/system/system_error.hpp>
#include <iostream>
#include <string>

#ifdef BOOST_WINDOWS_API
#include <windows.h>
#endif
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


/*<boost.py:auto_insert>*/ namespace system_error_test_ns {/*<boost.py:auto_insert>*/


using boost::system::system_error;
using boost::system::error_code;
using boost::system::system_category;
using std::string;

#define TEST(x,v,w) test(#x,x,v,w)

namespace
{
  void test( const char * desc, const system_error & ex,
    int v, const char * str )
  {
    std::cout << "test " << desc << "\n what() returns \"" << ex.what() << "\"\n";
    BOOST_TEST( ex.code().value() == v );
    BOOST_TEST( ex.code().category() == system_category() );
# ifdef BOOST_WINDOWS_API
    LANGID language_id;
#   if !defined(__MINGW32__) && !defined(__CYGWIN__)
      language_id = ::GetUserDefaultUILanguage();
#   else
      language_id = 0x0409; // Assume US English
#   endif
    // std::cout << "GetUserDefaultUILanguage() returns " << language_id << '\n';
    if ( language_id == 0x0409 )  // English (United States)
    {
      BOOST_TEST( std::string( ex.what() ) == str );
      if ( std::string( ex.what() ) != str )
        std::cout << "expected \"" << str << "\", but what() returned \""
          << ex.what() << "\"\n";
    }
# endif
  }

  const boost::uint_least32_t uvalue = 2u;
}

int main( int, char *[] )
{
  // all constructors, in the same order as they appear in the header:

  system_error c1_0( error_code(0, system_category()) ); 
  system_error c1_1( error_code(1, system_category()) );
  system_error c1_2u( error_code(uvalue, system_category()) );

  system_error c2_0( error_code(0, system_category()), string("c2_0") ); 
  system_error c2_1( error_code(1, system_category()), string("c2_1") );

  system_error c3_0( error_code(0, system_category()), "c3_0" ); 
  system_error c3_1( error_code(1, system_category()), "c3_1" );

  system_error c4_0( 0, system_category() ); 
  system_error c4_1( 1, system_category() );
  system_error c4_2u( uvalue, system_category() );

  system_error c5_0( 0, system_category(), string("c5_0") ); 
  system_error c5_1( 1, system_category(), string("c5_1") );

  system_error c6_0( 0, system_category(), "c6_0" ); 
  system_error c6_1( 1, system_category(), "c6_1" );

  TEST( c1_0, 0, "The operation completed successfully" );
  TEST( c1_1, 1, "Incorrect function" );
  TEST( c1_2u, 2, "The system cannot find the file specified" );

  TEST( c2_0, 0, "c2_0: The operation completed successfully" );
  TEST( c2_1, 1, "c2_1: Incorrect function" );

  TEST( c3_0, 0, "c3_0: The operation completed successfully" );
  TEST( c3_1, 1, "c3_1: Incorrect function" );

  TEST( c4_0, 0, "The operation completed successfully" );
  TEST( c4_1, 1, "Incorrect function" );
  TEST( c4_2u, 2, "The system cannot find the file specified" );

  TEST( c5_0, 0, "c5_0: The operation completed successfully" );
  TEST( c5_1, 1, "c5_1: Incorrect function" );

  TEST( c6_0, 0, "c6_0: The operation completed successfully" );
  TEST( c6_1, 1, "c6_1: Incorrect function" );

  return ::boost::report_errors();
}



/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(system_error_test)
{
    system_error_test_ns::main(0, NULL);
    return S3E_TRUE;
}
