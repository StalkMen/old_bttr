#if !defined(BOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
# ifndef CALL_METHOD_DWA2002411_HPP
#  define CALL_METHOD_DWA2002411_HPP

#  include <boost/type.hpp>

#  include <boost/python/converter/arg_to_python.hpp>
#  include <boost/python/converter/return_from_python.hpp>
#  include <boost/python/detail/preprocessor.hpp>
#  include <boost/python/detail/void_return.hpp>

#  include <boost/preprocessor/comma_if.hpp>
#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/repeat.hpp>
#  include <boost/preprocessor/debug/line.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace boost_cryray { namespace python {

# define BOOST_PYTHON_FAST_ARG_TO_PYTHON_GET(z, n, _) \
    , converter::arg_to_python<A##n>(a##n).get()

# define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/call_method.hpp>))
# include BOOST_PP_ITERATE()

# undef BOOST_PYTHON_FAST_ARG_TO_PYTHON_GET

}} // namespace boost_cryray::python

# endif // CALL_METHOD_DWA2002411_HPP

#elif BOOST_PP_ITERATION_DEPTH() == 1
# line BOOST_PP_LINE(__LINE__, call_method.hpp)

# define N BOOST_PP_ITERATION()

template <
    class R
    BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class A)
    >
typename detail::returnable<R>::type
call_method(PyObject* self, char const* name
    BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, const& a)
    , boost_cryray::type<R>* = 0
    )
{
    converter::return_from_python<R> converter;
    return converter(
        PyEval_CallMethod(
            self
            , const_cast<char*>(name)
            , const_cast<char*>("(" BOOST_PP_REPEAT_1ST(N, BOOST_PYTHON_FIXED, "O") ")")
            BOOST_PP_REPEAT_1ST(N, BOOST_PYTHON_FAST_ARG_TO_PYTHON_GET, nil)
            ));
}

# undef N

#endif // BOOST_PP_IS_ITERATING
