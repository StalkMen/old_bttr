
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
// Hinnant & John Maddock 2000.  Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED
#define BOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED

#include "boost/type_traits/is_const.hpp"
#include "boost/type_traits/broken_compiler_spec.hpp"
#include "boost/type_traits/detail/cv_traits_impl.hpp"
#include "boost/config.hpp"

#include <cstddef>

// should be the last #include
#include "boost/type_traits/detail/type_trait_def.hpp"

namespace boost_cryray {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template <typename T, bool is_const>
struct remove_volatile_helper
{
    typedef T type;
};

template <typename T>
struct remove_volatile_helper<T,true>
{
    typedef T const type;
};

template <typename T>
struct remove_volatile_impl
{
    typedef typename remove_volatile_helper<
          typename cv_traits_imp<T*>::unqualified_type
        , ::boost_cryray::is_const<T>::value
        >::type type;
};

} // namespace detail

// * convert a type T to a non-volatile type - remove_volatile<T>

BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_volatile,T,typename detail::remove_volatile_impl<T>::type)
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_volatile,T&,T&)
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_volatile,T volatile[N],T type[N])
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_volatile,T const volatile[N],T const type[N])

#else

BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_volatile,T,typename detail::remove_volatile_impl<T>::type)

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost_cryray

#include "boost/type_traits/detail/type_trait_undef.hpp"

#endif // BOOST_TT_REMOVE_VOLATILE_HPP_INCLUDED
