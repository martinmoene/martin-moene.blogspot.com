
#include "exception_ptr-lite.h"

/**
 * a user-defined exception.
 */
class my_exception : public virtual std11::cloneable
{
public:
    my_exception(const std::string& text) : text(text) {}

    ptr_type dynamic_clone() const
    {
        return ptr_type(new my_exception(*this));
    }

    void dynamic_throw() const
    {
        throw *this;
    }

    const std::string& what() const
    {
        return text;
    }

private:
    std::string text;
};

//------------------------------------------------------------------------

#include <iostream>

/**
 * rethrow a standard exception via an exception_ptr.
 */
int main()
{
    std11::exception_ptr p =
       std11::make_exception_ptr( std::invalid_argument("invalid argument") );

    try
    {
        std11::rethrow_exception( p );
    }
    catch ( std::exception const & e )
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }

    return 0; //VC6
}

// cl -nologo -W3 -EHsc -GR -I%CAMERA_ROOT%\CameraDll exception_ptr-c++03.cpp  && exception_ptr-c++03.exe
// cl -nologo -W3 -EHsc -GR -DSTD11_USE_AUTO_PTR exception_ptr-c++03.cpp  && exception_ptr-c++03.exe

// g++ -Wall -Wextra -I%BOOST_INCLUDE% -o exception_ptr-c++03.exe exception_ptr-c++03.cpp && exception_ptr-c++03
// g++ -Wall -Wextra -fpermissive -DSTD11_USE_AUTO_PTR  -o exception_ptr-c++03.exe exception_ptr-c++03.cpp && exception_ptr-c++03

