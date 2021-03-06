// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef ITERATOR_DWA2002512_HPP
# define ITERATOR_DWA2002512_HPP

# include <boost/python/detail/target.hpp>
# include <boost/python/object/iterator.hpp>
# include <boost/python/object_core.hpp>
# include <boost/type_traits/cv_traits.hpp>
# include <boost/type_traits/transform_traits.hpp>

namespace boost_cryray { namespace python { 

namespace detail
{
  // Adds an additional layer of binding to
  // objects::make_iterator(...), which allows us to pass member
  // function and member data pointers.
  template <class NextPolicies, class Target, class Accessor1, class Accessor2>
  inline object make_iterator(
      Accessor1 get_start, Accessor2 get_finish, boost_cryray::type<Target>* target = 0, NextPolicies* = 0)
  {
      return objects::make_iterator_function<NextPolicies,Target>(
          boost_cryray::protect(boost_cryray::bind(get_start, _1))
          , boost_cryray::protect(boost_cryray::bind(get_finish, _1))
          );
  }

  // Guts of template class iterators<>, below.
  template <bool const_ = false>
  struct iterators_impl
  {
      template <class T>
      struct apply
      {
          typedef typename T::iterator iterator;
          static iterator begin(T& x) { return x.begin(); }
          static iterator end(T& x) { return x.end(); }
      };
  };

  template <>
  struct iterators_impl<true>
  {
      template <class T>
      struct apply
      {
          typedef typename T::const_iterator iterator;
          static iterator begin(T& x) { return x.begin(); }
          static iterator end(T& x) { return x.end(); }
      };
  };
}

// An "ordinary function generator" which contains static begin(x) and
// end(x) functions that invoke T::begin() and T::end(), respectively.
template <class T>
struct iterators
    : detail::iterators_impl<
        boost_cryray::is_const<T>::value
      >::template apply<T>
{
};

// Create an iterator-building function which uses the given
// accessors. Deduce the Target type from the accessors. The iterator
// returns copies of the inderlying elements.
template <class Accessor1, class Accessor2>
object range(Accessor1 start, Accessor2 finish)
{
    return detail::make_iterator<objects::default_iterator_call_policies>(
                start, finish
                , detail::target(start));
}

// Create an iterator-building function which uses the given accessors
// and next() policies. Deduce the Target type.
template <class NextPolicies, class Accessor1, class Accessor2>
object range(Accessor1 start, Accessor2 finish, NextPolicies* = 0)
{
    return detail::make_iterator<NextPolicies>(start, finish, detail::target(start));
}

// Create an iterator-building function which uses the given accessors
// and next() policies, operating on the given Target type
template <class NextPolicies, class Target, class Accessor1, class Accessor2>
object range(Accessor1 start, Accessor2 finish, NextPolicies* = 0, boost_cryray::type<Target>* = 0)
{
    typedef typename add_reference<Target>::type target;
    return detail::make_iterator<NextPolicies, target>(start, finish);
}

// A Python callable object which produces an iterator traversing
// [x.begin(), x.end()), where x is an instance of the Container
// type. NextPolicies are used as the CallPolicies for the iterator's
// next() function.
template <class Container
          , class NextPolicies = objects::default_iterator_call_policies>
struct iterator : object
{
    iterator()
        : object(
            python::range<NextPolicies>(
                &iterators<Container>::begin, &iterators<Container>::end
                ))
    {
    }
};

}} // namespace boost_cryray::python

#endif // ITERATOR_DWA2002512_HPP
