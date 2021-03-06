//-----------------------------------------------------------------------------
// boost/mpl/is_even.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_MATH_IS_EVEN_HPP_INCLUDED
#define BOOST_MPL_MATH_IS_EVEN_HPP_INCLUDED

#include "boost/mpl/bool.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"
#include "boost/mpl/aux_/config/eti.hpp"

namespace boost_cryray { namespace mpl {

namespace math {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(N)
    >
struct is_even
    : bool_<((N::value % 2) == 0)>
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_even,(N))
};

} // namespace math

BOOST_MPL_AUX_VOID_SPEC(1, math::is_even)

}} // namespace boost_cryray::mpl

#endif // BOOST_MPL_MATH_IS_EVEN_HPP_INCLUDED
