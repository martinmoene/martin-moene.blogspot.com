#include <iostream>

template< typename T, typename U >
void foo( T const & t, U const & u )
{
    std::cout << "foo( T, U )" << std::endl;
}

template< typename T >
void foo( T const & t, int const & i )
{
    std::cout << "foo( T, int )" << std::endl;
}

int main()
{
    foo( 1,  2      );    // foo( T, int )
    foo( 1, "hello" );    // foo( T, U )

    return 0; // VC6
}

// cl -nologo -W3 -EHsc example1.cpp && example1
// g++ -Wall -std=c++03 -o example1.exe example1.cpp && example1
