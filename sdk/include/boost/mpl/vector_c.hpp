//-----------------------------------------------------------------------------
// boost mpl/vector_c.hpp header file
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

#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_VECTOR_C_HPP_INCLUDED
#define BOOST_MPL_VECTOR_C_HPP_INCLUDED

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "boost/mpl/limits/vector.hpp"

#   include "boost/preprocessor/inc.hpp"
#   include "boost/preprocessor/cat.hpp"
#   include "boost/preprocessor/stringize.hpp"

#   define MPL_AUX_VECTOR_HEADER \
    BOOST_PP_CAT(BOOST_PP_CAT(vector,BOOST_MPL_LIMIT_VECTOR_SIZE), _c).hpp \
    /**/

#   include BOOST_PP_STRINGIZE(boost/mpl/vector/MPL_AUX_VECTOR_HEADER)
#   undef MPL_AUX_VECTOR_HEADER
#   include <climits>
#endif

#include "boost/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_NO_PREPROCESSED_HEADERS) && \
    !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER vector_c.hpp
#   include "boost/mpl/aux_/include_preprocessed.hpp"

#else

#   include "boost/mpl/limits/vector.hpp"
#   include "boost/mpl/aux_/config/nttp.hpp"

#   include "boost/preprocessor/arithmetic/sub.hpp"
#   include "boost/preprocessor/tuple/elem.hpp"
#   include "boost/preprocessor/enum_params_with_a_default.hpp"
#   include "boost/preprocessor/enum_params.hpp"
#   include "boost/preprocessor/enum.hpp"
#   include "boost/preprocessor/repeat.hpp"
#   include "boost/preprocessor/comma_if.hpp"
#   include "boost/preprocessor/iterate.hpp"

#   include "boost/config.hpp"

#if defined(BOOST_MPL_PREPROCESSING_MODE)
#   undef LONG_MAX
#endif

namespace boost_cryray {
namespace mpl {

#   define AUX_VECTOR_C(i) \
    BOOST_PP_CAT(BOOST_PP_CAT(vector,i),_c) \
    /**/

#   define AUX_VECTOR_C_PARAMS(param) \
    BOOST_PP_ENUM_PARAMS( \
          BOOST_MPL_LIMIT_VECTOR_SIZE \
        , param \
        ) \
    /**/

#   define AUX_VECTOR_C_DEFAULT_PARAMS(param, value) \
     BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( \
          BOOST_MPL_LIMIT_VECTOR_SIZE \
        , param \
        , value \
        ) \
    /**/

#   define AUX_VECTOR_C_N_PARAMS(n, param) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_PP_ENUM_PARAMS(n, param) \
    /**/

#   define AUX_VECTOR_C_N_PARTIAL_SPEC_PARAMS(n, param, def) \
    BOOST_PP_ENUM_PARAMS(n, param) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_PP_ENUM( \
          BOOST_PP_SUB_D(1,BOOST_MPL_LIMIT_VECTOR_SIZE,n) \
        , BOOST_PP_TUPLE_ELEM_3_2 \
        , def \
        ) \
    /**/

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// forward declaration
template<
      typename T
    , AUX_VECTOR_C_DEFAULT_PARAMS(BOOST_MPL_AUX_NTTP_DECL(long, C), LONG_MAX)
    >
struct vector_c;
#else
namespace aux {
template< BOOST_MPL_AUX_NTTP_DECL(int, N) > struct vector_c_impl_chooser;
}
#endif

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, BOOST_MPL_LIMIT_VECTOR_SIZE, "boost/mpl/vector_c.hpp"))
#include BOOST_PP_ITERATE()

// real C++ version is already taken care of
#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace aux {
// vector_count_args
#define BOOST_MPL_AUX_COUNT_ARGS_PREFIX vector_c
#define BOOST_MPL_AUX_COUNT_ARGS_DEFAULT LONG_MAX
#define BOOST_MPL_AUX_COUNT_ARGS_TEMPLATE_PARAM BOOST_MPL_AUX_NTTP_DECL(long, T)
#define BOOST_MPL_AUX_COUNT_ARGS_ARITY BOOST_MPL_LIMIT_VECTOR_SIZE
#define BOOST_MPL_AUX_COUNT_ARGS_USE_STANDARD_PP_PRIMITIVES
#include "boost/mpl/aux_/count_args.hpp"

template<
      typename T
    , AUX_VECTOR_C_PARAMS(BOOST_MPL_AUX_NTTP_DECL(long, C))
    >
struct vector_c_impl
{
    typedef aux::vector_c_count_args< AUX_VECTOR_C_PARAMS(C) > arg_num_;
    typedef typename aux::vector_c_impl_chooser< arg_num_::value >
        ::template result_< T, AUX_VECTOR_C_PARAMS(C) >::type type;
};

} // namespace aux

template<
      typename T
    , AUX_VECTOR_C_DEFAULT_PARAMS(BOOST_MPL_AUX_NTTP_DECL(long, C), LONG_MAX)
    >
struct vector_c
    : aux::vector_c_impl< T,AUX_VECTOR_C_PARAMS(C) >::type
{
    typedef typename aux::vector_c_impl<
          T,AUX_VECTOR_C_PARAMS(C)
        >::type type;
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#   undef AUX_VECTOR_C_N_PARTIAL_SPEC_PARAMS
#   undef AUX_VECTOR_C_N_PARAMS
#   undef AUX_VECTOR_C_DEFAULT_PARAMS
#   undef AUX_VECTOR_C_PARAMS
#   undef AUX_VECTOR_C

} // namespace mpl
} // namespace boost_cryray

#endif // BOOST_MPL_USE_PREPROCESSED_HEADERS
#endif // BOOST_MPL_VECTOR_C_HPP_INCLUDED

///// iteration

#else
#define i BOOST_PP_FRAME_ITERATION(1)

#   if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#if i == BOOST_MPL_LIMIT_VECTOR_SIZE

//: primary template (not a specialization!)
template<
      typename T
    AUX_VECTOR_C_N_PARAMS(i, BOOST_MPL_AUX_NTTP_DECL(long, C))
    >
struct vector_c
    : AUX_VECTOR_C(i)< T AUX_VECTOR_C_N_PARAMS(i, C) >
{
    typedef AUX_VECTOR_C(i)< T AUX_VECTOR_C_N_PARAMS(i, C) > type;
};

#else

template<
      typename T
    AUX_VECTOR_C_N_PARAMS(i, BOOST_MPL_AUX_NTTP_DECL(long, C))
    >
struct vector_c< T,AUX_VECTOR_C_N_PARTIAL_SPEC_PARAMS(i, C, LONG_MAX) >
    : AUX_VECTOR_C(i)< T AUX_VECTOR_C_N_PARAMS(i, C) >
{
    typedef AUX_VECTOR_C(i)< T AUX_VECTOR_C_N_PARAMS(i, C) > type;
};

#endif // i == BOOST_MPL_LIMIT_VECTOR_SIZE

#   else

namespace aux {

template<>
struct vector_c_impl_chooser<i>
{
    template<
          typename T
        , AUX_VECTOR_C_PARAMS(BOOST_MPL_AUX_NTTP_DECL(long, C))
        >
    struct result_
    {
        typedef AUX_VECTOR_C(i)<
              T AUX_VECTOR_C_N_PARAMS(i, C)
            > type;
    };
};

} // namespace aux

#   endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#undef i
#endif // BOOST_PP_IS_ITERATING
