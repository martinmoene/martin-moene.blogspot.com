
#include <cassert>
#include <iostream>

#define Given( context ) \
    for ( int section = 0, count = 1; section < count; count -= 0==section++ )

#define When( action ) WhenThen( action )
#define Then( result ) WhenThen( result )

#define WhenThen( text ) \
    static int UNIQUE( id ) = 0; \
    if ( run now = run( UNIQUE( id ), section, count ) ) \
        for ( int section = 0, count = 1; section < count; count -= 0==section++ )

#define UNIQUE3( name, line ) name##line
#define UNIQUE2( name, line ) UNIQUE3( name, line )
#define UNIQUE(  name ) UNIQUE2( name, __LINE__ )

struct run
{
    int & id;
    int const & section;

    run( int & id, int const & section, int & count )
    : id( id ), section( section )
    {
        if ( section == 0 )
            id = count++ - 1;
    }
    operator bool() { return id == section; }
};

#define Case(...) int main()

Case( "x is afresh each when( action )" )
{
    Given( "x as fixture" ) {
        int x = 42;

        When( "x is changed" )
        {
            x = 7;
        }
        When( "x is used again" )
        {
            Then( "x has been reinitialised" )
            {
                assert( x == 42 );
            }
        }
    }
}

// cl -nologo -W3 -EHsc -I.. Block-scoped-fixtures-in-C++.cpp && Block-scoped-fixtures-in-C++
// g++ -Wall -Wextra -std=c++11 -I.. -o Block-scoped-fixtures-in-C++.exe Block-scoped-fixtures-in-C++.cpp && Block-scoped-fixtures-in-C++
