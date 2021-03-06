//  (C) Copyright John Maddock 2000.
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/libs/static_assert for documentation.

/*
 Revision history:
   02 August 2000
      Initial version.
*/

#ifndef BOOST_STATIC_ASSERT_HPP
#define BOOST_STATIC_ASSERT_HPP

#include <boost/config.hpp>

#ifdef __BORLANDC__
//
// workaround for buggy integral-constant expression support:
#define BOOST_BUGGY_INTEGRAL_CONSTANT_EXPRESSIONS
#endif

namespace boost_cryray{

// HP aCC cannot deal with missing names for template value parameters
template <bool x> struct STATIC_ASSERTION_FAILURE;

template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };

// HP aCC cannot deal with missing names for template value parameters
template<int x> struct static_assert_test{};

}

//
// Implicit instantiation requires that all member declarations be
// instantiated, but that the definitions are *not* instantiated.
//
// It's not particularly clear how this applies to enum's or typedefs;
// both are described as declarations [7.1.3] and [7.2] in the standard,
// however some compilers use "delayed evaluation" of one or more of
// these when implicitly instantiating templates.  We use typedef declarations
// by default, but try defining BOOST_USE_ENUM_STATIC_ASSERT if the enum
// version gets better results from your compiler...
//
// Implementation:
// Both of these versions rely on sizeof(incomplete_type) generating an error
// message containing the name of the incomplete type.  We use
// "STATIC_ASSERTION_FAILURE" as the type name here to generate
// an eye catching error message.  The result of the sizeof expression is either
// used as an enum initialiser, or as a template argument depending which version
// is in use...
// Note that the argument to the assert is explicitly cast to bool using old-
// style casts: too many compilers currently have problems with static_cast
// when used inside integral constant expressions.
//
#if !defined(BOOST_BUGGY_INTEGRAL_CONSTANT_EXPRESSIONS) && !defined(__MWERKS__)

#if defined(BOOST_MSVC)
// __LINE__ macro broken when -ZI is used see Q199057
// fortunately MSVC ignores duplicate typedef's.
#define BOOST_STATIC_ASSERT( B ) \
   typedef ::boost_cryray::static_assert_test<\
      sizeof(::boost_cryray::STATIC_ASSERTION_FAILURE< (bool)( B ) >)\
      > boost_static_assert_typedef_
#elif defined(BOOST_INTEL_CXX_VERSION)
// agurt 15/sep/02: a special care is needed to force Intel C++ issue an error 
// instead of warning in case of failure
# define BOOST_STATIC_ASSERT( B ) \
    typedef char BOOST_JOIN(boost_static_assert_typedef_, __LINE__) \
        [ ::boost_cryray::STATIC_ASSERTION_FAILURE< (bool)( B ) >::value ]
#else
// generic version
#define BOOST_STATIC_ASSERT( B ) \
   typedef ::boost_cryray::static_assert_test<\
      sizeof(::boost_cryray::STATIC_ASSERTION_FAILURE< (bool)( B ) >)>\
         BOOST_JOIN(boost_static_assert_typedef_, __LINE__)
#endif

#else
// alternative enum based implementation:
#define BOOST_STATIC_ASSERT( B ) \
   enum { BOOST_JOIN(boost_static_assert_enum_, __LINE__) \
      = sizeof(::boost_cryray::STATIC_ASSERTION_FAILURE< (bool)( B ) >) }
#endif


#endif // BOOST_STATIC_ASSERT_HPP
