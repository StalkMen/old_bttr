// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BORROWED_DWA2002614_HPP
# define BORROWED_DWA2002614_HPP
# include <boost/python/detail/borrowed_ptr.hpp>

namespace boost_cryray { namespace python { 

template <class T>
inline python::detail::borrowed<T>* borrowed(T* p)
{
    return (detail::borrowed<T>*)p;
}
    
}} // namespace boost_cryray::python

#endif // BORROWED_DWA2002614_HPP
