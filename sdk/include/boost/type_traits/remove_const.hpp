
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
// Hinnant & John Maddock 2000.  Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_REMOVE_CONST_HPP_INCLUDED
#define BOOST_TT_REMOVE_CONST_HPP_INCLUDED

#include "boost/type_traits/is_volatile.hpp"
#include "boost/type_traits/broken_compiler_spec.hpp"
#include "boost/type_traits/detail/cv_traits_impl.hpp"
#include "boost/config.hpp"

#include <cstddef>

// should be the last #include
#include "boost/type_traits/detail/type_trait_def.hpp"

namespace boost_cryray {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template <typename T, bool is_vol>
struct remove_const_helper
{
    typedef T type;
};

template <typename T>
struct remove_const_helper<T, true>
{
    typedef T volatile type;
};


template <typename T>
struct remove_const_impl
{
    typedef typename remove_const_helper<
          typename cv_traits_imp<T*>::unqualified_type
        , ::boost_cryray::is_volatile<T>::value
        >::type type;
};

} // namespace detail

// * convert a type T to non-const type - remove_const<T>

BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_const,T,typename detail::remove_const_impl<T>::type)
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_const,T&,T&)
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_const,T const[N],T type[N])
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_const,T const volatile[N],T volatile type[N])

#else

BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_const,T,typename detail::remove_const_impl<T>::type)

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost_cryray

#include "boost/type_traits/detail/type_trait_undef.hpp"

#endif // BOOST_TT_REMOVE_CONST_HPP_INCLUDED
