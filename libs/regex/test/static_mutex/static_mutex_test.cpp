/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         static_mutex_test.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: test program for boost::static_mutex.
  */

#include <iostream>
#include <boost/regex/pending/static_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/timer.hpp>
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


/*<boost.py:auto_insert>*/ namespace static_mutex_test_ns {/*<boost.py:auto_insert>*/


//
// we cannot use the regular Boost.Test in here: it is not thread safe
// and calls to BOOST_TEST will eventually crash on some compilers 
// (Borland certainly) due to race conditions inside the Boost.Test lib.
//
#define BOOST_TEST(pred) if(!(pred)) failed_test(__FILE__, __LINE__, BOOST_STRINGIZE(pred));

int total_failures = 0;
void failed_test(const char* file, int line, const char* pred)
{
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   boost::static_mutex::scoped_lock guard(mut);
   ++total_failures;
   std::cout << "Failed test in \"" << file << "\" at line " << line << ": " << pred << std::endl;
}

void print_cycles(int c)
{
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   boost::static_mutex::scoped_lock guard(mut);
   std::cout << "Thread exited after " << c << " cycles." << std::endl;
}

bool sufficient_time()
{
   // return true if enough time has passed since the tests began:
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   boost::static_mutex::scoped_lock guard(mut);
   static boost::timer t;
   // is 10 seconds enough?
   return t.elapsed() >= 10.0;
}

// define three trivial test proceedures:
bool t1()
{
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   static int has_lock = 0;
   static int data = 10000;

   boost::static_mutex::scoped_lock guard(mut);
   BOOST_TEST(++has_lock == 1);
   BOOST_TEST(guard.locked());
   BOOST_TEST(guard);
   bool result = (--data > 0) ? true : false;
   BOOST_TEST(--has_lock == 0);
   return result;
}

bool t2()
{
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   static int has_lock = 0;
   static int data = 10000;

   boost::static_mutex::scoped_lock guard(mut, false);
   BOOST_TEST(0 == guard.locked());
   BOOST_TEST(!guard);
   guard.lock();
   BOOST_TEST(++has_lock == 1);
   BOOST_TEST(guard.locked());
   BOOST_TEST(guard);
   bool result = (--data > 0) ? true : false;
   BOOST_TEST(--has_lock == 0);
   guard.unlock();
   BOOST_TEST(0 == guard.locked());
   BOOST_TEST(!guard);
   return result;
}

bool t3()
{
   static boost::static_mutex mut = BOOST_STATIC_MUTEX_INIT ;
   static int has_lock = 0;
   static int data = 10000;

   boost::static_mutex::scoped_lock guard(mut);
   BOOST_TEST(++has_lock == 1);
   BOOST_TEST(guard.locked());
   BOOST_TEST(guard);
   bool result = (--data > 0) ? true : false;
   BOOST_TEST(--has_lock == 0);
   return result;
}

// define their thread procs:
void thread1_proc()
{
   int cycles = 0;
   while(!sufficient_time())
   {
      t1();
      t2();
      ++cycles;
   }
   print_cycles(cycles);
}

void thread2_proc()
{
   int cycles = 0;
   while(!sufficient_time())
   {
      t2();
      t3();
      ++cycles;
   }
   print_cycles(cycles);
}

void thread3_proc()
{
   int cycles = 0;
   while(!sufficient_time())
   {
      t1();
      t3();
      ++cycles;
   }
   print_cycles(cycles);
}

// make sure that at least one of our test proceedures 
// is called during program startup:
struct startup1
{
   startup1()
   {
      t1();
   }
   ~startup1()
   {
      t1();
   }
};

startup1 up1;

int main()
{
   (void)up1;
   
   std::list<boost::shared_ptr<boost::thread> > threads;
   for(int i = 0; i < 2; ++i)
   {
      try{
         threads.push_back(boost::shared_ptr<boost::thread>(new boost::thread(&thread1_proc)));
      }
      catch(const std::exception& e)
      {
         std::cerr << "<note>Thread creation failed with message: " << e.what() << "</note>" << std::endl;
      }
   }
   for(int i = 0; i < 2; ++i)
   {
      try{
         threads.push_back(boost::shared_ptr<boost::thread>(new boost::thread(&thread2_proc)));
      }
      catch(const std::exception& e)
      {
         std::cerr << "<note>Thread creation failed with message: " << e.what() << "</note>" << std::endl;
      }
   }
   for(int i = 0; i < 2; ++i)
   {
      try{
         threads.push_back(boost::shared_ptr<boost::thread>(new boost::thread(&thread3_proc)));
      }
      catch(const std::exception& e)
      {
         std::cerr << "<note>Thread creation failed with message: " << e.what() << "</note>" << std::endl;
      }
   }

   std::list<boost::shared_ptr<boost::thread> >::const_iterator a(threads.begin()), b(threads.end());
   while(a != b)
   {
      (*a)->join();
      ++a;
   }

   return total_failures;
}

/*<boost.py:auto_insert>*/}/*<boost.py:auto_insert>*/

IW_TEST(static_mutex_test)
{
    static_mutex_test_ns::main();
    return S3E_TRUE;
}
