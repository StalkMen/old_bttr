#ifndef BOOST_CURRENT_FUNCTION_HPP_INCLUDED
#define BOOST_CURRENT_FUNCTION_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/current_function.hpp - BOOST_CURRENT_FUNCTION
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  http://www.boost.org/libs/utility/current_function.html
//

namespace boost_cryray
{

namespace detail
{

inline void current_function_helper()
{

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000))

# define BOOST_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)

# define BOOST_CURRENT_FUNCTION __FUNCSIG__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)

# define BOOST_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define BOOST_CURRENT_FUNCTION __func__

#else

# define BOOST_CURRENT_FUNCTION "(unknown)"

#endif

}

} // namespace detail

} // namespace boost_cryray

#endif // #ifndef BOOST_CURRENT_FUNCTION_HPP_INCLUDED
