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

#ifndef COLLECTION_CONCEPT_RG103101_HPP
#define COLLECTION_CONCEPT_RG103101_HPP

#include "boost/concept_check.hpp"

namespace boost_cryray {
namespace detail {
namespace multi_array {

  //===========================================================================
  // Collection Concept

  template <class Collection>
  struct CollectionConcept
  {
    typedef typename Collection::value_type value_type;
    typedef typename Collection::iterator iterator;
    typedef typename Collection::const_iterator const_iterator;
    typedef typename Collection::reference reference;
    typedef typename Collection::const_reference const_reference;
    // typedef typename Collection::pointer pointer;
    typedef typename Collection::difference_type difference_type;
    typedef typename Collection::size_type size_type;

    void constraints() {
      boost_cryray::function_requires<boost_cryray::InputIteratorConcept<iterator> >();
      boost_cryray::function_requires<boost_cryray::InputIteratorConcept<const_iterator> >();
      boost_cryray::function_requires<boost_cryray::CopyConstructibleConcept<value_type> >();
      const_constraints(c);
      i = c.begin();
      i = c.end();
      c.swap(c);
    }
    void const_constraints(const Collection& c) {
      ci = c.begin();
      ci = c.end();
      n = c.size();
      b = c.empty();
    }
    Collection c;
    bool b;
    iterator i;
    const_iterator ci;
    size_type n;
  };

}
}
}
#endif // COLLECTION_CONCEPT_RG103101_HPP
