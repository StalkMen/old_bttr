// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef FUNCTION_OBJECT_DWA2002725_HPP
# define FUNCTION_OBJECT_DWA2002725_HPP
# include <boost/python/detail/wrap_python.hpp>
# include <boost/function/function2.hpp>
# include <boost/python/object_core.hpp>
# include <boost/python/args_fwd.hpp>
# include <boost/python/object/py_function.hpp>

namespace boost_cryray { namespace python {

namespace objects
{ 
  BOOST_PYTHON_DECL api::object function_object(
      py_function const& f
      , unsigned min_arity, unsigned max_arity
      , python::detail::keyword_range const&);

  BOOST_PYTHON_DECL api::object function_object(
      py_function const& f
      , unsigned arity
      , python::detail::keyword_range const&);

  BOOST_PYTHON_DECL api::object function_object(py_function const& f, unsigned arity);

  // Add an attribute to the name_space with the given name. If it is
  // a Boost.Python function object
  // (boost/python/object/function.hpp), and an existing function is
  // already there, add it as an overload.
  BOOST_PYTHON_DECL void add_to_namespace(
      object const& name_space, char const* name, object const& attribute);

  BOOST_PYTHON_DECL void add_to_namespace(
      object const& name_space, char const* name, object const& attribute, char const* doc);
}

}} // namespace boost_cryray::python::objects

#endif // FUNCTION_OBJECT_DWA2002725_HPP
