
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_SCALAR_HPP_INCLUDED
#define BOOST_TT_IS_SCALAR_HPP_INCLUDED

#include "boost/type_traits/is_arithmetic.hpp"
#include "boost/type_traits/is_enum.hpp"
#include "boost/type_traits/is_pointer.hpp"
#include "boost/type_traits/is_member_pointer.hpp"
#include "boost/type_traits/detail/ice_or.hpp"
#include "boost/config.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

namespace detail {

template <typename T>
struct is_scalar_impl
{ 
   BOOST_STATIC_CONSTANT(bool, value =
      (::boost_cryray::type_traits::ice_or<
         ::boost_cryray::is_arithmetic<T>::value,
         ::boost_cryray::is_enum<T>::value,
         ::boost_cryray::is_pointer<T>::value,
         ::boost_cryray::is_member_pointer<T>::value
      >::value));
};

// these specializations are only really needed for compilers 
// without partial specialization support:
template <> struct is_scalar_impl<void>{ BOOST_STATIC_CONSTANT(bool, value = false ); };
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct is_scalar_impl<void const>{ BOOST_STATIC_CONSTANT(bool, value = false ); };
template <> struct is_scalar_impl<void volatile>{ BOOST_STATIC_CONSTANT(bool, value = false ); };
template <> struct is_scalar_impl<void const volatile>{ BOOST_STATIC_CONSTANT(bool, value = false ); };
#endif

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_scalar,T,::boost_cryray::detail::is_scalar_impl<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_SCALAR_HPP_INCLUDED
