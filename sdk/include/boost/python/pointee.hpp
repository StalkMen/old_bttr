// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef POINTEE_DWA2002323_HPP
# define POINTEE_DWA2002323_HPP

# include <boost/type_traits/object_traits.hpp>
# include <boost/type_traits/remove_pointer.hpp>

namespace boost_cryray { namespace python {

namespace detail
{
  template <bool is_ptr = true>
  struct pointee_impl
  {
      template <class T> struct apply : remove_pointer<T> {};
  };

  template <>
  struct pointee_impl<false>
  {
      template <class T> struct apply
      {
          typedef typename T::element_type type;
      };
  };
}

template <class T>
struct pointee
    : detail::pointee_impl<is_pointer<T>::value>::template apply<T>
{
};

}} // namespace boost_cryray::python::detail

#endif // POINTEE_DWA2002323_HPP
