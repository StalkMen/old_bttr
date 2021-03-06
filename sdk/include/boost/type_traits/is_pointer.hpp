
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
// Hinnant & John Maddock 2000.  Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_POINTER_HPP_INCLUDED
#define BOOST_TT_IS_POINTER_HPP_INCLUDED

#include "boost/type_traits/is_member_pointer.hpp"
#include "boost/type_traits/detail/ice_and.hpp"
#include "boost/type_traits/detail/ice_not.hpp"
#include "boost/type_traits/config.hpp"

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   include "boost/type_traits/is_reference.hpp"
#   include "boost/type_traits/is_array.hpp"
#   include "boost/type_traits/detail/is_function_ptr_tester.hpp"
#   include "boost/type_traits/detail/false_result.hpp"
#   include "boost/type_traits/detail/ice_or.hpp"
#endif

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost_cryray {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template< typename T > struct is_pointer_helper
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

#   define TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(helper,sp,result) \
template< typename T > struct helper<sp> \
{ \
    BOOST_STATIC_CONSTANT(bool, value = result); \
}; \
/**/

TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(is_pointer_helper,T*,true)
TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(is_pointer_helper,T* const,true)
TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(is_pointer_helper,T* volatile,true)
TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(is_pointer_helper,T* const volatile,true)

#   undef TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC

template< typename T >
struct is_pointer_impl
{
    BOOST_STATIC_CONSTANT(bool, value = 
        (::boost_cryray::type_traits::ice_and<
              ::boost_cryray::detail::is_pointer_helper<T>::value
            , ::boost_cryray::type_traits::ice_not<
                ::boost_cryray::is_member_pointer<T>::value
                >::value
            >::value)
        );
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pointer,T,::boost_cryray::detail::is_pointer_impl<T>::value)

#if defined(__BORLANDC__) && !defined(__COMO__)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T&,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& const,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& volatile,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& const volatile,false)
#endif

#else // no partial template specialization

namespace detail {

struct pointer_helper
{
    pointer_helper(const volatile void*);
};

yes_type BOOST_TT_DECL is_pointer_tester(pointer_helper);
no_type BOOST_TT_DECL is_pointer_tester(...);

template <bool>
struct is_pointer_select
    : ::boost_cryray::type_traits::false_result
{
};

template <>
struct is_pointer_select<false>
{
    template <typename T> struct result_
    {
        static T& make_t();
        BOOST_STATIC_CONSTANT(bool, value =
                (::boost_cryray::type_traits::ice_or<
                    (1 == sizeof(is_pointer_tester(make_t()))),
                    (1 == sizeof(type_traits::is_function_ptr_tester(make_t())))
                >::value));
    };
};

template <typename T>
struct is_pointer_impl
    : is_pointer_select<
          ::boost_cryray::type_traits::ice_or<
              ::boost_cryray::is_reference<T>::value
            , ::boost_cryray::is_array<T>::value
            >::value
        >::template result_<T>
{
};

BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void,false)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void const,false)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void volatile,false)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void const volatile,false)
#endif

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pointer,T,::boost_cryray::detail::is_pointer_impl<T>::value)

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost_cryray

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_POINTER_HPP_INCLUDED
