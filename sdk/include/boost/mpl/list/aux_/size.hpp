//-----------------------------------------------------------------------------
// boost mpl/list/aux_/size.hpp header file
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

#ifndef BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED

#include "boost/mpl/size_fwd.hpp"
#include "boost/mpl/list/aux_/tag.hpp"

namespace boost_cryray {
namespace mpl {

template<>
struct size_traits< aux::list_tag >
{
    template< typename List > struct algorithm
        : List::size
    {
    };
};

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_LIST_AUX_SIZE_HPP_INCLUDED
