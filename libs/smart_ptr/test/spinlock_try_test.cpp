//
// spinlock_try_test.cpp
//
// Copyright 2008 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/detail/lightweight_test.hpp>
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


/*<boost.py:auto_insert>*/ namespace spinlock_try_test_ns {/*<boost.py:auto_insert>*/


// Sanity check only

static boost::detail::spinlock sp = BOOST_DETAIL_SPINLOCK_INIT;
static boost::detail::spinlock sp2 = BOOST_DETAIL_SPINLOCK_INIT;

int main()
{
    BOOST_TEST( sp.try_lock() );
    BOOST_TEST( !sp.try_lock() );
    BOOST_TEST( sp2.try_lock() );
    BOOST_TEST( !sp.try_lock() );
    BOOST_TEST( !sp2.try_lock() );
    sp.unlock();
    sp2.unlock();

    sp.lock();
    BOOST_TEST( !sp.try_lock() );
    sp2.lock();
    BOOST_TEST( !sp.try_lock() );
    BOOST_TEST( !sp2.try_lock() );
    sp.unlock();
    sp2.unlock();

    {
        boost::detail::spinlock::scoped_lock lock( sp );
        BOOST_TEST( !sp.try_lock() );
        boost::detail::spinlock::scoped_lock lock2( sp2 );
        BOOST_TEST( !sp.try_lock() );
        BOOST_TEST( !sp2.try_lock() );
    }

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(spinlock_try_test)
{
    spinlock_try_test_ns::main();
    return S3E_TRUE;
}
