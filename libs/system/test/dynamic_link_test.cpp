//  dynamic_link_test.cpp  -------------------------------------------------------------//

//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See www.boost.org/LICENSE_1_0.txt

//  Library home page is www.boost.org/libs/system

//--------------------------------------------------------------------------------------// 

//  Dynamic link libraries (DLL's), also know as dynamic shared objects (DSO's),
//  can cause symbol visability problems unless carefully configured. One of the
//  manifestations, particularly with GCC, is that a system_error exception thrown from
//  a DLL or DSO is not caught.
//
//  The purpose of this program is to test for that error.

//--------------------------------------------------------------------------------------// 

#include <boost/system/system_error.hpp>

#include <iostream>

namespace boost
{
  namespace system
  {
    BOOST_SYSTEM_DECL void throw_test();
  }
}
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


/*<boost.py:auto_insert>*/ namespace dynamic_link_test_ns {/*<boost.py:auto_insert>*/


int main()
{
  try
  {
    boost::system::throw_test();
  }
  catch (const boost::system::system_error& ex)
  {
    std::cout << "  caught boost::system::system_error as expected\n";
    std::cout << "  what() reports " << ex.what() << '\n';
    return 0;
  }

  catch (const std::runtime_error& ex)
  {
    std::cout << "  error: caught std::runtime_error instead of boost::system::system_error\n";
    std::cout << "  what() reports " << ex.what() << '\n';
    return 1;
  }

  std::cout << "  error: failed to catch boost::system::system_error\n";
  return 1;
}
/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(dynamic_link_test)
{
    dynamic_link_test_ns::main();
    return S3E_TRUE;
}
