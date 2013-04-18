template <typename T>
T foo( T t ) { return t; }

double foo( int i ) { return 3.14 * i; }

#include <iostream>

int main()
{
   std::cout << "foo( 2    ): " << foo( 2    ) << std::endl;
   std::cout << "foo( 3.14 ): " << foo( 3.14 ) << std::endl;

   return 0; // VC6
}
// cl -nologo -W3 -EHsc example2.cpp && example2.exe
// g++ -o example1 example2.cpp && example2
