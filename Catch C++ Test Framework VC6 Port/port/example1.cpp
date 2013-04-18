enum Selector { LessThan, GreaterThan, };

// Function compare<>()

template <Selector S> bool compare( int x, int y );

template <> bool compare<LessThan   >( int x, int y ) { return x < y; }
template <> bool compare<GreaterThan>( int x, int y ) { return x > y; }

// class Comparator<>{}

template <Selector S> struct Comparator { static bool compare( int x, int y ); };

template <> bool Comparator<LessThan   >::compare( int x, int y ) { return x < y; }
template <> bool Comparator<GreaterThan>::compare( int x, int y ) { return x > y; }

// Tests

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "templated function",
           "fails to differentiate on template parameter in VC6" )
{
    CHECK( compare<LessThan   >( 1, 2 ) );
    CHECK( compare<GreaterThan>( 2, 1 ) );
    CHECK( true == compare<GreaterThan>( 2, 1 ) );
}

TEST_CASE( "templated class",
           "successfully differentiates on template parameter in VC6" )
{
    REQUIRE( Comparator<LessThan   >::compare( 1, 2 ) );
    REQUIRE( Comparator<GreaterThan>::compare( 2, 1 ) );
}

// cl -nologo -W3 -EHsc example1.cpp && example1.exe
// g++ -Wall -o example1.exe example1.cpp && example1.exe

/* prompt>cl -nologo -W3 -EHsc example1.cpp && example1.exe
 * example1.cpp
 * [Started testing: example1]
 * [Test case: 'templated function']
 * example1.cpp(34):
 * true == compare<GreaterThan>( 2, 1 ) failed for: true == false
 * [Summary for test case 'templated function': 1 test case - failed (2 assertions - 1 failed)]
 *
 * [Summary for group '': 2 test cases - 1 failed (4 assertions - 1 failed)]
 *
 * [Summary for 'example1': 2 test cases - 1 failed (4 assertions - 1 failed)]
 */

