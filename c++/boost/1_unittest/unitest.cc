#define BOOST_TEST_MODULE My Test 
#include <boost/test/included/unit_test.hpp> 
#include <iostream>
#include <regex>
 
using namespace std;
 
bool isOdd(int x) {
	return x % 2;
}
 
// ------------Unit Test------------------
  
BOOST_AUTO_TEST_CASE( case0 ) {
  	BOOST_TEST( isOdd(3) );
	BOOST_TEST( !isOdd(2) );
}
/*  		 
#include <variant>   // support by c++ 17
*/

BOOST_AUTO_TEST_CASE(first_test) 
{
  int i = 1;
  BOOST_TEST(i); 
  BOOST_TEST(i == 2); 
}
