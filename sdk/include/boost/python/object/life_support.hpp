// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef LIFE_SUPPORT_DWA200222_HPP
# define LIFE_SUPPORT_DWA200222_HPP
# include <boost/python/detail/config.hpp>
# include <boost/python/detail/wrap_python.hpp>

namespace boost_cryray { namespace python { namespace objects { 

BOOST_PYTHON_DECL PyObject* make_nurse_and_patient(PyObject* nurse, PyObject* patient);

}}} // namespace boost_cryray::python::object

#endif // LIFE_SUPPORT_DWA200222_HPP
