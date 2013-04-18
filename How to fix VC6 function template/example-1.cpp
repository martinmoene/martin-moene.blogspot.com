#include <iostream>

enum Selector
{
   LessThan,
   GreaterThan,
};

template <Selector S> bool compare( int x, int y );

template <> bool compare<LessThan   >( int x, int y ) { return x < y; }
template <> bool compare<GreaterThan>( int x, int y ) { return x > y; }

int main()
{
    std::cout << compare<LessThan   >( 1, 2 ) << ", " <<
                 compare<GreaterThan>( 1, 2 ) << std::endl;
}

// cl -nologo -W3 -EHsc example-1.cpp && example-1.exe
// g++ -o example-1.exe example-1.cpp && example-1.exe
