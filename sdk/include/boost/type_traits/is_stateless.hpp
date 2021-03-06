
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_STATELESS_HPP_INCLUDED
#define BOOST_TT_IS_STATELESS_HPP_INCLUDED

#include "boost/type_traits/has_trivial_constructor.hpp"
#include "boost/type_traits/has_trivial_copy.hpp"
#include "boost/type_traits/has_trivial_destructor.hpp"
#include "boost/type_traits/is_class.hpp"
#include "boost/type_traits/is_empty.hpp"
#include "boost/type_traits/detail/ice_and.hpp"
#include "boost/config.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

namespace detail {

template <typename T>
struct is_stateless_impl
{
  BOOST_STATIC_CONSTANT(bool, value = 
    (::boost_cryray::type_traits::ice_and<
       ::boost_cryray::has_trivial_constructor<T>::value,
       ::boost_cryray::has_trivial_copy<T>::value,
       ::boost_cryray::has_trivial_destructor<T>::value,
       ::boost_cryray::is_class<T>::value,
       ::boost_cryray::is_empty<T>::value
     >::value));
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_stateless,T,::boost_cryray::detail::is_stateless_impl<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_STATELESS_HPP_INCLUDED
