
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED
#define BOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED

#include "boost/type_traits/config.hpp"
#include "boost/type_traits/intrinsics.hpp"
#include "boost/type_traits/is_pod.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/type_traits/is_volatile.hpp"
#include "boost/type_traits/detail/ice_and.hpp"
#include "boost/type_traits/detail/ice_or.hpp"
#include "boost/type_traits/detail/ice_not.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

namespace detail {

template <typename T>
struct has_trivial_assign_impl
{
   BOOST_STATIC_CONSTANT(bool, value =
      (::boost_cryray::type_traits::ice_and<
         ::boost_cryray::type_traits::ice_or<
            ::boost_cryray::is_pod<T>::value,
            BOOST_HAS_TRIVIAL_ASSIGN(T)
         >::value,
      ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_const<T>::value >::value,
      ::boost_cryray::type_traits::ice_not< ::boost_cryray::is_volatile<T>::value >::value
      >::value));
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_trivial_assign,T,::boost_cryray::detail::has_trivial_assign_impl<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_HAS_TRIVIAL_ASSIGN_HPP_INCLUDED
