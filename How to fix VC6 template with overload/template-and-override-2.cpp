
#include <iostream>

struct S {};
struct Q {};

template <typename T>
struct X
{
   static T foo( T const & t )
   {
      return t;
   }

   static Q foo( S const & s )
   {
      return Q();
   }
};

std::ostream & operator<<( std::ostream & os, Q const & )
{
      return os << "struct Q";
}

int main()
{

   int k = 2;
   double f = 3.14;
   S s;

   std::cout << "foo( x ): " << X::foo( k ) << std::endl;
   std::cout << "foo( f ): " << X::foo( f ) << std::endl;
//   std::cout << "foo( s ): " << X::foo( s ) << std::endl;

   return 0;
}


// cl -nologo -EHsc template-and-override-2.cpp && template-and-override-2
