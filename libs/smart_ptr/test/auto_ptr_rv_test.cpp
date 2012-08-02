//
//  auto_ptr_rv_test.cpp
//
//  Copyright (c) 2006 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/shared_ptr.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <memory>
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


/*<boost.py:auto_insert>*/ namespace auto_ptr_rv_test_ns {/*<boost.py:auto_insert>*/


struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

    static std::auto_ptr<X> create()
    {
        return std::auto_ptr<X>( new X );
    }

private:

    X( X const & );
    X & operator=( X const & );
};

long X::instances = 0;

int main()
{
    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<X> p( X::create() );
        BOOST_TEST( X::instances == 1 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );

        p.reset();
        BOOST_TEST( X::instances == 0 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<X const> p( X::create() );
        BOOST_TEST( X::instances == 1 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );

        p.reset();
        BOOST_TEST( X::instances == 0 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<void> p( X::create() );
        BOOST_TEST( X::instances == 1 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );

        p.reset();
        BOOST_TEST( X::instances == 0 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<void const> p( X::create() );
        BOOST_TEST( X::instances == 1 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );

        p.reset();
        BOOST_TEST( X::instances == 0 );

        p = X::create();
        BOOST_TEST( X::instances == 1 );
    }

    BOOST_TEST( X::instances == 0 );

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(auto_ptr_rv_test)
{
    auto_ptr_rv_test_ns::main();
    return S3E_TRUE;
}
