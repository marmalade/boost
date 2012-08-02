#include <boost/config.hpp>

#if defined(BOOST_MSVC)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#endif

//  shared_ptr_mt_test.cpp - tests shared_ptr with multiple threads
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <vector>

#include <cstdio>
#include <ctime>

#include <boost/detail/lightweight_thread.hpp>
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


/*<boost.py:auto_insert>*/ namespace shared_ptr_mt_test_ns {/*<boost.py:auto_insert>*/


//

int const n = 1024 * 1024;

void test( boost::shared_ptr<int> const & pi )
{
    std::vector< boost::shared_ptr<int> > v;

    for( int i = 0; i < n; ++i )
    {
        v.push_back( pi );
    }
}

int const m = 16; // threads

#if defined( BOOST_HAS_PTHREADS )

char const * thmodel = "POSIX";

#else

char const * thmodel = "Windows";

#endif

int main()
{
    using namespace std; // printf, clock_t, clock

    printf( "Using %s threads: %d threads, %d iterations: ", thmodel, m, n );

    boost::shared_ptr<int> pi( new int(42) );

    clock_t t = clock();

    pthread_t a[ m ];

    for( int i = 0; i < m; ++i )
    {
        boost::detail::lw_thread_create( a[ i ], boost::bind( test, pi ) );
    }

    for( int j = 0; j < m; ++j )
    {
        pthread_join( a[j], 0 );
    }

    t = clock() - t;

    printf( "\n\n%.3f seconds.\n", static_cast<double>(t) / CLOCKS_PER_SEC );

    return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(shared_ptr_mt_test)
{
    shared_ptr_mt_test_ns::main();
    return S3E_TRUE;
}
