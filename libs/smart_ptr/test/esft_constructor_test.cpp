//
//  esft_constructor_test.cpp
//
//  A test for the new enable_shared_from_this support for calling
//  shared_from_this from constructors (that is, prior to the
//  object's ownership being passed to an external shared_ptr).
//
//  Copyright (c) 2008 Frank Mori Hess
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/smart_ptr/enable_shared_from_this2.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
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


/*<boost.py:auto_insert>*/ namespace esft_constructor_test_ns {/*<boost.py:auto_insert>*/


class X: public boost::enable_shared_from_this2< X >
{
private:

    int destroyed_;
    int deleted_;
    int expected_;

private:

    X( X const& );
    X& operator=( X const& );

public:

    static int instances;

public:

    explicit X( int expected, boost::shared_ptr<X> *early_px = 0 ): destroyed_( 0 ), deleted_( 0 ), expected_( expected )
    {
        ++instances;
        if( early_px ) *early_px = shared_from_this();
    }

    ~X()
    {
        BOOST_TEST( deleted_ == expected_ );
        BOOST_TEST( destroyed_ == 0 );
        ++destroyed_;
        --instances;
    }

    typedef void (*deleter_type)( X* );

    static void deleter( X * px )
    {
        ++px->deleted_;
    }

    static void deleter2( X * px )
    {
        ++px->deleted_;
        delete px;
    }
};

int X::instances = 0;

template<typename T, typename U>
bool are_shared_owners(const boost::shared_ptr<T> &a, const boost::shared_ptr<U> &b)
{
    return !(a < b) && !(b < a);
}

struct Y: public boost::enable_shared_from_this2<Y>
{};

int main()
{
    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<X> early_px;
        X* x = new X( 1, &early_px );
        BOOST_TEST( early_px.use_count() > 0 );
        BOOST_TEST( boost::get_deleter<X::deleter_type>(early_px) == 0 );
        boost::shared_ptr<X> px( x, &X::deleter2 );
        BOOST_TEST( early_px.use_count() == 2 && px.use_count() == 2 );
        BOOST_TEST(are_shared_owners(early_px, px));
        px.reset();
        BOOST_TEST( early_px.use_count() == 1 );
        BOOST_TEST( X::instances == 1 );
        // X::deleter_type *pd = boost::get_deleter<X::deleter_type>(early_px);
        // BOOST_TEST(pd && *pd == &X::deleter2 );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<X> early_px;
        X* x = new X( 1, &early_px );
        boost::weak_ptr<X> early_weak_px = early_px;
        early_px.reset();
        BOOST_TEST( !early_weak_px.expired() );
        boost::shared_ptr<X> px( x, &X::deleter2 );
        BOOST_TEST( px.use_count() == 1 );
        BOOST_TEST( X::instances == 1 );
        BOOST_TEST(are_shared_owners(early_weak_px.lock(), px));
        px.reset();
        BOOST_TEST( early_weak_px.expired() );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<X> early_px;
        X x( 1, &early_px );
        BOOST_TEST( early_px.use_count() > 0 );
        boost::shared_ptr<X> px( &x, &X::deleter );
        BOOST_TEST( early_px.use_count() == 2 && px.use_count() == 2 );
        early_px.reset();
        BOOST_TEST( px.use_count() == 1 );
        BOOST_TEST( X::instances == 1 );
        px.reset();
        try
        {
            x.shared_from_this();
            BOOST_ERROR("x did not throw bad_weak_ptr");
        }
        catch( const boost::bad_weak_ptr & )
        {}
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::weak_ptr<X> early_weak_px;
        {
            boost::shared_ptr<X> early_px;
            X x( 0, &early_px );
            early_weak_px = early_px;
            early_px.reset();
            BOOST_TEST( !early_weak_px.expired() );
            BOOST_TEST( X::instances == 1 );
        }
        BOOST_TEST( early_weak_px.expired() );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr<Y> px(new Y());
        Y y(*px);
        px.reset();
        try
        {
            y.shared_from_this();
        }
        catch( const boost::bad_weak_ptr & )
        {
            BOOST_ERROR("y threw bad_weak_ptr");
        }
    }
    
    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(esft_constructor_test)
{
    esft_constructor_test_ns::main();
    return S3E_TRUE;
}
