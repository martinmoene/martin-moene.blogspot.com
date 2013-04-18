#include <iostream>

enum Selector
{
   LessThan,
   GreaterThan,
};

template <Selector S>
struct Comparator
{
    static bool compare( int x, int y );
};

template <> bool Comparator<LessThan   >::compare( int x, int y ) { return x < y; }
template <> bool Comparator<GreaterThan>::compare( int x, int y ) { return x > y; }

int main()
{
    std::cout << Comparator<LessThan   >::compare( 1, 2 ) << ", " <<
                 Comparator<GreaterThan>::compare( 1, 2 ) << std::endl;
}

// cl -nologo -W3 -EHsc example-2.cpp && example-2.exe
// g++ -o example-2.exe example-2.cpp && example-2.exe
