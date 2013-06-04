/*
 * exception_ptr, make_exception_ptr() and rethrow_exception() for C++03.
 *
 * Created on 30 May 2013 by Martin Moene.
 */

#include <cassert>
#include <stdexcept>
#include <string>
#include <typeinfo>

// GNUC complains with std::auto_ptr<> because copy-constructor
// moves from const argument (auto_ptr<>)
//#define STD11_USE_AUTO_PTR

#ifdef STD11_USE_AUTO_PTR
# include <memory>
#else
# if ( _MSC_VER >= 1200 ) && ( _MSC_VER < 1300 )
   // VC6 correction for Boost 1.51 boost/shared_ptr.hpp:
#  include <cstdlib>
   namespace std { using ::abort; }
# endif
# include <boost/shared_ptr.hpp>
#endif

#ifdef STD11_USE_AUTO_PTR
#define STD11_SMART_PTR(T) std::auto_ptr<T>
#else
#define STD11_SMART_PTR(T) boost::shared_ptr<T>
#endif

namespace std11
{

/**
 * interface for exceptions to be cloned.
 *
 * This is modelled after the following proposal:
 *   Beman Dawes. 5 May 2007.
 *   Cloning and Throwing Dynamically Typed Exceptions (Rev 1)
 *   (Library-only support for exception propagation from threads)
 *   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2229.html
 */
class cloneable
{
public:
    typedef STD11_SMART_PTR(cloneable) ptr_type;
    virtual ptr_type      dynamic_clone() const = 0;
    virtual void          dynamic_throw() const = 0;
    virtual              ~cloneable() {}
};

/**
 * exception_ptr type.
 */
typedef cloneable::ptr_type exception_ptr;

#define STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( E )                            \
    class E : public virtual cloneable                                      \
    {                                                                       \
    public:                                                                 \
        ~E() throw() {}                                                     \
                                                                            \
        E( std::E const & e ) : ex( new std::E(e) ) {}                      \
                                                                            \
        E( E const & other ) : ex( other.ex ) {}                            \
                                                                            \
        ptr_type dynamic_clone() const { return ptr_type( new E(*this) ); } \
                                                                            \
        void dynamic_throw() const { throw *ex; }                           \
                                                                            \
    private:                                                                \
        STD11_SMART_PTR(std::E) ex;                                         \
    };

STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( exception )

STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( logic_error  )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( invalid_argument )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( domain_error )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( length_error )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( out_of_range )
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( future_error )  // C++11
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_optional_access ) //(C++14)

STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( runtime_error )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( range_error )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( overflow_error )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( underflow_error )
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( system_error )  // (C++11)
// NTS: macro doesn't support ::
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( ios_base::failure ) // (derived from runtime_error since C++11 )

STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_typeid )
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_cast )
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_weak_ptr ) // (C++11)
//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_function_call ) // (C++11)
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_alloc )

//STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_array_new_length ) // (C++11)
STD11_DEFINE_DYNAMIC_EXCEPTION_TYPE( bad_exception )

/**
 * default conversion shim:
 */
inline cloneable const &
make_cloneable( cloneable const & e )
{
    return e;
}

/**
 * define conversion shims for a standard exception:
 */
#define STD11_DEFINE_MAKE_CLONEABLE(E)  \
    inline E                            \
    make_cloneable( std::E const & e )  \
    {                                   \
        return E( e );                  \
    }

STD11_DEFINE_MAKE_CLONEABLE( exception )

STD11_DEFINE_MAKE_CLONEABLE( logic_error )
STD11_DEFINE_MAKE_CLONEABLE( invalid_argument )
STD11_DEFINE_MAKE_CLONEABLE( domain_error )
STD11_DEFINE_MAKE_CLONEABLE( length_error )
STD11_DEFINE_MAKE_CLONEABLE( out_of_range )
//STD11_DEFINE_MAKE_CLONEABLE( future_error ) // C++11
//STD11_DEFINE_MAKE_CLONEABLE( bad_optional_access ) //(C++14)

STD11_DEFINE_MAKE_CLONEABLE( runtime_error )
STD11_DEFINE_MAKE_CLONEABLE( range_error )
STD11_DEFINE_MAKE_CLONEABLE( overflow_error )
STD11_DEFINE_MAKE_CLONEABLE( underflow_error )
//STD11_DEFINE_MAKE_CLONEABLE( system_error )  // (C++11)
// NTS: macro doesn't support ::
//STD11_DEFINE_MAKE_CLONEABLE( ios_base::failure ) // (derived from runtime_error since C++11)

STD11_DEFINE_MAKE_CLONEABLE( bad_typeid )
STD11_DEFINE_MAKE_CLONEABLE( bad_cast )
//STD11_DEFINE_MAKE_CLONEABLE( bad_weak_ptr ) // (C++11)
//STD11_DEFINE_MAKE_CLONEABLE( bad_function_call ) // (C++11)
STD11_DEFINE_MAKE_CLONEABLE( bad_alloc )
//STD11_DEFINE_MAKE_CLONEABLE( bad_array_new_length ) // (C++11)
STD11_DEFINE_MAKE_CLONEABLE( bad_exception )

/**
 * make_exception_ptr given a standard or user-defined exception.
 */
template <typename E>
inline exception_ptr
make_exception_ptr( E const & e )
{
    try
    {
        throw make_cloneable( e );
    }
    catch ( cloneable const & ex )
    {
        return ex.dynamic_clone();
    }

    return exception_ptr();
}

/**
 * rethrow_exception given in p.
 */
inline void
rethrow_exception( exception_ptr p )
{
    assert( p.get() != NULL );
    p->dynamic_throw();
}

/**
 * current_exception is not supported.
 */
inline exception_ptr
current_exception();

} // namespace std11

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

//    std11::exception_ptr q = p;
//     // Error: p doesn't contain the exception anymore:

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

// cl -nologo -W3 -EHsc -GR -ID:\People\Martin\00_IP\SpmDev\camera4\CameraDll exception_ptr-c++03.cpp  && exception_ptr-c++03.exe
// cl -nologo -W3 -EHsc -GR -DSTD11_USE_AUTO_PTR -ID:\People\Martin\00_IP\SpmDev\camera4\CameraDll exception_ptr-c++03.cpp  && exception_ptr-c++03.exe

// g++ -Wall -Wextra -ID:\People\Martin\00_IP\SpmDev\Vendor\Boost -o exception_ptr-c++03.exe exception_ptr-c++03.cpp && exception_ptr-c++03
// g++ -Wall -Wextra -fpermissive -DSTD11_USE_AUTO_PTR -ID:\People\Martin\00_IP\SpmDev\Vendor\Boost -o exception_ptr-c++03.exe exception_ptr-c++03.cpp && exception_ptr-c++03

