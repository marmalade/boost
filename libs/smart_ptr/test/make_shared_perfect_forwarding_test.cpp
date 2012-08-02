// make_shared_perfect_forwarding_test.cpp - a test of make_shared
//   perfect forwarding of constructor arguments when using a C++0x
//   compiler.
//
// Copyright 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>
#include <boost/make_shared.hpp>
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


/*<boost.py:auto_insert>*/ namespace make_shared_perfect_forwarding_test_ns {/*<boost.py:auto_insert>*/


#if !defined( BOOST_HAS_RVALUE_REFS )

int main()
{
    return 0;
}

#else // BOOST_HAS_RVALUE_REFS

class myarg
{
public:
    myarg()
    {}
private:
    myarg(myarg && other)
    {}
    myarg& operator=(myarg && other)
    {
        return *this;
    }
    myarg(const myarg & other)
    {}
    myarg& operator=(const myarg & other)
    {
        return *this;
    }
};

class X
{
public:
    enum constructor_id
    {
        move_constructor,
        const_ref_constructor,
        ref_constructor
    };

    X(myarg &&arg): constructed_by_(move_constructor)
    {}
    X(const myarg &arg): constructed_by_(const_ref_constructor)
    {}
    X(myarg &arg): constructed_by_(ref_constructor)
    {}

    constructor_id constructed_by_;
};

struct Y
{
    Y(int &value): ref(value)
    {}
    int &ref;
};

int main()
{
    {
        myarg a;
        boost::shared_ptr< X > x = boost::make_shared< X >(a);
        BOOST_TEST( x->constructed_by_ == X::ref_constructor);
    }
    {
        const myarg ca;
        boost::shared_ptr< X > x = boost::make_shared< X >(ca);
        BOOST_TEST( x->constructed_by_ == X::const_ref_constructor);
    }
    {
        boost::shared_ptr< X > x = boost::make_shared< X >(myarg());
        BOOST_TEST( x->constructed_by_ == X::move_constructor);
    }
    {
        int value = 1;
        boost::shared_ptr< Y > y = boost::make_shared< Y >(value);
        BOOST_TEST( y->ref == 1 && value == y->ref );
        ++y->ref;
        BOOST_TEST( value == y->ref );
    }

    return boost::report_errors();
}

#endif // BOOST_HAS_RVALUE_REFS

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(make_shared_perfect_forwarding_test)
{
    make_shared_perfect_forwarding_test_ns::main();
    return S3E_TRUE;
}
