#include <cassert>
#include <exception>
#include <stdexcept>
#include <iostream>

int main()
{
    std::exception_ptr p;

    try
    {
        throw std::runtime_error("runtime_error");
    }
    catch ( std::exception const & e )
    {
        std::cout << "original exception catched as std::exception: " << e.what() << std::endl;

        // e is base type
        // p = std::make_exception_ptr( e );

        p = std::current_exception();
    }

    try
    {
        std::rethrow_exception( p );
    }
    catch ( std::runtime_error const & e )
    {
        // p propagated original type
        std::cout << "propagated exception catched as std::runtime_error : " << e.what() << std::endl;
    }
    catch ( std::exception const & e )
    {
        // p propagated base type
        std::cout << "propagated exception catched as std::exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }
}

// MSVC 2012  -- cl -nologo -W4 -EHsc example-c++11.cpp && example-c++11
// GNUC 4.8.0 -- g++ -Wall -Wextra -std=c++11 -o example-c++11.exe example-c++11.cpp && example-c++11

// Output:
//   original exception catched as std::exception: runtime_error
//   propagated exception catched as std::runtime_error : runtime_error
