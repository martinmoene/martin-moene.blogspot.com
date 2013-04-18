#include <iostream>
#include <stdexcept>
#include <string>

struct any {};

struct bad_any_cast : public std::runtime_error
{
    bad_any_cast(std::string const msg)
    : std::runtime_error("bad any cast: " + msg) {}
};

template<typename value_type>
value_type any_cast( any const & operand )
{
    if ( typeid(value_type) != typeid(double) )
    {
        throw bad_any_cast( typeid(value_type).name() );
    }

   return 3.14;
}

int main()
{
    try
    {
        double const d = any_cast<double>( any() );
        double const e = any_cast<long  >( any() );
    }
    catch( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// cl -nologo -W3 -EHsc example-1.cpp && example-1.exe
// g++ -o example-1.exe example-1.cpp && example-1.exe
