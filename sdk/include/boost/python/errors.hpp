//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.

#ifndef ERRORS_DWA052500_H_
# define ERRORS_DWA052500_H_

# include <boost/python/detail/config.hpp>
# include <boost/python/detail/wrap_python.hpp>
# include <boost/function/function0.hpp>

namespace boost_cryray { namespace python {

struct BOOST_PYTHON_DECL error_already_set {};
struct BOOST_PYTHON_DECL argument_error : error_already_set {};

// Handles exceptions caught just before returning to Python code.
// Returns true iff an exception was caught.
BOOST_PYTHON_DECL bool handle_exception_impl(function0<void>);

template <class T>
bool handle_exception(T f)
{
    return handle_exception_impl(function0<void>(boost_cryray::ref(f)));
}

namespace detail { inline void rethrow() { throw; } }

inline void handle_exception()
{
    handle_exception(detail::rethrow);
}

BOOST_PYTHON_DECL void throw_argument_error();
BOOST_PYTHON_DECL void throw_error_already_set();

template <class T>
inline T* expect_non_null(T* x)
{
    if (x == 0)
        throw_error_already_set();
    return x;
}

// Return source if it is an instance of pytype; throw an appropriate
// exception otherwise.
BOOST_PYTHON_DECL PyObject* pytype_check(PyTypeObject* pytype, PyObject* source);

}} // namespace boost_cryray::python

#endif // ERRORS_DWA052500_H_
