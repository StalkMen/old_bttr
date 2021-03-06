//-----------------------------------------------------------------------------
// boost mpl/vector/aux_/size.hpp header file
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

#ifndef BOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED

#include "boost/mpl/size_fwd.hpp"
#include "boost/mpl/integral_c.hpp"
#include "boost/mpl/vector/aux_/tag.hpp"
#include "boost/mpl/aux_/config/vector.hpp"
#include "boost/config.hpp"

namespace boost_cryray {
namespace mpl {

#if defined(BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL)

template<>
struct size_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
        : Vector::size
    {
    };
};

#else

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct size_traits< aux::vector_tag<N> >
{
    template< typename Vector > struct algorithm
        : integral_c<long,N>
    {
    };
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_VECTOR_AUX_SIZE_HPP_INCLUDED
