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
#include "catch_reporter_compact.hpp"

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

// cl -nologo -W3 -EHsc example1.cpp && example1.exe --reporter compact
// g++ -Wall -o example1.exe example1.cpp && example1.exe --reporter compact
