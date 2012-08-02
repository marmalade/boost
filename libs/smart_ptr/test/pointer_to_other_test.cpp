//
//  pointer_to_other_test.cpp - a test for boost/pointer_to_other.hpp
//
//  Copyright (c) 2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/pointer_to_other.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/intrusive_ptr.hpp>

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


/*<boost.py:auto_insert>*/ namespace pointer_to_other_test_ns {/*<boost.py:auto_insert>*/


template<class T, class U> void assert_same_type( T** pt = 0, U** pu = 0 )
{
    pt = pu;
}

struct X;
struct Y;

int main()
{
    // shared_ptr

    assert_same_type< boost::pointer_to_other< boost::shared_ptr<X>, Y >::type, boost::shared_ptr<Y> >();
    assert_same_type< boost::pointer_to_other< boost::shared_ptr<X>, void >::type, boost::shared_ptr<void> >();
    assert_same_type< boost::pointer_to_other< boost::shared_ptr<void>, Y >::type, boost::shared_ptr<Y> >();

    // shared_array

    assert_same_type< boost::pointer_to_other< boost::shared_array<X>, Y >::type, boost::shared_array<Y> >();
    assert_same_type< boost::pointer_to_other< boost::shared_array<X>, void >::type, boost::shared_array<void> >();
    assert_same_type< boost::pointer_to_other< boost::shared_array<void>, Y >::type, boost::shared_array<Y> >();

    // scoped_ptr

    assert_same_type< boost::pointer_to_other< boost::scoped_ptr<X>, Y >::type, boost::scoped_ptr<Y> >();
    assert_same_type< boost::pointer_to_other< boost::scoped_ptr<X>, void >::type, boost::scoped_ptr<void> >();
    assert_same_type< boost::pointer_to_other< boost::scoped_ptr<void>, Y >::type, boost::scoped_ptr<Y> >();

    // scoped_array

    assert_same_type< boost::pointer_to_other< boost::scoped_array<X>, Y >::type, boost::scoped_array<Y> >();
    assert_same_type< boost::pointer_to_other< boost::scoped_array<X>, void >::type, boost::scoped_array<void> >();
    assert_same_type< boost::pointer_to_other< boost::scoped_array<void>, Y >::type, boost::scoped_array<Y> >();

    // intrusive_ptr

    assert_same_type< boost::pointer_to_other< boost::intrusive_ptr<X>, Y >::type, boost::intrusive_ptr<Y> >();
    assert_same_type< boost::pointer_to_other< boost::intrusive_ptr<X>, void >::type, boost::intrusive_ptr<void> >();
    assert_same_type< boost::pointer_to_other< boost::intrusive_ptr<void>, Y >::type, boost::intrusive_ptr<Y> >();

    // auto_ptr

    assert_same_type< boost::pointer_to_other< std::auto_ptr<X>, Y >::type, std::auto_ptr<Y> >();
    assert_same_type< boost::pointer_to_other< std::auto_ptr<X>, void >::type, std::auto_ptr<void> >();
    assert_same_type< boost::pointer_to_other< std::auto_ptr<void>, Y >::type, std::auto_ptr<Y> >();

    // raw pointer
   
    assert_same_type< boost::pointer_to_other< X *, Y >::type, Y * >();
    assert_same_type< boost::pointer_to_other< X *, void >::type, void * >();
    assert_same_type< boost::pointer_to_other< void *, Y >::type, Y * >();

    return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(pointer_to_other_test)
{
    pointer_to_other_test_ns::main();
    return S3E_TRUE;
}
