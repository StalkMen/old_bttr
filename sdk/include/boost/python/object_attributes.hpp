// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef OBJECT_ATTRIBUTES_DWA2002615_HPP
# define OBJECT_ATTRIBUTES_DWA2002615_HPP

# include <boost/python/proxy.hpp>
# include <boost/python/object_core.hpp>
# include <boost/python/object_protocol.hpp>

namespace boost_cryray { namespace python { namespace api {

struct const_attribute_policies
{
    typedef char const* key_type;
    static object get(object const& target, char const* key);
};
  
struct attribute_policies : const_attribute_policies
{
    static object const& set(object const& target, char const* key, object const& value);
    static void del(object const&target, char const* key);
};

//
// implementation
//
template <class U>
inline object_attribute object_operators<U>::attr(char const* name)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_attribute(x, name);
}

template <class U>
inline const_object_attribute object_operators<U>::attr(char const* name) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_attribute(x, name);
}

inline object const_attribute_policies::get(object const& target, char const* key)
{
    return python::getattr(target, key);
}

inline object const& attribute_policies::set(
    object const& target
    , char const* key
    , object const& value)
{
    python::setattr(target, key, value);
    return value;
}

inline void attribute_policies::del(
    object const& target
    , char const* key)
{
    python::delattr(target, key);
}

}}} // namespace boost_cryray::python::api

#endif // OBJECT_ATTRIBUTES_DWA2002615_HPP
