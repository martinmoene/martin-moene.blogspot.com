
// cl -nologo -W3 -EHsc partial-ordering-sln.cpp && partial-ordering-sln
// g++ -Wall -std=c++03 -o partial-ordering-sln.exe partial-ordering-sln.cpp && partial-ordering-sln

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

struct S
{
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
};

int main()
{
    foo( 1,  2     , Preferred() );    // foo( T, int )
    foo( 1, "hello", Preferred() );    // foo( T, U )

    bar( 1,  2      );    // foo( T, int )
    bar( 1, "hello" );    // foo( T, U )

    S().foo( 1,  2     , Preferred() );    // foo( T, int )
    S().foo( 1, "hello", Preferred() );    // foo( T, U )

    S().bar( 1,  2      );    // foo( T, int )
    S().bar( 1, "hello" );    // foo( T, U )

    return 0; // VC6
}

/*
 * GCC:
 * prompt>g++ -Wall -o partial-ordering-sln.exe partial-ordering-sln.cpp && partial-ordering-sln
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 *
 * VC6:
 * prompt>cl -nologo -W3 -EHsc partial-ordering-sln.cpp && partial-ordering-sln partial-ordering-sln.cpp
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 */
