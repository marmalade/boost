#include <boost/config.hpp>

//  shared_ptr_alloc2_test.cpp
//
//  Copyright (c) 2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/detail/lightweight_test.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <cstddef>
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


/*<boost.py:auto_insert>*/ namespace shared_ptr_alloc2_test_ns {/*<boost.py:auto_insert>*/


// test_allocator

struct test_allocator_base
{
    int id_;

    static int last_global_id_;
    static int count_;

    explicit test_allocator_base( int id ): id_( id )
    {
    }
};

int test_allocator_base::last_global_id_ = 0;
int test_allocator_base::count_ = 0;

template<class T> class test_allocator: public test_allocator_base
{
public:

    typedef T * pointer;
    typedef T const * const_pointer;
    typedef T & reference;
    typedef T const & const_reference;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

private:

    static T * last_pointer_;
    static std::size_t last_n_;
    static int last_id_;

public:

    template<class U> struct rebind
    {
        typedef test_allocator<U> other;
    };

    pointer address( reference r ) const
    {
        return &r;
    }

    const_pointer address( const_reference s ) const
    {
        return &s;
    }

    explicit test_allocator( int id = 0 ): test_allocator_base( id )
    {
    }

    template<class U> test_allocator( test_allocator<U> const & r ): test_allocator_base( r )
    {
    }

    template<class U> test_allocator & operator=( test_allocator<U> const & r )
    {
        test_allocator_base::operator=( r );
        return *this;
    }

    void deallocate( pointer p, size_type n )
    {
        BOOST_TEST( p == last_pointer_ );
        BOOST_TEST( n == last_n_ );
        BOOST_TEST( id_ == last_id_ );

        --count_;

        ::operator delete( p );
    }

    pointer allocate( size_type n, void const * )
    {
        T * p = static_cast< T* >( ::operator new( n * sizeof( T ) ) );

        last_pointer_ = p;
        last_n_ = n;
        last_id_ = id_;

        last_global_id_ = id_;
        ++count_;

        return p;
    }

    void construct( pointer p, T const & t )
    {
        new( p ) T( t );
    }

    void destroy( pointer p )
    {
        p->~T();
    }

    size_type max_size() const
    {
        return size_type( -1 ) / sizeof( T );
    }
};

template<class T> T * test_allocator<T>::last_pointer_ = 0;
template<class T> std::size_t test_allocator<T>::last_n_ = 0;
template<class T> int test_allocator<T>::last_id_ = 0;

template<class T, class U> inline bool operator==( test_allocator<T> const & a1, test_allocator<U> const & a2 )
{
    return a1.id_ == a2.id_;
}

template<class T, class U> inline bool operator!=( test_allocator<T> const & a1, test_allocator<U> const & a2 )
{
    return a1.id_ != a2.id_;
}

template<> class test_allocator<void>: public test_allocator_base
{
public:

    typedef void * pointer;
    typedef void const * const_pointer;
    typedef void value_type;

    template<class U> struct rebind
    {
        typedef test_allocator<U> other;
    };

    explicit test_allocator( int id = 0 ): test_allocator_base( id )
    {
    }

    template<class U> test_allocator( test_allocator<U> const & r ): test_allocator_base( r )
    {
    }

    template<class U> test_allocator & operator=( test_allocator<U> const & r )
    {
        test_allocator_base::operator=( r );
        return *this;
    }
};

//

struct X
{
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

    X( X const & );
    X & operator=( X const & );
};

int X::instances = 0;

int main()
{
    BOOST_TEST( X::instances == 0 );

    boost::shared_ptr<void> pv( new X, boost::checked_deleter<X>(), std::allocator<X>() );

    BOOST_TEST( X::instances == 1 );

    pv.reset( new X, boost::checked_deleter<X>(), test_allocator<float>( 42 ) );

    BOOST_TEST( X::instances == 1 );

    BOOST_TEST( test_allocator_base::last_global_id_ == 42 );
    BOOST_TEST( test_allocator_base::count_ > 0 );

    pv.reset();

    BOOST_TEST( X::instances == 0 );
    BOOST_TEST( test_allocator_base::count_ == 0 );

    pv.reset( new X, boost::checked_deleter<X>(), test_allocator<void>( 43 ) );

    BOOST_TEST( X::instances == 1 );
    BOOST_TEST( test_allocator_base::last_global_id_ == 43 );

    pv.reset( new X, boost::checked_deleter<X>(), std::allocator<void>() );

    BOOST_TEST( X::instances == 1 );

    pv.reset();

    BOOST_TEST( X::instances == 0 );

    return boost::report_errors();
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(shared_ptr_alloc2_test)
{
    shared_ptr_alloc2_test_ns::main();
    return S3E_TRUE;
}
