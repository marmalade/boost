#include <boost/config.hpp>

//  sp_atomic_test.cpp
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
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


/*<boost.py:auto_insert>*/ namespace sp_atomic_test_ns {/*<boost.py:auto_insert>*/


//

struct X
{
};

#define BOOST_TEST_SP_EQ( p, q ) BOOST_TEST( p == q && !( p < q ) && !( q < p ) )

int main()
{
    boost::shared_ptr<X> px( new X );

    {
        boost::shared_ptr<X> p2 = boost::atomic_load( &px );
        BOOST_TEST_SP_EQ( p2, px );

        boost::shared_ptr<X> px2( new X );
        boost::atomic_store( &px, px2 );
        BOOST_TEST_SP_EQ( px, px2 );

        p2 = boost::atomic_load( &px );
        BOOST_TEST_SP_EQ( p2, px );
        BOOST_TEST_SP_EQ( p2, px2 );

        boost::shared_ptr<X> px3( new X );
        boost::shared_ptr<X> p3 = boost::atomic_exchange( &px, px3 );
        BOOST_TEST_SP_EQ( p3, px2 );
        BOOST_TEST_SP_EQ( px, px3 );

        boost::shared_ptr<X> px4( new X );
        boost::shared_ptr<X> cmp;

        bool r = boost::atomic_compare_exchange( &px, &cmp, px4 );
        BOOST_TEST( !r );
        BOOST_TEST_SP_EQ( px, px3 );
        BOOST_TEST_SP_EQ( cmp, px3 );

        r = boost::atomic_compare_exchange( &px, &cmp, px4 );
        BOOST_TEST( r );
        BOOST_TEST_SP_EQ( px, px4 );
    }

    //

    px.reset();

    {
        boost::shared_ptr<X> p2 = boost::atomic_load_explicit( &px, boost::memory_order_acquire );
        BOOST_TEST_SP_EQ( p2, px );

        boost::shared_ptr<X> px2( new X );
        boost::atomic_store_explicit( &px, px2, boost::memory_order_release );
        BOOST_TEST_SP_EQ( px, px2 );

        boost::shared_ptr<X> p3 = boost::atomic_exchange_explicit( &px, boost::shared_ptr<X>(), boost::memory_order_acq_rel );
        BOOST_TEST_SP_EQ( p3, px2 );
        BOOST_TEST_SP_EQ( px, p2 );

        boost::shared_ptr<X> px4( new X );
        boost::shared_ptr<X> cmp( px2 );

        bool r = boost::atomic_compare_exchange_explicit( &px, &cmp, px4, boost::memory_order_acquire, boost::memory_order_relaxed );
        BOOST_TEST( !r );
        BOOST_TEST_SP_EQ( px, p2 );
        BOOST_TEST_SP_EQ( cmp, p2 );

        r = boost::atomic_compare_exchange_explicit( &px, &cmp, px4, boost::memory_order_release, boost::memory_order_acquire );
        BOOST_TEST( r );
        BOOST_TEST_SP_EQ( px, px4 );
    }

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(sp_atomic_test)
{
    sp_atomic_test_ns::main();
    return S3E_TRUE;
}
