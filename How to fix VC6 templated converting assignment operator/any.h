/**
 * \file core/any.h
 *
 * \brief   any from Valued Conversions, C++ Report July/August 2000.
 * \author  Kevlin Henney
 * \date    12 June 2012
 * \since   0.0.0
 *
 * http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf
 * Some adaptations from Boost.Any 1.36
 */

#ifndef G_SPM_CORE_ANY_H_INCLUDED
#define G_SPM_CORE_ANY_H_INCLUDED

#include <algorithm>
#include <sstream>
#include <typeinfo>

#include <iostream>

/**
 * namespace spm
 */
namespace spm {

/*
 * any value.
 */
class any
{
public:
   any()
   : content(0)
   {
   }

   ~any()
   {
      delete content;
   }

   bool empty() const
   {
      return !content;
   }

   const std::type_info & type() const
   {
      return content ? content->type() : typeid(void);
   }

   /*
    * Inward conversions and helpers for a generalized union type.
    */

   // int = 0 helps VC6 to distinguish copy-assignment and converting constructors.

   any( any const & other, int = 0 )
   : content( other.content ? other.content->clone() : 0 )
   {
   }

   template<typename value_type>
   any( value_type const & value )
   : content( new holder<value_type>( value ) )
   {
   }

   any & swap( any & rhs )
   {
      std::swap( content, rhs.content );
      return *this;
   }

   // Boost.Any 1.43
   any & operator=( any rhs )
   {
      rhs.swap( *this );
      return *this;
   }

   // Boost.Any 1.43
   template<typename value_type>
   any & operator=( value_type const & rhs )
   {
      any( rhs ).swap( *this );
      return *this;
   }

   /*
    * Functions to extract the value from the generalized union type.
    */

   operator const void *() const
   {
      return content;
   }

   template<typename value_type>
   bool copy_to( value_type & value ) const
   {
      const value_type * copyable = to_ptr<value_type>();

      if( copyable )
         value = *copyable;

      return copyable;
   }

   // added value_type argument for VC6, see any_cast.
   template<typename value_type>
   const value_type *to_ptr( value_type ) const
   {
      return type() == typeid(value_type) ? &static_cast<holder<value_type> *>( content )->held : 0;
   }

private:
   class placeholder
   {
   public:
      virtual ~placeholder()
      {
      }

      virtual std::type_info const & type() const = 0;

      virtual placeholder * clone() const = 0;
   };

   template<typename value_type>
   class holder : public placeholder
   {
   public:
      holder( value_type const & value )
      : held( value )
      {
      }

      virtual std::type_info const & type() const
      {
         return typeid( value_type );
      }

      virtual placeholder * clone() const
      {
         return new holder( held );
      }

      const value_type held;
   };

   placeholder * content;
};

template<typename result_type, typename arg_type>
result_type interpret_cast( arg_type const & arg )
{
   std::stringstream interpreter;
   interpreter << arg;

   result_type result = result_type();
   interpreter >> result;

   return result;
}

class bad_any_cast : public std::bad_cast
{
public:
   virtual const char * what() const throw()
   {
      return "spm::bad_any_cast: "
             "failed conversion using spm::any_cast";
   }
};

template<typename value_type>
value_type any_cast( any const & operand)
{
   const value_type * result = operand.to_ptr( value_type() );

   if ( !result )
   {
      throw bad_any_cast();
   }

   return *result;
}

} // namespace spm

#endif // G_SPM_CORE_ANY_H_INCLUDED

/*
 * end of file
 */
