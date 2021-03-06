//-----------------------------------------------------------------------------
// boost mpl/aux_/debug_print.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Fernando Cacciola, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_DEBUG_PRINT_HPP_INCLUDED
#define BOOST_MPL_AUX_DEBUG_PRINT_HPP_INCLUDED

namespace boost_cryray {
namespace mpl {
namespace aux {

template< typename T >
struct print_
{
    enum { value = T::not_existing_memeber };
};

} // namespace aux
} // namespace mpl
} // namespace boost_cryray

#define BOOST_MPL_AUX_DEBUG_PRINT(type) \
enum { mpl_debug_print_##type = boost_cryray::mpl::aux::print_<type>::value } ;\
/**/

#endif // BOOST_MPL_AUX_DEBUG_PRINT_HPP_INCLUDED
