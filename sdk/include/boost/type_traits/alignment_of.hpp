
// (C) Copyright John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.

#ifndef BOOST_TT_ALIGNMENT_OF_HPP_INCLUDED
#define BOOST_TT_ALIGNMENT_OF_HPP_INCLUDED

#include "boost/config.hpp"
#include <cstddef>

// should be the last #include
#include "boost/type_traits/detail/size_t_trait_def.hpp"

#ifdef BOOST_MSVC
#   pragma warning(push)
#   pragma warning(disable: 4121) // alignment is sensitive to packing
#endif
#ifdef __BORLANDC__
#pragma option push -Vx- -Ve-
#endif

namespace boost_cryray {

template <typename T> struct alignment_of;

// get the alignment of some arbitrary type:
namespace detail {

template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};


template <unsigned A, unsigned S>
struct alignment_logic
{
    BOOST_STATIC_CONSTANT(std::size_t, value = A < S ? A : S);
};


template< typename T >
struct alignment_of_impl
{
    BOOST_STATIC_CONSTANT(std::size_t, value =
        (::boost_cryray::detail::alignment_logic<
            sizeof(detail::alignment_of_hack<T>) - sizeof(T),
            sizeof(T)
        >::value));
};

} // namespace detail

BOOST_TT_AUX_SIZE_T_TRAIT_DEF1(alignment_of,T,::boost_cryray::detail::alignment_of_impl<T>::value)

// references have to be treated specially, assume
// that a reference is just a special pointer:
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <typename T>
struct alignment_of<T&>
    : alignment_of<T*>
{
};
#endif
#ifdef __BORLANDC__
// long double gives an incorrect value of 10 (!)
// unless we do this...
struct long_double_wrapper{ long double ld; };
template<> struct alignment_of<long double>
   : public alignment_of<long_double_wrapper>{};
#endif

// void has to be treated specially:
BOOST_TT_AUX_SIZE_T_TRAIT_SPEC1(alignment_of,void,0)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_SIZE_T_TRAIT_SPEC1(alignment_of,void const,0)
BOOST_TT_AUX_SIZE_T_TRAIT_SPEC1(alignment_of,void volatile,0)
BOOST_TT_AUX_SIZE_T_TRAIT_SPEC1(alignment_of,void const volatile,0)
#endif

} // namespace boost_cryray

#ifdef __BORLANDC__
#pragma option pop
#endif
#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif

#include "boost/type_traits/detail/size_t_trait_undef.hpp"

#endif // BOOST_TT_ALIGNMENT_OF_HPP_INCLUDED

