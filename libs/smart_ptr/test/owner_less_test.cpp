//
//  owner_less_test.cpp
//
//  A regression test for owner_less
//
//  Copyright (c) 2008 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/detail/lightweight_test.hpp>
#include <boost/smart_ptr/owner_less.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
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


/*<boost.py:auto_insert>*/ namespace owner_less_test_ns {/*<boost.py:auto_insert>*/


int main()
{
  boost::owner_less<boost::shared_ptr<int> > comp;
  {
    boost::shared_ptr<int> x;
    boost::shared_ptr<int> y;
    boost::weak_ptr<int> w;
    BOOST_TEST(!(comp(x, w) || comp(w, x)));
  }
  {
    boost::shared_ptr<int> z((int*)0);
    boost::weak_ptr<int> w;
    BOOST_TEST(comp(z, w) || comp(w, z));
    {
      boost::shared_ptr<int> zz(z);
      w = boost::weak_ptr<int>(zz);
      BOOST_TEST(!(comp(z, zz) || comp(z, zz)));
      BOOST_TEST(!(comp(z, w) || comp(z, w)));
    }
    BOOST_TEST(!(comp(z, w) || comp(w, z)));
  }
  {
    boost::shared_ptr<int> x;
    boost::shared_ptr<int> z((int*)0);
    BOOST_TEST(comp(x, z) || comp(z, x));
  }
  {
    boost::shared_ptr<int> a((int*)0);
    boost::shared_ptr<int> b((int*)0);
    BOOST_TEST(comp(a, b) || comp(b, a));
    boost::weak_ptr<int> w(a);
    BOOST_TEST(!(comp(a, w) || comp(w, a)));
    BOOST_TEST(comp(b, w) || comp(w, b));
  }

  boost::owner_less<boost::weak_ptr<int> > weak_comp;
  {
    boost::shared_ptr<int> a((int*)0);
    boost::weak_ptr<int> wa(a);
    boost::shared_ptr<int> b((int*)0);
    boost::weak_ptr<int> wb(b);
    BOOST_TEST(!(weak_comp(a, wa) || weak_comp(wa, a)));
    BOOST_TEST(!(weak_comp(b, wb) || weak_comp(wb, b)));
    BOOST_TEST(weak_comp(wa, wb) || weak_comp(wb, wa));
    BOOST_TEST(weak_comp(wa, b) || weak_comp(b, wa));
  }

  return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(owner_less_test)
{
    owner_less_test_ns::main();
    return S3E_TRUE;
}
