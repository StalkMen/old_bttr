//-----------------------------------------------------------------------------
// boost mpl/project2nd.hpp header file
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

#ifndef BOOST_MPL_PROJECT2ND_HPP_INCLUDED
#define BOOST_MPL_PROJECT2ND_HPP_INCLUDED

#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost_cryray {
namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T1)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T2)
    >
struct project2nd
{
    typedef T2 type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,project2nd,(T1,T2))
};

BOOST_MPL_AUX_VOID_SPEC(2, project2nd)

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_PROJECT2ND_HPP_INCLUDED
