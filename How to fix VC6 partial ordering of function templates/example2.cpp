#include <iostream>

struct Basic {};
struct Preferred : Basic {};

template< typename T, typename U >
void foo( T const & t, U const & u, Basic const & )
{
    std::cout << "foo( T, U )" << std::endl;
}

template< typename T >
void foo( T const & t, int const & i, Preferred const & )
{
    std::cout << "foo( T, int )" << std::endl;
}

template< typename T, typename U >
void bar( T const & t, U const & u )
{
    foo( t, u, Preferred() );
}

int main()
{
    foo( 1,  2     , Preferred() );    // foo( T, int )
    foo( 1, "hello", Preferred() );    // foo( T, U )

    bar( 1,  2      );    // foo( T, int )
    bar( 1, "hello" );    // foo( T, U )

    return 0; // VC6
}

// cl -nologo -W3 -EHsc example2.cpp && example2
// g++ -Wall -std=c++03 -o example2.exe example2.cpp && example2
