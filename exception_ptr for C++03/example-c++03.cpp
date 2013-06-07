#include "exception_ptr-lite.h"
#include <iostream>

int main()
{
    std11::exception_ptr p;

    try
    {
        throw std::runtime_error("runtime_error");
    }
    catch ( std::exception const & e )
    {
        std::cout << "original exception catched as std::exception: " << e.what() << std::endl;

        // e is base type
        p = std11::make_exception_ptr( e );

        // not available in exception_ptr lite:
        // p = std11::current_exception();
    }

    try
    {
        std11::rethrow_exception( p );
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

// MSVC 2012  -- cl -nologo -W4 -EHsc example-c++03.cpp && example-c++03
// MSVC 6     -- cl -nologo -W4 -EHsc -DSTD11_USE_AUTO_PTR example-c++03.cpp && example-c++03
// GNUC 4.8.0 -- g++ -Wall -Wextra -std=c++03 -I%BOOST_INCLUDE% -o example-c++03.exe example-c++03.cpp && example-c++03
// GNUC 4.8.0 -- g++ -Wall -Wextra -std=c++03 -DSTD11_USE_AUTO_PTR -o example-c++03.exe example-c++03.cpp && example-c++03

// Output:
//   original exception catched as std::exception: length_error
//   propagated exception catched as std::exception: std::exception
