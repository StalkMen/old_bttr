#ifndef BOOST_PROPERTY_HPP
#define BOOST_PROPERTY_HPP

#include <boost/pending/ct_if.hpp>

namespace boost_cryray {

  struct no_property { 
    typedef no_property tag_type;
    typedef no_property next_type;
    typedef no_property value_type;
    enum { num = 0 };
    typedef void kind;
  };
  template <class Tag, class T, class Base = no_property>
  struct property : public Base {
    typedef Base next_type;
    typedef Tag tag_type;
    typedef T value_type;
    property() { }
    property(const T& v) : m_value(v) { }
    property(const T& v, const Base& b) : Base(b), m_value(v) { }
    // copy constructor and assignment operator will be generated by compiler
    T m_value;
  };

  // The BGL properties specialize property_kind and
  // property_num, and use enum's for the Property type (see
  // graph/properties.hpp), but the user may want to use a class
  // instead with a nested kind type and num.  Also, we may want to
  // switch BGL back to using class types for properties at some point.

  template <class PropertyTag>
  struct property_kind {
    typedef typename PropertyTag::kind type;
  };

  template <class P>
  struct has_property { 
    BOOST_STATIC_CONSTANT(bool, value = true);
    typedef true_type type;
  };
  template <>
  struct has_property<no_property> { 
    BOOST_STATIC_CONSTANT(bool, value = false); 
    typedef false_type type; 
  };

} // namespace boost_cryray

#include <boost/pending/detail/property.hpp>

namespace boost_cryray {

  template <class PropertyList, class Tag>
  struct property_value {
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    typedef typename detail::build_property_tag_value_alist<PropertyList>::type AList;
    typedef typename detail::extract_value<AList,Tag>::type type;
#else
    typedef typename detail::build_property_tag_value_alist<PropertyList>::type AList;
    typedef typename detail::ev_selector<AList>::type Extractor;
    typedef typename Extractor::template bind_<AList,Tag>::type type;
#endif  
  };

  template <class Tag1, class Tag2, class T1, class Base>
  inline typename property_value<property<Tag1,T1,Base>, Tag2>::type& 
  get_property_value(property<Tag1,T1,Base>& p, Tag2 tag2) {
    BOOST_STATIC_CONSTANT(bool, 
                          match = (detail::same_property<Tag1,Tag2>::value));
    typedef property<Tag1,T1,Base> Prop;
    typedef typename property_value<Prop, Tag2>::type T2;
    T2* t2 = 0;
    typedef detail::property_value_dispatch<match> Dispatcher;
    return Dispatcher::get_value(p, t2, tag2);
  }
  template <class Tag1, class Tag2, class T1, class Base>
  inline
  const typename property_value<property<Tag1,T1,Base>, Tag2>::type& 
  get_property_value(const property<Tag1,T1,Base>& p, Tag2 tag2) {
    BOOST_STATIC_CONSTANT(bool, 
                          match = (detail::same_property<Tag1,Tag2>::value));
    typedef property<Tag1,T1,Base> Prop;
    typedef typename property_value<Prop, Tag2>::type T2;
    T2* t2 = 0;
    typedef detail::property_value_dispatch<match> Dispatcher;
    return Dispatcher::const_get_value(p, t2, tag2);
  }

} // namesapce boost

#endif /* BOOST_PROPERTY_HPP */
