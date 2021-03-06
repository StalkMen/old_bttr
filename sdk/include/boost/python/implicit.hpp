// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef IMPLICIT_DWA2002325_HPP
# define IMPLICIT_DWA2002325_HPP
# include <boost/type.hpp>
# include <boost/python/converter/implicit.hpp>
# include <boost/python/converter/registry.hpp>
# include <boost/python/type_id.hpp>

namespace boost_cryray { namespace python { 

template <class Source, class Target>
void implicitly_convertible(boost_cryray::type<Source>* = 0, boost_cryray::type<Target>* = 0)
{
    typedef converter::implicit<Source,Target> functions;
    
    converter::registry::push_back(
          &functions::convertible
        , &functions::construct
        , type_id<Target>());
}

}} // namespace boost_cryray::python

#endif // IMPLICIT_DWA2002325_HPP
