//-----------------------------------------------------------------------------
// boost mpl/replace.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy and John R. Bandela
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_REPLACE_HPP_INCLUDED
#define BOOST_MPL_REPLACE_HPP_INCLUDED

#include "boost/mpl/replace_if.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/mpl/aux_/void_spec.hpp"

namespace boost_cryray {
namespace mpl {

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(OldType)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(NewType)
    >
struct replace
    : replace_if< Sequence,same_as<OldType>,NewType >
{
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

BOOST_MPL_AUX_ALGORITHM_VOID_SPEC(3, replace)

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_REPLACE_HPP_INCLUDED
