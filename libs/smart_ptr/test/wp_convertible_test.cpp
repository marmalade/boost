#include <boost/config.hpp>

//  wp_convertible_test.cpp
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
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


/*<boost.py:auto_insert>*/ namespace wp_convertible_test_ns {/*<boost.py:auto_insert>*/


//

class incomplete;

struct X
{
};

struct Y
{
};

struct Z: public X
{
};

int f( boost::weak_ptr<void const> )
{
    return 1;
}

int f( boost::weak_ptr<int> )
{
    return 2;
}

int f( boost::weak_ptr<incomplete> )
{
    return 3;
}

int g( boost::weak_ptr<X> )
{
    return 4;
}

int g( boost::weak_ptr<Y> )
{
    return 5;
}

int g( boost::weak_ptr<incomplete> )
{
    return 6;
}

int main()
{
    BOOST_TEST( 1 == f( boost::weak_ptr<double>() ) );
    BOOST_TEST( 1 == f( boost::shared_ptr<double>() ) );
    BOOST_TEST( 4 == g( boost::weak_ptr<Z>() ) );
    BOOST_TEST( 4 == g( boost::shared_ptr<Z>() ) );

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(wp_convertible_test)
{
    wp_convertible_test_ns::main();
    return S3E_TRUE;
}
