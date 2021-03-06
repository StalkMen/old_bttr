// Copyright (C) 2002 Ronald Garcia
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.
//

#ifndef BASE_RG071801_HPP
#define BASE_RG071801_HPP

//
// base.hpp - some implementation base classes for from which
// functionality is acquired
//

#include "boost/multi_array/extent_range.hpp"
#include "boost/multi_array/extent_gen.hpp"
#include "boost/multi_array/index_range.hpp"
#include "boost/multi_array/index_gen.hpp"
#include "boost/multi_array/storage_order.hpp"
#include "boost/multi_array/types.hpp"
#include "boost/config.hpp"
#include "boost/multi_array/iterator_adaptors.hpp"
#include "boost/static_assert.hpp"
#include "boost/type.hpp"
#include <cassert>
#include <cstddef>
#include <memory>

namespace boost_cryray {

/////////////////////////////////////////////////////////////////////////
// class declarations
/////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t NumDims,
  typename Allocator = std::allocator<T> >
class multi_array;

// This is a public interface for use by end users!
namespace multi_array_types {
  typedef boost_cryray::detail::multi_array::size_type size_type;
  typedef std::ptrdiff_t difference_type;
  typedef boost_cryray::detail::multi_array::index index;
  typedef detail::multi_array::index_range<index,size_type> index_range;
  typedef detail::multi_array::extent_range<index,size_type> extent_range;
  typedef detail::multi_array::index_gen<0,0> index_gen;
  typedef detail::multi_array::extent_gen<0> extent_gen;
}


// boost_cryray::extents and boost_cryray::indices are now a part of the public
// interface.  That way users don't necessarily have to create their 
// own objects.  On the other hand, one may not want the overhead of 
// object creation in small-memory environments.  Thus, the objects
// can be left undefined by defining BOOST_MULTI_ARRAY_NO_GENERATORS 
// before loading multi_array.hpp.
#if !BOOST_MULTI_ARRAY_NO_GENERATORS
namespace {
  multi_array_types::extent_gen extents;
  multi_array_types::index_gen indices;
}
#endif // BOOST_MULTI_ARRAY_NO_GENERATORS

namespace detail {
namespace multi_array {

template <typename T, std::size_t NumDims>
class sub_array;

template <typename T, std::size_t NumDims, typename TPtr = const T*>
class const_sub_array;

template <typename T, std::size_t NumDims, typename value_type,
  typename reference_type, typename tag, typename difference_type>
struct iterator_generator;

template <typename T, std::size_t NumDims, typename value_type,
  typename reference_type, typename tag, typename difference_type>
struct const_iterator_generator;

template <typename T, std::size_t NumDims, typename value_type,
  typename reference_type, typename tag, typename difference_type>
struct reverse_iterator_generator;

template <typename T, std::size_t NumDims, typename value_type,
  typename reference_type, typename tag, typename difference_type>
struct const_reverse_iterator_generator;

template <typename T,typename TPtr>
struct iterator_base;

template <typename T, std::size_t NumDims>
struct iterator_policies;

template <typename T, std::size_t NumDims, typename TPtr = const T*>
class const_multi_array_view;

template <typename T, std::size_t NumDims>
class multi_array_view;

/////////////////////////////////////////////////////////////////////////
// class interfaces
/////////////////////////////////////////////////////////////////////////

class multi_array_base {
public:
  typedef multi_array_types::size_type size_type;
  typedef multi_array_types::difference_type difference_type;
  typedef multi_array_types::index index;
  typedef multi_array_types::index_range index_range;
  typedef multi_array_types::extent_range extent_range;
  typedef multi_array_types::index_gen index_gen;
  typedef multi_array_types::extent_gen extent_gen;
};

//
// value_accessor_n
//  contains the routines for accessing elements from
//  N-dimensional views.
//
template<typename T, std::size_t NumDims>
class value_accessor_n : public multi_array_base {
  typedef multi_array_base super_type;
public:
  typedef typename super_type::index index;

  // 
  // public typedefs used by classes that inherit from this base
  //
  typedef T element;
  typedef boost_cryray::multi_array<T,NumDims-1> value_type;
  typedef sub_array<T,NumDims-1> reference;
  typedef const_sub_array<T,NumDims-1> const_reference;

protected:
  // used by array operator[] and iterators to get reference types.
  template <typename Reference, typename TPtr>
  Reference access(boost_cryray::type<Reference>,index idx,TPtr base,
                   const size_type* extents,
                   const index* strides,
                   const index* index_base) const {

    // return a sub_array<T,NDims-1> proxy object
    TPtr newbase = base + idx * strides[0];
    return Reference(newbase,extents+1,strides+1,index_base+1);

  }

