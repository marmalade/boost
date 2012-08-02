//
//  shared_ptr_alloc_test.cpp - use to evaluate the impact of count allocations
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/shared_ptr.hpp>
#include <boost/config.hpp>
#include <boost/detail/quick_allocator.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <cstddef>
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


/*<boost.py:auto_insert>*/ namespace shared_ptr_alloc_test_ns {/*<boost.py:auto_insert>*/


int const n = 1024 * 1024;

template<class T> void test(T * = 0)
{
    std::clock_t t = std::clock();
    std::clock_t t2;

    {
        std::vector< boost::shared_ptr<T> > v;

        for(int i = 0; i < n; ++i)
        {
            boost::shared_ptr<T> pi(new T(i));
            v.push_back(pi);
        }

        t2 = std::clock();
    }

    std::clock_t t3 = std::clock();

    std::cout << "   " << static_cast<double>(t3 - t) / CLOCKS_PER_SEC << " seconds, " << static_cast<double>(t2 - t) / CLOCKS_PER_SEC << " + " << static_cast<double>(t3 - t2) / CLOCKS_PER_SEC << ".\n";
}

class X
{
public:

    explicit X(int n): n_(n)
    {
    }

    void * operator new(std::size_t)
    {
        return std::allocator<X>().allocate(1, static_cast<X*>(0));
    }

    void operator delete(void * p)
    {
        std::allocator<X>().deallocate(static_cast<X*>(p), 1);
    }

private:

    X(X const &);
    X & operator=(X const &);

    int n_;
};

class Y
{
public:

    explicit Y(int n): n_(n)
    {
    }

    void * operator new(std::size_t n)
    {
        return boost::detail::quick_allocator<Y>::alloc(n);
    }

    void operator delete(void * p, std::size_t n)
    {
        boost::detail::quick_allocator<Y>::dealloc(p, n);
    }

private:

    Y(Y const &);
    Y & operator=(Y const &);

    int n_;
};

class Z: public Y
{
public:

    explicit Z(int n): Y(n), m_(n + 1)
    {
    }

private:

    Z(Z const &);
    Z & operator=(Z const &);

    int m_;
};

int main()
{
    std::cout << BOOST_COMPILER "\n";
    std::cout << BOOST_PLATFORM "\n";
    std::cout << BOOST_STDLIB "\n";

#if defined(BOOST_HAS_THREADS)
    std::cout << "BOOST_HAS_THREADS: (defined)\n";
#else
    std::cout << "BOOST_HAS_THREADS: (not defined)\n";
#endif

#if defined(BOOST_SP_USE_STD_ALLOCATOR)
    std::cout << "BOOST_SP_USE_STD_ALLOCATOR: (defined)\n";
#else
    std::cout << "BOOST_SP_USE_STD_ALLOCATOR: (not defined)\n";
#endif

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)
    std::cout << "BOOST_SP_USE_QUICK_ALLOCATOR: (defined)\n";
#else
    std::cout << "BOOST_SP_USE_QUICK_ALLOCATOR: (not defined)\n";
#endif

#if defined(BOOST_QA_PAGE_SIZE)
    std::cout << "BOOST_QA_PAGE_SIZE: " << BOOST_QA_PAGE_SIZE << "\n";
#else
    std::cout << "BOOST_QA_PAGE_SIZE: (not defined)\n";
#endif

    std::cout << n << " shared_ptr<int> allocations + deallocations:\n";

    test<int>();
    test<int>();
    test<int>();

    std::cout << n << " shared_ptr<X> allocations + deallocations:\n";

    test<X>();
    test<X>();
    test<X>();

    std::cout << n << " shared_ptr<Y> allocations + deallocations:\n";

    test<Y>();
    test<Y>();
    test<Y>();

    std::cout << n << " shared_ptr<Z> allocations + deallocations:\n";

    test<Z>();
    test<Z>();
    test<Z>();
    return 0;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(shared_ptr_alloc_test)
{
    shared_ptr_alloc_test_ns::main();
    return S3E_TRUE;
}
