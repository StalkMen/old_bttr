// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef CLASS_FWD_DWA200222_HPP
# define CLASS_FWD_DWA200222_HPP
# include <boost/python/detail/not_specified.hpp>

namespace boost_cryray { namespace python { 

template <
    class T // class being wrapped
    // arbitrarily-ordered optional arguments. Full qualification needed for MSVC6
    , class X1 = ::boost_cryray::python::detail::not_specified
    , class X2 = ::boost_cryray::python::detail::not_specified
    , class X3 = ::boost_cryray::python::detail::not_specified
    >
class class_;

}} // namespace boost_cryray::python

#endif // CLASS_FWD_DWA200222_HPP
