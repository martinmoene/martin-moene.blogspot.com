#include <cassert>
#include <exception>
#include <stdexcept>
#include <iostream>

int main()
{
    std::exception_ptr p =
       std::make_exception_ptr( std::invalid_argument("invalid argument") );

    try
    {
        std::rethrow_exception( p );
    }
    catch ( std::exception const & e )
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }
}

// g++ -Wall -Wextra -std=c++11 -o example1.exe example1.cpp && example1
