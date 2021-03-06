#if !defined(BOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

# ifndef TARGET_DWA2002521_HPP
#  define TARGET_DWA2002521_HPP

#  include <boost/python/detail/preprocessor.hpp>

#  include <boost/type.hpp>

#  include <boost/preprocessor/comma_if.hpp>
#  include <boost/preprocessor/if.hpp>
#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/debug/line.hpp>
#  include <boost/preprocessor/enum_params.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_params.hpp>

namespace boost_cryray { namespace python { namespace detail {

#  define BOOST_PP_ITERATION_PARAMS_1                                                                   \
    (4, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/detail/target.hpp>, BOOST_PYTHON_FUNCTION_POINTER))
#  include BOOST_PP_ITERATE()

#  define BOOST_PP_ITERATION_PARAMS_1                                                                    \
    (4, (0, BOOST_PYTHON_CV_COUNT - 1, <boost/python/detail/target.hpp>, BOOST_PYTHON_POINTER_TO_MEMBER))
#  include BOOST_PP_ITERATE()

template <class R, class T>
boost_cryray::type<T*>* target(R (T::*)) { return 0; }

}}} // namespace boost_cryray::python::detail

# endif // TARGET_DWA2002521_HPP

/* --------------- function pointers --------------- */
#elif BOOST_PP_ITERATION_DEPTH() == 1 && BOOST_PP_ITERATION_FLAGS() == BOOST_PYTHON_FUNCTION_POINTER
# line BOOST_PP_LINE(__LINE__, target.hpp(function_pointers))

# define N BOOST_PP_ITERATION()

template <class R BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
boost_cryray::type<BOOST_PP_IF(N, A0, void)>* target(R (*)(BOOST_PP_ENUM_PARAMS_Z(1, N, A)))
{
    return 0;
}

# undef N

/* --------------- pointers-to-members --------------- */
#elif BOOST_PP_ITERATION_DEPTH() == 1 && BOOST_PP_ITERATION_FLAGS() == BOOST_PYTHON_POINTER_TO_MEMBER
// Outer over cv-qualifiers

# define BOOST_PP_ITERATION_PARAMS_2 (3, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/detail/target.hpp>))
# include BOOST_PP_ITERATE()

#elif BOOST_PP_ITERATION_DEPTH() == 2
# line BOOST_PP_LINE(__LINE__, target.hpp(pointers-to-members))
// Inner over arities

# define N BOOST_PP_ITERATION()
# define Q BOOST_PYTHON_CV_QUALIFIER(BOOST_PP_RELATIVE_ITERATION(1))

template <class R, class T BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)>
boost_cryray::type<T Q*>* target(R (T::*)(BOOST_PP_ENUM_PARAMS_Z(1, N, A)) Q)
{
    return 0;
}

# undef N
# undef Q

#endif
