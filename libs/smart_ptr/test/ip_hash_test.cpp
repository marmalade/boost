//
// ip_hash_test.cpp
//
// Copyright 2011 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/intrusive_ptr.hpp>
#include <boost/functional/hash.hpp>
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


/*<boost.py:auto_insert>*/ namespace ip_hash_test_ns {/*<boost.py:auto_insert>*/


class base
{
private:

    int use_count_;

    base(base const &);
    base & operator=(base const &);

protected:

    base(): use_count_(0)
    {
    }

    virtual ~base()
    {
    }

public:

    long use_count() const
    {
        return use_count_;
    }

    inline friend void intrusive_ptr_add_ref(base * p)
    {
        ++p->use_count_;
    }

    inline friend void intrusive_ptr_release(base * p)
    {
        if(--p->use_count_ == 0) delete p;
    }
};

struct X: public base
{
};

int main()
{
    boost::hash< boost::intrusive_ptr<X> > hasher;

    boost::intrusive_ptr<X> p1, p2( p1 ), p3( new X ), p4( p3 ), p5( new X );

    BOOST_TEST_EQ( p1, p2 );
    BOOST_TEST_EQ( hasher( p1 ), hasher( p2 ) );

    BOOST_TEST_NE( p1, p3 );
    BOOST_TEST_NE( hasher( p1 ), hasher( p3 ) );

    BOOST_TEST_EQ( p3, p4 );
    BOOST_TEST_EQ( hasher( p3 ), hasher( p4 ) );

    BOOST_TEST_NE( p3, p5 );
    BOOST_TEST_NE( hasher( p3 ), hasher( p5 ) );

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(ip_hash_test)
{
    ip_hash_test_ns::main();
    return S3E_TRUE;
}
