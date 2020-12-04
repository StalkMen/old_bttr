/*
 *
 * Copyright (c) 1998-2000
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org/libs/regex for documentation.
  *   FILE         regex.h
  *   VERSION      3.12
  *   DESCRIPTION: Declares POSIX API functions
  */

#ifndef BOOST_RE_REGEX_H
#define BOOST_RE_REGEX_H

#include <boost/cregex.hpp>

//
// add using declarations to bring POSIX API functions into
// global scope, only if this is C++ (and not C).
//
#ifdef __cplusplus

using boost_cryray::regoff_t;
using boost_cryray::regex_tA;
using boost_cryray::regmatch_t;
using boost_cryray::REG_BASIC;
using boost_cryray::REG_EXTENDED;
using boost_cryray::REG_ICASE;
using boost_cryray::REG_NOSUB;
using boost_cryray::REG_NEWLINE;
using boost_cryray::REG_NOSPEC;
using boost_cryray::REG_PEND;
using boost_cryray::REG_DUMP;
using boost_cryray::REG_NOCOLLATE;
using boost_cryray::REG_ESCAPE_IN_LISTS;
using boost_cryray::REG_NEWLINE_ALT;
using boost_cryray::REG_PERL;
using boost_cryray::REG_AWK;
using boost_cryray::REG_GREP;
using boost_cryray::REG_EGREP;
using boost_cryray::REG_ASSERT;
using boost_cryray::REG_INVARG;
using boost_cryray::REG_ATOI;
using boost_cryray::REG_ITOA;

using boost_cryray::REG_NOTBOL;
using boost_cryray::REG_NOTEOL;
using boost_cryray::REG_STARTEND;

using boost_cryray::reg_comp_flags;
using boost_cryray::reg_exec_flags;
using boost_cryray::regcompA;
using boost_cryray::regerrorA;
using boost_cryray::regexecA;
using boost_cryray::regfreeA;

#ifndef BOOST_NO_WREGEX
using boost_cryray::regcompW;
using boost_cryray::regerrorW;
using boost_cryray::regexecW;
using boost_cryray::regfreeW;
using boost_cryray::regex_tW;
#endif

using boost_cryray::REG_NOERROR;
using boost_cryray::REG_NOMATCH;
using boost_cryray::REG_BADPAT;
using boost_cryray::REG_ECOLLATE;
using boost_cryray::REG_ECTYPE;
using boost_cryray::REG_EESCAPE;
using boost_cryray::REG_ESUBREG;
using boost_cryray::REG_EBRACK;
using boost_cryray::REG_EPAREN;
using boost_cryray::REG_EBRACE;
using boost_cryray::REG_BADBR;
using boost_cryray::REG_ERANGE;
using boost_cryray::REG_ESPACE;
using boost_cryray::REG_BADRPT;
using boost_cryray::REG_EEND;
using boost_cryray::REG_ESIZE;
using boost_cryray::REG_ERPAREN;
using boost_cryray::REG_EMPTY;
using boost_cryray::REG_E_MEMORY;
using boost_cryray::REG_E_UNKNOWN;
using boost_cryray::reg_errcode_t;

#endif // __cplusplus

#endif // BOOST_RE_REGEX_H


