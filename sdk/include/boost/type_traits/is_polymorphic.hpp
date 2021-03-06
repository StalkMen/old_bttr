//  (C) Copyright John Maddock 2000. Permission to copy, use, modify, sell and   
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_TT_IS_POLYMORPHIC_HPP
#define BOOST_TT_IS_POLYMORPHIC_HPP

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/remove_cv.hpp>
// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"
#include <boost/detail/workaround.hpp>

namespace boost_cryray{
namespace detail{

template <class T>
struct is_polymorphic_imp1
{
# if BOOST_WORKAROUND(__MWERKS__, <= 0x2407) // CWPro7 should return false always.
    typedef char d1, (&d2)[2];
# else 
   typedef typename remove_cv<T>::type ncvT;
   struct d1 : public ncvT
   {
      d1();
#  if !defined(__GNUC__) // this raises warnings with some classes, and buys nothing with GCC
      ~d1()throw();
#  endif 
      char padding[256];
   };
   struct d2 : public ncvT
   {
      d2();
      virtual ~d2()throw();
#  ifndef BOOST_MSVC
      // for some reason this messes up VC++ when T has virtual bases:
      virtual void foo();
#  endif
      char padding[256];
   };
# endif 
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(d2) == sizeof(d1)));
};

template <class T>
struct is_polymorphic_imp2
{
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template <bool is_class>
struct is_polymorphic_selector
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp2<T> type;
   };
};

template <>
struct is_polymorphic_selector<true>
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp1<T> type;
   };
};

template <class T>
struct is_polymorphic_imp
{
   typedef is_polymorphic_selector< ::boost_cryray::is_class<T>::value> selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type imp_type;
   BOOST_STATIC_CONSTANT(bool, value = imp_type::value);
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_polymorphic,T,::boost_cryray::detail::is_polymorphic_imp<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif
