//-----------------------------------------------------------------------------
// boost mpl/vector/vector20.hpp header file
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

#ifndef BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED
#define BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "boost/mpl/vector/vector10.hpp"
#endif

#include "boost/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_NO_PREPROCESSED_HEADERS) \
 && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER vector20.hpp
#   include "boost/mpl/vector/aux_/include_preprocessed.hpp"

#else

#   include "boost/mpl/aux_/config/vector.hpp"
#   include "boost/mpl/aux_/config/ctps.hpp"
#   include "boost/preprocessor/iterate.hpp"
#   include "boost/config.hpp"

namespace boost_cryray {
namespace mpl {

#   define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(11, 20, "boost/mpl/vector/aux_/numbered.hpp"))
#   include BOOST_PP_ITERATE()

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_USE_PREPROCESSED_HEADERS

#endif // BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED
