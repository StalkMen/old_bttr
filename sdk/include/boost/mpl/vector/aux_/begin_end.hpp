//-----------------------------------------------------------------------------
// boost mpl/vector/aux_/begin_end.hpp header file
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

#ifndef BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED

#include "boost/mpl/aux_/config/vector.hpp"

#if defined(BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL)

#   include "boost/mpl/begin_end_fwd.hpp"
#   include "boost/mpl/integral_c.hpp"
#   include "boost/mpl/vector/aux_/iterator.hpp"
#   include "boost/mpl/vector/aux_/tag.hpp"

namespace boost_cryray {
namespace mpl {

template<>
struct begin_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
    {
        typedef vector_iterator<
              Vector
            , integral_c<long,0>
            > type;
    };
};

template<>
struct end_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
    {
        typedef vector_iterator<
              Vector
            , typename Vector::size
            > type;
    };
};

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL

#endif // BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
