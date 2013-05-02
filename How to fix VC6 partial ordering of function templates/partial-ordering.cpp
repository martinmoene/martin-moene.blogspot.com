
// cl -nologo -W3 -EHsc partial-ordering.cpp && partial-ordering
// g++ -Wall -std=c++03 -o partial-ordering.exe partial-ordering.cpp && partial-ordering

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

struct S
{
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
};

int main()
{
    foo( 1,  2      );    // foo( T, int )
    foo( 1, "hello" );    // foo( T, U )

    S().foo( 1,  2      );    // foo( T, int )
    S().foo( 1, "hello" );    // foo( T, U )

    return 0; // VC6
}

/*
 * GCC:
 * prompt>g++ -Wall -o partial-ordering.exe partial-ordering.cpp && partial-ordering
 * foo( T, int )
 * foo( T, U )
 * foo( T, int )
 * foo( T, U )
 *
 * VC6:
 * prompt>cl -nologo -W3 -EHsc partial-ordering.cpp && partial-ordering
 * partial-ordering.cpp
 * partial-ordering.cpp(26) : error C2667: 'foo' : none of 2 overload have a best conversion
 * partial-ordering.cpp(26) : error C2668: 'foo' : ambiguous call to overloaded function
 */
