#ifndef BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
#define BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  bind/placeholders.hpp - _N definitions
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

#include <boost/bind/arg.hpp>
#include <boost/config.hpp>

namespace
{

#if defined(__BORLANDC__)

static inline boost_cryray::arg<1> _1() { return boost_cryray::arg<1>(); }
static inline boost_cryray::arg<2> _2() { return boost_cryray::arg<2>(); }
static inline boost_cryray::arg<3> _3() { return boost_cryray::arg<3>(); }
static inline boost_cryray::arg<4> _4() { return boost_cryray::arg<4>(); }
static inline boost_cryray::arg<5> _5() { return boost_cryray::arg<5>(); }
static inline boost_cryray::arg<6> _6() { return boost_cryray::arg<6>(); }
static inline boost_cryray::arg<7> _7() { return boost_cryray::arg<7>(); }
static inline boost_cryray::arg<8> _8() { return boost_cryray::arg<8>(); }
static inline boost_cryray::arg<9> _9() { return boost_cryray::arg<9>(); }

#elif (defined(BOOST_MSVC) && BOOST_MSVC <= 1300) || (defined(__DECCXX_VER) && __DECCXX_VER <= 60590031) || defined(__MWERKS__)

static boost_cryray::arg<1> _1;
static boost_cryray::arg<2> _2;
static boost_cryray::arg<3> _3;
static boost_cryray::arg<4> _4;
static boost_cryray::arg<5> _5;
static boost_cryray::arg<6> _6;
static boost_cryray::arg<7> _7;
static boost_cryray::arg<8> _8;
static boost_cryray::arg<9> _9;

#else

boost_cryray::arg<1> _1;
boost_cryray::arg<2> _2;
boost_cryray::arg<3> _3;
boost_cryray::arg<4> _4;
boost_cryray::arg<5> _5;
boost_cryray::arg<6> _6;
boost_cryray::arg<7> _7;
boost_cryray::arg<8> _8;
boost_cryray::arg<9> _9;

#endif

} // unnamed namespace

#endif // #ifndef BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