  value_accessor_n() { }
  ~value_accessor_n() { }
};



//
// value_accessor_one
//  contains the routines for accessing reference elements from
//  1-dimensional views.
//
template<typename T>
class value_accessor_one : public multi_array_base {
  typedef multi_array_base super_type;
public:
  typedef typename super_type::index index;
  //
  // public typedefs for use by classes that inherit it.
  //
  typedef T element;
  typedef T value_type;
  typedef T& reference;
  typedef T const& const_reference;

protected:
  // used by array operator[] and iterators to get reference types.
  template <typename Reference, typename TPtr>
  Reference access(boost_cryray::type<Reference>,index idx,TPtr base,
                   const size_type*,
                   const index* strides,
                   const index*) const {
    return *(base + idx * strides[0]);
  }

  value_accessor_one() { }
  ~value_accessor_one() { }
};


/////////////////////////////////////////////////////////////////////////
// choose value accessor begins
//

struct choose_value_accessor_n {
  template <typename T, std::size_t NumDims>
  struct bind {
    typedef value_accessor_n<T,NumDims> type;
  };
};

struct choose_value_accessor_one {
  template <typename T, std::size_t NumDims>
  struct bind {
    typedef value_accessor_one<T> type;
  };
};


template <std::size_t NumDims>
struct value_accessor_gen_helper {
  typedef choose_value_accessor_n choice;
};

template <>
struct value_accessor_gen_helper<1> {
  typedef choose_value_accessor_one choice;
};

template <typename T, std::size_t NumDims>
struct value_accessor_generator {
private:
  typedef typename value_accessor_gen_helper<NumDims>::choice Choice;
public:
  typedef typename Choice::template bind<T,NumDims>::type type;
};

//
// choose value accessor ends
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// multi_array/sub_array base stuffs
/////////////////////////////////////////////////////////////////////////
template <std::size_t NumDims>
struct iterator_tag_selector {
  typedef std::input_iterator_tag type;
};

template <>
struct iterator_tag_selector<1> {
  typedef std::random_access_iterator_tag type;
};


////////////////////////////////////////////////////////////////////////
// multi_array_base
////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t NumDims>
class multi_array_impl_base :
  public value_accessor_generator<T,NumDims>::type {
  typedef typename value_accessor_generator<T,NumDims>::type super_type;
public:
  typedef typename super_type::index index;
  typedef typename super_type::size_type size_type;
  typedef typename super_type::element element;
  typedef typename super_type::index_range index_range;
  typedef typename super_type::value_type value_type;
  typedef typename super_type::reference reference;
  typedef typename super_type::const_reference const_reference;

  template <std::size_t NDims>
  struct subarray {
    typedef boost_cryray::detail::multi_array::sub_array<T,NDims> type;
  };

  template <std::size_t NDims>
  struct const_subarray {
    typedef boost_cryray::detail::multi_array::const_sub_array<T,NDims> type;
  };

  template <std::size_t NDims>
  struct array_view {
    typedef boost_cryray::detail::multi_array::multi_array_view<T,NDims> type;
  };

  template <std::size_t NDims>
  struct const_array_view {
  public:
    typedef boost_cryray::detail::multi_array::const_multi_array_view<T,NDims> type;
  };

  //
  // iterator support
  //

  typedef typename iterator_tag_selector<NumDims>::type iterator_tag;

  typedef typename
    iterator_generator<T,NumDims,value_type,
    reference,iterator_tag,index>::type iterator;

  typedef typename
    const_iterator_generator<T,NumDims,value_type,
    const_reference,iterator_tag,index>::type const_iterator;

  typedef typename
    reverse_iterator_generator<T,NumDims,value_type,
    reference,iterator_tag,index>::type reverse_iterator;

  typedef typename
    const_reverse_iterator_generator<T,NumDims,value_type,
    const_reference,iterator_tag,index>::type const_reverse_iterator;

protected:
  typedef iterator_base<T,T*> iter_base;
  typedef iterator_base<T,const T*> const_iter_base;

  multi_array_impl_base() { }
  ~multi_array_impl_base() { }

  // Used by operator() in our array classes
  template <typename Reference, typename IndexList, typename TPtr>
  Reference access_element(boost_cryray::type<Reference>, TPtr base,
                           const IndexList& indices,
                           const index* strides) const {
    index offset = 0;
    for (size_type n = 0; n != NumDims; ++n) 
      offset += indices[n] * strides[n];
    
    return base[offset];
  }

  template <typename StrideList, typename ExtentList>
  void compute_strides(StrideList& stride_list, ExtentList& extent_list,
                       const general_storage_order<NumDims>& storage)
  {
    // invariant: stride = the stride for dimension n
    index stride = 1;
    for (size_type n = 0; n != NumDims; ++n) {
      index stride_sign = +1;
      
      if (!storage.ascending(storage.ordering(n)))
        stride_sign = -1;
      
      // The stride for this dimension is the product of the
      // lengths of the ranks minor to it.
      stride_list[storage.ordering(n)] = stride * stride_sign;
      
      stride *= extent_list[storage.ordering(n)];
    } 
  }

  // This calculates the offset to the array base pointer due to:
  // 1. dimensions stored in descending order
  // 2. non-zero dimension index bases
  template <typename StrideList, typename ExtentList, typename BaseList>
  index
  calculate_origin_offset(const StrideList& stride_list,
                          const ExtentList& extent_list,
                          const general_storage_order<NumDims>& storage,
                          const BaseList& index_base_list)
  {
    return
      calculate_descending_dimension_offset(stride_list,extent_list,
                                            storage) +
      calculate_indexing_offset(stride_list,index_base_list);
  }

  // This calculates the offset added to the base pointer that are
  // caused by descending dimensions
  template <typename StrideList, typename ExtentList>
  index
  calculate_descending_dimension_offset(const StrideList& stride_list,
                                const ExtentList& extent_list,
                                const general_storage_order<NumDims>& storage)
  {
    index offset = 0;
    if (!storage.all_dims_ascending()) 
      for (size_type n = 0; n != NumDims; ++n)
        if (!storage.ascending(n))
          offset -= (extent_list[n] - 1) * stride_list[n];

    return offset;
  }

  // This is used to reindex array_views, which are no longer
  // concerned about storage order (specifically, whether dimensions
  // are ascending or descending) since the viewed array handled it.

  template <typename StrideList, typename BaseList>
  index
  calculate_indexing_offset(const StrideList& stride_list,
                          const BaseList& index_base_list)
  {
    index offset = 0;
    for (size_type n = 0; n != NumDims; ++n)
        offset -= stride_list[n] * index_base_list[n];
    return offset;
  }

  // Slicing using an index_gen.
  // Note that populating an index_gen creates a type that encodes
  // both the number of dimensions in the current Array (NumDims), and 
  // the Number of dimensions for the resulting view.  This allows the 
  // compiler to fail if the dimensions aren't completely accounted
  // for.  For reasons unbeknownst to me, a BOOST_STATIC_ASSERT
  // within the member function template does not work. I should add a 
  // note to the documentation specifying that you get a damn ugly
  // error message if you screw up in your slicing code.
  template <typename ArrayRef, int NDims, typename TPtr>
  ArrayRef
  generate_array_view(boost_cryray::type<ArrayRef>,
                      const boost_cryray::detail::multi_array::
                      index_gen<NumDims,NDims>& indices,
                      const size_type* extents,
                      const index* strides,
                      const index* index_bases,
                      TPtr base) const {

    boost_cryray::array<index,NDims> new_strides;
    boost_cryray::array<index,NDims> new_extents;

    index offset = 0;
    size_type dim = 0;
    for (size_type n = 0; n != NumDims; ++n) {
      const index default_start = index_bases[n];
      const index default_finish = default_start+extents[n];
      const index_range& current_range = indices.ranges_[n];
      index start = current_range.get_start(default_start);
      index finish = current_range.get_finish(default_finish);
      index index_factor = current_range.stride();
      index len = (finish - start) / index_factor;

      // the array data pointer is modified to account for non-zero
      // bases during slicing (see [Garcia] for the math involved)
      offset += start * strides[n];

      if (!current_range.is_degenerate()) {

        // The index_factor for each dimension is included into the
        // strides for the array_view (see [Garcia] for the math involved).
        new_strides[dim] = index_factor * strides[n];
        
        // calculate new extents
        new_extents[dim] = len;
        ++dim;
      }
    }
    assert (dim == NDims);

    return
      ArrayRef(base+offset,
               new_extents,
               new_strides);
  }
                     

};

} // namespace multi_array
} // namespace detail

} // namespace boost_cryray

#endif // BASE_RG071801_HPP
