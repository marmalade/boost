//
//  pointer_cast_test.cpp - a test for boost/pointer_cast.hpp
//
//  Copyright (c) 2005 Ion Gaztanaga
//  Copyright (c) 2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>

#include <boost/pointer_cast.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/get_pointer.hpp>

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


/*<boost.py:auto_insert>*/ namespace pointer_cast_test_ns {/*<boost.py:auto_insert>*/


namespace
{

// Let's create these inheritance relationship:
//
//    base   base2
//      |     |
//      derived
//         |
//  derived_derived
//

class base
{
   public:
   virtual ~base(){}
   int filler [5];
};

class base2
{
public:

    virtual ~base2(){}
    int filler [5];
};

class derived
   : public base, public base2 
{
    int filler [5];
};

class derived_derived
   : public derived
{
    int filler [5];
};

// And now some simple check functions

#if !defined( BOOST_NO_RTTI )

template <class BasePtr>
bool check_dynamic_pointer_cast(const BasePtr &ptr)
{
   //Check that dynamic_pointer_cast versus dynamic_cast
   return
   //Correct cast with dynamic_pointer_cast
   boost::get_pointer(boost::dynamic_pointer_cast<derived>(ptr)) ==
      //Correct cast with dynamic_cast
      dynamic_cast<derived*>(boost::get_pointer(ptr)) 
   &&
   //Incorrect cast with dynamic_pointer_cast
   boost::get_pointer(boost::dynamic_pointer_cast<derived_derived>(ptr)) ==
      //Incorrect cast with dynamic_cast
      dynamic_cast<derived_derived*>(boost::get_pointer(ptr));
}

#endif

template <class BasePtr>
bool check_static_pointer_cast(const BasePtr &ptr)
{
   return
   //Cast base -> derived -> base2 using static_pointer_cast
   boost::get_pointer(
            boost::static_pointer_cast<base2>(
               boost::static_pointer_cast<derived>(ptr))) ==
   //Now the same with static_cast
   static_cast<base2*>(static_cast<derived*>(boost::get_pointer(ptr)));
}

template <class BasePtr>
bool check_const_pointer_cast(const BasePtr &ptr)
{
   return   
   //Unconst and const again using const_pointer_cast
   boost::get_pointer(
      boost::const_pointer_cast<const base>
         (boost::const_pointer_cast<base>(ptr))) ==
   //Now the same with const_cast
   const_cast<const base*>(const_cast<base*>(boost::get_pointer(ptr)));
}

}

int main()
{
    {
        // Try casts with shared_ptr

        boost::shared_ptr<base> ptr(new derived);

#if !defined( BOOST_NO_RTTI )
        BOOST_TEST( check_dynamic_pointer_cast( ptr ) );
#endif
        BOOST_TEST( check_static_pointer_cast( ptr ) );
        BOOST_TEST( check_const_pointer_cast( ptr ) );
    }
   
    {
        // Try casts with raw pointer

        boost::scoped_ptr<base> ptr(new derived);

#if !defined( BOOST_NO_RTTI )
        BOOST_TEST( check_dynamic_pointer_cast( ptr.get() ) );
#endif
        BOOST_TEST( check_static_pointer_cast( ptr.get() ) );
        BOOST_TEST( check_const_pointer_cast( ptr.get() ) );
    }
    
    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(pointer_cast_test)
{
    pointer_cast_test_ns::main();
    return S3E_TRUE;
}
