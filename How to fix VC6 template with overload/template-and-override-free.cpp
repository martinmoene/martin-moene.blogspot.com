
#include <iostream>

//namespace vc6_detail {

   template <typename T>
   T foo( T t )
   {
      return t;
   }

//} // namespace a
//
//using namespace vc6_detail;

struct S {};
struct Q {};

Q foo( S s )
{
   return Q();
}

std::ostream & operator<<( std::ostream & os, Q const & )
{
   return os << "struct Q";
}

int main()
{
   std::cout <<
      "foo( 2    ): " << foo( 2    ) << std::endl <<
      "foo( 3.14 ): " << foo( 3.14 ) << std::endl <<
      "foo( S()  ): " << foo( S()  ) << std::endl;

   return 0;
}


// cl -nologo -EHsc template-and-override-free.cpp && template-and-override-free
