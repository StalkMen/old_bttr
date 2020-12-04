
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED
#define BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED

#include "boost/type_traits/is_integral.hpp"
#include "boost/type_traits/is_float.hpp"
#include "boost/type_traits/detail/ice_or.hpp"
#include "boost/config.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

namespace detail {

template< typename T >
struct is_arithmetic_impl
{ 
    BOOST_STATIC_CONSTANT(bool, value = 
        (::boost_cryray::type_traits::ice_or< 
            ::boost_cryray::is_integral<T>::value,
            ::boost_cryray::is_float<T>::value
        >::value)); 
};

} // namespace detail

//* is a type T an arithmetic type described in the standard (3.9.1p8)
BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_arithmetic,T,::boost_cryray::detail::is_arithmetic_impl<T>::value)

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED
