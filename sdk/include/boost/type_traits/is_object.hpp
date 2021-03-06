
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_OBJECT_HPP_INCLUDED
#define BOOST_TT_IS_OBJECT_HPP_INCLUDED

#include "boost/type_traits/is_reference.hpp"
#include "boost/type_traits/is_void.hpp"
#include "boost/type_traits/is_function.hpp"
#include "boost/type_traits/detail/ice_and.hpp"
#include "boost/type_traits/detail/ice_not.hpp"
#include "boost/config.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

namespace detail {

template <typename T>
struct is_object_impl
{
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
   BOOST_STATIC_CONSTANT(bool, value =
      (::boost_cryray::type_traits::ice_and<
         ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_reference<T>::value>::value,
         ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_void<T>::value>::value,
         ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_function<T>::value>::value
      >::value));
#else
   BOOST_STATIC_CONSTANT(bool, value =
      (::boost_cryray::type_traits::ice_and<
         ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_reference<T>::value>::value,
         ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_void<T>::value>::value
      >::value));
#endif
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_object,T,::boost_cryray::detail::is_object_impl<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_OBJECT_HPP_INCLUDED
