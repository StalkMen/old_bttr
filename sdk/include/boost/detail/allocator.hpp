/*
 *
 * Copyright (c) 2001
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef BOOST_DETAIL_ALLOCATOR_HPP
#define BOOST_DETAIL_ALLOCATOR_HPP

#include <boost/config.hpp>
#include <cstdlib>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
using ::ptrdiff_t;
using ::size_t;
}
#endif

// see if we have SGI alloc class:
#if defined(BOOST_NO_STD_ALLOCATOR) && (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION) || defined(__GLIBCPP__) || defined(__STL_CONFIG_H))
#  define BOOST_HAVE_SGI_ALLOCATOR
#  include <memory>
#  if defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)
namespace boost_cryray{ namespace detail{
   typedef std::__sgi_alloc alloc_type;
}}
#  else
namespace boost_cryray{ namespace detail{
   typedef std::alloc alloc_type;
}}
#  endif
#endif


namespace boost_cryray{ namespace detail{

template <class T>
void allocator_construct(T* p, const T& t)
{ new (p) T(t); }

template <class T>
void allocator_destroy(T* p)
{ 
   (void)p; // warning suppression
   p->~T(); 
}

} }

#if !defined(BOOST_NO_STD_ALLOCATOR)

#include <memory>

#define BOOST_DEFAULT_ALLOCATOR(T) std::allocator< T >

namespace boost_cryray{ namespace detail{

template <class T, class A>
struct rebind_allocator
{
   typedef typename A::template rebind<T> binder;
   typedef typename binder::other type;
};

} // namespace detail
} // namespace boost_cryray

#elif !defined(BOOST_NO_MEMBER_TEMPLATES)

// no std::allocator, but the compiler supports the necessary syntax,
// write our own allocator instead:

#define BOOST_DEFAULT_ALLOCATOR(T) ::boost_cryray::detail::allocator< T >

namespace boost_cryray{ namespace detail{

template <class T>
class allocator
{
public:

   typedef T              value_type;
   typedef value_type *   pointer;
   typedef const T*       const_pointer;
   typedef T&             reference;
   typedef const T&       const_reference;
   typedef std::size_t    size_type;
   typedef std::ptrdiff_t difference_type;

   template <class U>
   struct rebind
   {
      typedef allocator<U> other;
   };

   allocator(){}
   
   template <class U>
   allocator(const allocator<U>&){}

   allocator(const allocator&){}

   template <class U>
   allocator& operator=(const allocator<U>&)
   { return *this; }

   ~allocator(){}

   pointer address(reference x) { return &x; }

   const_pointer address(const_reference x) const { return &x; }

   pointer allocate(size_type n, const void* = 0) 
   {
      #ifdef BOOST_HAVE_SGI_ALLOCATOR
      return n != 0 ?
         reinterpret_cast<pointer>(alloc_type::allocate(n * sizeof(value_type)))
         : 0;
      #else
      return n != 0 ?
         reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)))
         : 0;
      #endif
   }

   void deallocate(pointer p, size_type n) 
   {
      #ifdef BOOST_HAVE_SGI_ALLOCATOR
      assert( (p == 0) == (n == 0) );
      if (p != 0)
         alloc_type::deallocate((void*)p, n);
      #else
      assert( (p == 0) == (n == 0) );
      if (p != 0)
         ::operator delete((void*)p);
      #endif
   }

   size_type max_size() const
   { return size_t(-1) / sizeof(value_type); }

   void construct(pointer p, const T& val) const
   { allocator_construct(p, val); }

   void destroy(pointer p) const
   { allocator_destroy(p); }
};

template <class T, class A>
struct rebind_allocator
{
   typedef typename A::template rebind<T> binder;
   typedef typename binder::other type;
};

} // namespace detail
} // namespace boost_cryray

#else

// no std::allocator, use workaround version instead,
// each allocator class must derive from a base class
// that allocates blocks of bytes:

#define BOOST_DEFAULT_ALLOCATOR(T) ::boost_cryray::detail::allocator_adapter<T, ::boost_cryray::detail::simple_alloc>

namespace boost_cryray{ namespace detail{

class simple_alloc
{
public:

   typedef void           value_type;
   typedef value_type *   pointer;
   typedef const void*    const_pointer;
   typedef std::size_t    size_type;
   typedef std::ptrdiff_t difference_type;

   simple_alloc(){}
   simple_alloc(const simple_alloc&){}

   ~simple_alloc(){}

   pointer allocate(size_type n, const void* = 0) 
   {
      #ifdef BOOST_HAVE_SGI_ALLOCATOR
      return n != 0 ?
         reinterpret_cast<pointer>(alloc_type::allocate(n))
         : 0;
      #else
      return n != 0 ?
         reinterpret_cast<pointer>(::operator new(n))
         : 0;
      #endif
   }

   void deallocate(pointer p, size_type n) 
   {
      #ifdef BOOST_HAVE_SGI_ALLOCATOR
      assert( (p == 0) == (n == 0) );
      if (p != 0)
         alloc_type::deallocate((void*)p, n);
      #else
      assert( (p == 0) == (n == 0) );
      if (p != 0)
         ::operator delete((void*)p);
      #endif
   }
};

template <class T, class Base>
class allocator_adapter : public Base
{
public:

   typedef T              value_type;
   typedef value_type *   pointer;
   typedef const T*       const_pointer;
   typedef T&             reference;
   typedef const T&       const_reference;
   typedef size_t         size_type;
   typedef std::ptrdiff_t difference_type;
   typedef Base      base_type;

   allocator_adapter(){}
   allocator_adapter(const base_type& x) : Base(x){}
   allocator_adapter& operator=(const base_type& x)
   {
      *(static_cast<base_type*>(this)) = x;
      return *this;
   }

   ~allocator_adapter(){}

   pointer address(reference x) { return &x; }

   const_pointer address(const_reference x) const { return &x; }

   pointer allocate(size_type n, const void* = 0) 
   {
      return n != 0 ?
         reinterpret_cast<pointer>(base_type::allocate(n * sizeof(value_type)))
         : 0;
   }

   void deallocate(pointer p, size_type n) 
   {
      assert( (p == 0) == (n == 0) );
      if (p != 0)
         static_cast<base_type*>(this)->deallocate((void*)p, n * sizeof(value_type));
   }

   size_type max_size() const
   { return size_t(-1) / sizeof(value_type); }

   void construct(pointer p, const T& val) const
   { allocator_construct(p, val); }

   void destroy(pointer p) const
   { allocator_destroy(p); }
};

template <class T, class A>
struct rebind_allocator
{
   typedef allocator_adapter<T, typename A::base_type> type;
};

} // namespace detail
} // namespace boost_cryray

#endif

#endif // include guard
