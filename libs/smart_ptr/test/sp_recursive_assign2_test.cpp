//
//  sp_recursive_assign2_test.cpp
//
//  Copyright 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//


#include <boost/shared_ptr.hpp>
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


/*<boost.py:auto_insert>*/ namespace sp_recursive_assign2_test_ns {/*<boost.py:auto_insert>*/


//

class X
{
public:

    static int instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X( X const& );
};

int X::instances = 0;

class Y
{
public:

    static int instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y( Y const& );
};

int Y::instances = 0;

static boost::shared_ptr<void> s_pv;

class Z
{
public:

    static int instances;

    Z()
    {
        ++instances;
    }

    ~Z()
    {
        --instances;

        boost::shared_ptr<Y> pv( new Y );
        s_pv = pv;
    }

private:

    Z( Z const& );
};

int Z::instances = 0;

int main()
{
    BOOST_TEST( X::instances == 0 );
    BOOST_TEST( Y::instances == 0 );
    BOOST_TEST( Z::instances == 0 );

    {
        boost::shared_ptr<Z> pv( new Z );
        s_pv = pv;
    }

    BOOST_TEST( X::instances == 0 );
    BOOST_TEST( Y::instances == 0 );
    BOOST_TEST( Z::instances == 1 );

    {
        boost::shared_ptr<X> pv( new X );
        s_pv = pv;
    }

    BOOST_TEST( X::instances == 0 );
    BOOST_TEST( Y::instances == 1 );
    BOOST_TEST( Z::instances == 0 );

    s_pv.reset();

    BOOST_TEST( X::instances == 0 );
    BOOST_TEST( Y::instances == 0 );
    BOOST_TEST( Z::instances == 0 );

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(sp_recursive_assign2_test)
{
    sp_recursive_assign2_test_ns::main();
    return S3E_TRUE;
}
