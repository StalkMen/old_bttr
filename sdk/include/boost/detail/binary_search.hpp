// Copyright (c)  2000 David Abrahams. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
// 
// Copyright (c) 1994
// Hewlett-Packard Company
// 
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hewlett-Packard Company makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.
// 
// Copyright (c) 1996
// Silicon Graphics Computer Systems, Inc.
// 
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Silicon Graphics makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.
// 
#ifndef BINARY_SEARCH_DWA_122600_H_
# define BINARY_SEARCH_DWA_122600_H_

# include <boost/detail/iterator.hpp>
# include <utility>

namespace boost_cryray { namespace detail {

template <class ForwardIter, class Tp>
ForwardIter lower_bound(ForwardIter first, ForwardIter last,
                             const Tp& val) 
{
    typedef detail::iterator_traits<ForwardIter> traits;
    
    typename traits::difference_type len = boost_cryray::detail::distance(first, last);
    typename traits::difference_type half;
    ForwardIter middle;

    while (len > 0) {
      half = len >> 1;
      middle = first;
      std::advance(middle, half);
      if (*middle < val) {
        first = middle;
        ++first;
        len = len - half - 1;
      }
      else
        len = half;
    }
    return first;
}

template <class ForwardIter, class Tp, class Compare>
ForwardIter lower_bound(ForwardIter first, ForwardIter last,
                              const Tp& val, Compare comp)
{
  typedef detail::iterator_traits<ForwardIter> traits;

  typename traits::difference_type len = boost_cryray::detail::distance(first, last);
  typename traits::difference_type half;
  ForwardIter middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    std::advance(middle, half);
    if (comp(*middle, val)) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class ForwardIter, class Tp>
ForwardIter upper_bound(ForwardIter first, ForwardIter last,
                           const Tp& val)
{
  typedef detail::iterator_traits<ForwardIter> traits;

  typename traits::difference_type len = boost_cryray::detail::distance(first, last);
  typename traits::difference_type half;
  ForwardIter middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    std::advance(middle, half);
    if (val < *middle)
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class Tp, class Compare>
ForwardIter upper_bound(ForwardIter first, ForwardIter last,
                           const Tp& val, Compare comp)
{
  typedef detail::iterator_traits<ForwardIter> traits;

  typename traits::difference_type len = boost_cryray::detail::distance(first, last);
  typename traits::difference_type half;
  ForwardIter middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    std::advance(middle, half);
    if (comp(val, *middle))
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class Tp>
std::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const Tp& val)
{
  typedef detail::iterator_traits<ForwardIter> traits;

  typename traits::difference_type len = boost_cryray::detail::distance(first, last);
  typename traits::difference_type half;
  ForwardIter middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    std::advance(middle, half);
    if (*middle < val) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (val < *middle)
      len = half;
    else {
      left = boost_cryray::detail::lower_bound(first, middle, val);
      std::advance(first, len);
      right = boost_cryray::detail::upper_bound(++middle, first, val);
      return std::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return std::pair<ForwardIter, ForwardIter>(first, first);
}

template <class ForwardIter, class Tp, class Compare>
std::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const Tp& val,
              Compare comp)
{
  typedef detail::iterator_traits<ForwardIter> traits;

  typename traits::difference_type len = boost_cryray::detail::distance(first, last);
  typename traits::difference_type half;
  ForwardIter middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    std::advance(middle, half);
    if (comp(*middle, val)) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (comp(val, *middle))
      len = half;
    else {
      left = boost_cryray::detail::lower_bound(first, middle, val, comp);
      std::advance(first, len);
      right = boost_cryray::detail::upper_bound(++middle, first, val, comp);
      return std::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return std::pair<ForwardIter, ForwardIter>(first, first);
}           

template <class ForwardIter, class Tp>
bool binary_search(ForwardIter first, ForwardIter last,
                   const Tp& val) {
  ForwardIter i = boost_cryray::detail::lower_bound(first, last, val);
  return i != last && !(val < *i);
}

template <class ForwardIter, class Tp, class Compare>
bool binary_search(ForwardIter first, ForwardIter last,
                   const Tp& val,
                   Compare comp) {
  ForwardIter i = boost_cryray::detail::lower_bound(first, last, val, comp);
  return i != last && !comp(val, *i);
}

}} // namespace boost_cryray::detail

#endif // BINARY_SEARCH_DWA_122600_H_
