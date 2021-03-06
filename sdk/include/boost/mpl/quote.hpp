//-----------------------------------------------------------------------------
// boost mpl/quote.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_QUOTE_HPP_INCLUDED
#define BOOST_MPL_QUOTE_HPP_INCLUDED

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "boost/mpl/aux_/has_type.hpp"
#endif

#include "boost/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_NO_PREPROCESSED_HEADERS) \
 && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER quote.hpp
#   include "boost/mpl/aux_/include_preprocessed.hpp"

#else

#   include "boost/mpl/limits/arity.hpp"
#   include "boost/mpl/aux_/preprocessor/params.hpp"
#   include "boost/mpl/aux_/config/ttp.hpp"
#   include "boost/mpl/aux_/config/ctps.hpp"

#   include "boost/preprocessor/iterate.hpp"
#   include "boost/preprocessor/cat.hpp"

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)

namespace boost_cryray {
namespace mpl {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename T, bool has_type_ = aux::has_type<T>::value >
struct quote_impl
#if !defined(__BORLANDC__) && (__BORLANDC__ <= 0x561 || !defined(BOOST_STRICT_CONFIG))
    : T
{
#else
{
    typedef typename T::type type;
#endif
};

template< typename T >
struct quote_impl<T,false>
{
    typedef T type;
};

#else

template< bool > struct quote_impl
{
    template< typename T > struct result_
        : T
    {
    };
};

template<> struct quote_impl<false>
{
    template< typename T > struct result_
    {
        typedef T type;
    };
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, BOOST_MPL_METAFUNCTION_MAX_ARITY, "boost/mpl/quote.hpp"))
#include BOOST_PP_ITERATE()

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

#endif // BOOST_MPL_USE_PREPROCESSED_HEADERS
#endif // BOOST_MPL_QUOTE_HPP_INCLUDED

///// iteration

#else
#define i BOOST_PP_FRAME_ITERATION(1)

template<
      template< BOOST_MPL_PP_PARAMS(i, typename P) > class F
    >
struct BOOST_PP_CAT(quote,i)
{
    template< BOOST_MPL_PP_PARAMS(i, typename U) > struct apply
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
        : quote_impl< F< BOOST_MPL_PP_PARAMS(i, U) > >
#else
        : quote_impl< aux::has_type< F< BOOST_MPL_PP_PARAMS(i, U) > >::value >
            ::template result_< F< BOOST_MPL_PP_PARAMS(i, U) > >
#endif
    {
    };
};

#undef i
#endif // BOOST_PP_IS_ITERATING
