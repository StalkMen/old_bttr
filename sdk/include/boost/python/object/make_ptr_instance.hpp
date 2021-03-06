// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef MAKE_PTR_INSTANCE_DWA200296_HPP
# define MAKE_PTR_INSTANCE_DWA200296_HPP

# include <boost/python/object/make_instance.hpp>
# include <boost/python/converter/registry.hpp>
# include <boost/type_traits/is_polymorphic.hpp>
# include <boost/get_pointer.hpp>
# include <typeinfo>

namespace boost_cryray { namespace python { namespace objects { 

template <class T, class Holder>
struct make_ptr_instance
    : make_instance_impl<T, Holder, make_ptr_instance<T,Holder> >
{
    template <class Arg>
    static inline Holder* construct(void* storage, PyObject*, Arg& x)
    {
        return new (storage) Holder(x);
    }
    
    template <class Ptr>
    static inline PyTypeObject* get_class_object(Ptr const& x)
    {
        return get_class_object_impl(get_pointer(x));
    }

 private:
    template <class U>
    static inline PyTypeObject* get_class_object_impl(U const volatile* p)
    {
        PyTypeObject* derived = get_derived_class_object(is_polymorphic<U>::type(), p);
        if (derived)
            return derived;
        return converter::registered<T>::converters.get_class_object();
    }
    
    template <class U>
    static inline PyTypeObject* get_derived_class_object(mpl::true_, U const volatile* x)
    {
        converter::registration const* r = converter::registry::query(type_info(typeid(*x)));
        return r ? r->m_class_object : 0;
    }
    
    template <class U>
    static inline PyTypeObject* get_derived_class_object(mpl::false_, U* x)
    {
# if BOOST_WORKAROUND(__MWERKS__, <= 0x2407)
        if (typeid(*x) != typeid(U))
            return get_derived_class_object(mpl::true_(), x);
# else
        (void)x;
# endif 
        return 0;
    }
};
  

}}} // namespace boost_cryray::python::object

#endif // MAKE_PTR_INSTANCE_DWA200296_HPP
