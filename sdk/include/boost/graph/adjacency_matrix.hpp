//=======================================================================
// Copyright 2001 University of Notre Dame.
// Author: Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef BOOST_ADJACENCY_MATRIX_HPP
#define BOOST_ADJACENCY_MATRIX_HPP

#include <boost/config.hpp>
#include <vector>
#include <memory>
#include <cassert>
#include <boost/iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/pending/ct_if.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/detail/edge.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/graph/properties.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost_cryray {
  
  namespace detail {

    template <class Directed, class Vertex>
    class matrix_edge_desc_impl : public edge_desc_impl<Directed,Vertex>
    {
      typedef edge_desc_impl<Directed,Vertex> Base;
    public:
      matrix_edge_desc_impl() { }
      matrix_edge_desc_impl(bool exists, Vertex s, Vertex d, 
                            const void* ep = 0)
        : Base(s, d, ep), m_exists(exists) { }
      bool exists() const { return m_exists; }
    private:
      bool m_exists;
    };

    struct does_edge_exist {
      template <class Edge>
      bool operator()(const Edge& e) const { return e.exists(); }
    };

    template <typename EdgeProperty>
    bool get_edge_exists(const std::pair<bool, EdgeProperty>& stored_edge) {
      return stored_edge.first;
    }
    template <typename EdgeProperty>
    void set_edge_exists(std::pair<bool, EdgeProperty>& stored_edge, 
                         bool flag) {
      stored_edge.first = flag;
    }

    template <typename EdgeProxy>
    bool get_edge_exists(const EdgeProxy& edge_proxy) {
      return edge_proxy;
    }
    template <typename EdgeProxy>
    EdgeProxy& set_edge_exists(EdgeProxy& edge_proxy, bool flag) {
      edge_proxy = flag;
      return edge_proxy; // just to avoid never used warning
    }



    template <typename EdgeProperty>
    const EdgeProperty&
    get_property(const std::pair<bool, EdgeProperty>& stored_edge) {
      return stored_edge.second;
    }
    template <typename EdgeProperty>
    EdgeProperty&
    get_property(std::pair<bool, EdgeProperty>& stored_edge) {
      return stored_edge.second;
    }

    template <typename EdgeProperty>
    inline void
    set_property(std::pair<bool, EdgeProperty>& stored_edge, 
                 const EdgeProperty& ep) {
      stored_edge.second = ep;
    }

    inline const no_property& get_property(const char&) {
      static no_property s_prop;
      return s_prop;
    }
    inline no_property& get_property(char&) {
      static no_property s_prop;
      return s_prop;
    }
    template <typename EdgeProxy, typename EdgeProperty>
    inline void
    set_property(EdgeProxy, const EdgeProperty&) { }
    
    //=======================================================================
    // Directed Out Edge Iterator

    template <typename VertexDescriptor, typename MatrixIter, 
              typename VerticesSizeType>
    struct dir_adj_matrix_out_edge_iter_policies 
      : public default_iterator_policies 
    {
      dir_adj_matrix_out_edge_iter_policies() { }
      dir_adj_matrix_out_edge_iter_policies(const VertexDescriptor& src,
                                            const VerticesSizeType& n)
        : m_src(src), m_targ(0), m_n(n) { }

      template <typename Iter>
      void increment(Iter& i) {
        ++i.base();
        ++m_targ;
      }
      template <typename Iter>
      inline typename Iter::value_type
      dereference(const Iter& i) const 
      {
        typedef typename Iter::value_type EdgeDescriptor;
        return EdgeDescriptor(get_edge_exists(*i.base()), m_src, m_targ, 
                              &get_property(*i.base()));
      }
      VertexDescriptor m_src, m_targ;
      VerticesSizeType m_n;
    };

    //=======================================================================
    // Undirected Out Edge Iterator

    template <typename VertexDescriptor, typename MatrixIter, 
              typename VerticesSizeType>
    struct undir_adj_matrix_out_edge_iter_policies 
    {
      undir_adj_matrix_out_edge_iter_policies() { }
      undir_adj_matrix_out_edge_iter_policies(const VertexDescriptor& src,
                                              const VerticesSizeType& n)
        : m_src(src), m_inc(src), m_targ(0), m_n(n) { }

      void initialize(MatrixIter&) const{ }

      template <typename Iter>
      void increment(Iter& i) {
        if (m_targ < m_src)     // first half
          ++i.base();
        else {                  // second half
          ++m_inc;
          i.base() += m_inc;
        }
        ++m_targ;
      }
      template <typename Iter>
      inline typename Iter::value_type
      dereference(const Iter& i) const 
      {
        typedef typename Iter::value_type EdgeDescriptor;
        return EdgeDescriptor(get_edge_exists(*i.base()), m_src, m_targ,
                              &get_property(*i.base()));
      }
      template <typename Iter>
      bool equal(const Iter& x, const Iter& y) const
      { return x.base() == y.base(); }

      VertexDescriptor m_src, m_inc, m_targ;
      VerticesSizeType m_n;
    };

    //=======================================================================
    // Edge Iterator

    template <typename Directed, typename MatrixIter, 
              typename VerticesSizeType>
    struct adj_matrix_edge_iter_policies
      : public default_iterator_policies 
    {
      adj_matrix_edge_iter_policies() { }
      adj_matrix_edge_iter_policies(const MatrixIter& start,
                                    const VerticesSizeType& n) 
        : m_start(start), m_src(0), m_targ(0), m_n(n) { }

      void initialize(MatrixIter&) const{ }

      template <typename Iter>
      void increment(Iter& i) {
        increment_dispatch(i.base(), Directed());
      }
      void increment_dispatch(MatrixIter& i, directedS) {
        ++i;
        if (m_targ == m_n - 1) {
          m_targ = 0;
          ++m_src;
        } else
          ++m_targ;
      }
      void increment_dispatch(MatrixIter& i, undirectedS) {
        ++i;
        if (m_targ == m_src) {
          m_targ = 0;
          ++m_src;
        } else
          ++m_targ;
      }

      template <typename Iter>
      inline typename Iter::value_type
      dereference(const Iter& i) const 
      {
        typedef typename Iter::value_type EdgeDescriptor;
        return EdgeDescriptor(get_edge_exists(*i.base()), m_src, m_targ,
                              &get_property(*i.base()));
      }
      
      MatrixIter m_start;
      VerticesSizeType m_src, m_targ, m_n;
    };

  } // namespace detail

  //=========================================================================
  // Adjacency Matrix Traits
  template <typename Directed = directedS>
  class adjacency_matrix_traits {
    typedef typename Directed::is_bidir_t is_bidir;
    typedef typename Directed::is_directed_t is_directed;
  public:
    typedef typename boost_cryray::ct_if_t<is_bidir,
      bidirectional_tag,
      typename boost_cryray::ct_if_t<is_directed,
        directed_tag, undirected_tag
      >::type
    >::type directed_category;
    
    typedef disallow_parallel_edge_tag edge_parallel_category;
    
    typedef std::size_t vertex_descriptor;

    typedef detail::matrix_edge_desc_impl<directed_category,
      vertex_descriptor> edge_descriptor;
  };

  struct adjacency_matrix_class_tag { };

  struct adj_matrix_traversal_tag :
    public virtual adjacency_matrix_tag,
    public virtual vertex_list_graph_tag,
    public virtual incidence_graph_tag,
    public virtual adjacency_graph_tag,
    public virtual edge_list_graph_tag { };
  
  //=========================================================================
  // Adjacency Matrix Class
  template <typename Directed = directedS,
            typename VertexProperty = no_property,
            typename EdgeProperty = no_property,
            typename GraphProperty = no_property,
            typename Allocator = std::allocator<bool> >
  class adjacency_matrix {
    typedef adjacency_matrix self;
    typedef adjacency_matrix_traits<Directed> Traits;
  public: // should be private
    typedef typename ct_if_t<typename has_property<EdgeProperty>::type,
      std::pair<bool, EdgeProperty>, char>::type StoredEdge;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
    typedef std::vector<StoredEdge> Matrix;
#else
    // This causes internal compiler error for MSVC
    typedef typename Allocator::template rebind<StoredEdge>::other Alloc;
    typedef std::vector<StoredEdge, Alloc> Matrix;
#endif
    typedef typename Matrix::iterator MatrixIter;
    typedef typename Matrix::size_type size_type;
  public:
    // Graph concept required types
    typedef typename Traits::vertex_descriptor vertex_descriptor;
    typedef typename Traits::edge_descriptor edge_descriptor;
    typedef typename Traits::directed_category directed_category;
    typedef typename Traits::edge_parallel_category edge_parallel_category;
    typedef adj_matrix_traversal_tag traversal_category;

    //private: if friends worked, these would be private

    typedef detail::dir_adj_matrix_out_edge_iter_policies<
      vertex_descriptor, MatrixIter, size_type> DirOutEdgePolicies;

    typedef detail::undir_adj_matrix_out_edge_iter_policies<
      vertex_descriptor, MatrixIter, size_type> UnDirOutEdgePolicies;

    typedef typename ct_if_t<typename Directed::is_directed_t,
      DirOutEdgePolicies, UnDirOutEdgePolicies>::type OutEdgePolicies;
    typedef iterator_adaptor<MatrixIter, OutEdgePolicies,
      edge_descriptor, edge_descriptor, edge_descriptor*,
      multi_pass_input_iterator_tag, std::ptrdiff_t> unfiltered_out_edge_iter;
    
    typedef detail::adj_matrix_edge_iter_policies<
      Directed, MatrixIter, size_type> EdgePolicies;
    typedef iterator_adaptor<MatrixIter, EdgePolicies,
      edge_descriptor, edge_descriptor, edge_descriptor*,
      multi_pass_input_iterator_tag, std::ptrdiff_t> unfiltered_edge_iter;
    
    typedef filter_iterator_generator<detail::does_edge_exist,
      unfiltered_out_edge_iter, edge_descriptor, edge_descriptor,
      edge_descriptor*, multi_pass_input_iterator_tag, std::ptrdiff_t>
      OutEdgeFilterGen;
    typedef typename OutEdgeFilterGen::type::policies_type
      FilteredOutEdgePolicy;

    typedef filter_iterator_generator<detail::does_edge_exist,
      unfiltered_edge_iter, edge_descriptor, edge_descriptor, edge_descriptor*,
      multi_pass_input_iterator_tag, std::ptrdiff_t> EdgeFilterGen;
    typedef typename EdgeFilterGen::type::policies_type FilteredEdgePolicy;
  public:

    // IncidenceGraph concept required types
    typedef typename OutEdgeFilterGen::type out_edge_iterator;

    typedef size_type degree_size_type;

    // BidirectionalGraph required types
    typedef void in_edge_iterator;

    // AdjacencyGraph required types
    typedef typename adjacency_iterator_generator<self,
      vertex_descriptor, out_edge_iterator>::type adjacency_iterator;

    // VertexListGraph required types
    typedef size_type vertices_size_type;
    typedef integer_range<vertex_descriptor> VertexList;
    typedef typename VertexList::iterator vertex_iterator;

    // EdgeListGrpah required types
    typedef size_type edges_size_type;
    typedef typename EdgeFilterGen::type edge_iterator;

    // PropertyGraph required types
    typedef EdgeProperty edge_property_type;
    typedef VertexProperty vertex_property_type;
    typedef adjacency_matrix_class_tag graph_tag;

    // Constructor required by MutableGraph
    adjacency_matrix(vertices_size_type n_vertices) 
      : m_matrix(Directed::is_directed ? 
                 (n_vertices * n_vertices)
                 : (n_vertices * (n_vertices + 1) / 2)),
      m_vertex_set(0, n_vertices),
      m_vertex_properties(n_vertices) { }


    //private: if friends worked, these would be private

    typename Matrix::const_reference
    get_edge(vertex_descriptor u, vertex_descriptor v) const {
      if (Directed::is_directed)
        return m_matrix[u * m_vertex_set.size() + v];
      else {
        if (v > u)
          std::swap(u, v);
        return m_matrix[u * (u - 1)/2 + v];
      }
    }
    typename Matrix::reference
    get_edge(vertex_descriptor u, vertex_descriptor v) {
      if (Directed::is_directed)
        return m_matrix[u * m_vertex_set.size() + v];
      else {
        if (v > u)
          std::swap(u, v);
        return m_matrix[u * (u + 1)/2 + v];
      }
    }

    Matrix m_matrix;
    VertexList m_vertex_set;
    std::vector<VertexProperty> m_vertex_properties;
  };
  
  //=========================================================================
  // Functions required by the AdjacencyMatrix concept 

  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor,
            bool>
  edge(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
       typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor v,
       const adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    bool exists = detail::get_edge_exists(g.get_edge(u,v));
    typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor
      e(exists, u, v, &detail::get_property(g.get_edge(u,v)));
    return std::make_pair(e, exists);
  }

  //=========================================================================
  // Functions required by the IncidenceGraph concept 

  // O(1)
  template <typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<directedS,VP,EP,GP,A>::out_edge_iterator,
            typename adjacency_matrix<directedS,VP,EP,GP,A>::out_edge_iterator>
  out_edges
    (typename adjacency_matrix<directedS,VP,EP,GP,A>::vertex_descriptor u,
     const adjacency_matrix<directedS,VP,EP,GP,A>& g_)
  {
    typedef adjacency_matrix<directedS,VP,EP,GP,A> Graph;
    Graph& g = const_cast<Graph&>(g_);
    typename Graph::vertices_size_type offset = u * g.m_vertex_set.size();
    typename Graph::MatrixIter f = g.m_matrix.begin() + offset;
    typename Graph::MatrixIter l = f + g.m_vertex_set.size();
    typename Graph::OutEdgePolicies oep(u, g.m_vertex_set.size());
    typename Graph::unfiltered_out_edge_iter first(f, oep), last(l, oep);
    detail::does_edge_exist pred;
    typename Graph::FilteredOutEdgePolicy fp(pred, last);
    typedef typename Graph::out_edge_iterator out_edge_iterator;
    return std::make_pair(out_edge_iterator(first, fp), 
                          out_edge_iterator(last, fp));
  }

  // O(1)
  template <typename VP, typename EP, typename GP, typename A>
  std::pair<
    typename adjacency_matrix<undirectedS,VP,EP,GP,A>::out_edge_iterator,
    typename adjacency_matrix<undirectedS,VP,EP,GP,A>::out_edge_iterator>
  out_edges
    (typename adjacency_matrix<undirectedS,VP,EP,GP,A>::vertex_descriptor u,
     const adjacency_matrix<undirectedS,VP,EP,GP,A>& g_)
  {
    typedef adjacency_matrix<undirectedS,VP,EP,GP,A> Graph;
    Graph& g = const_cast<Graph&>(g_);
    typename Graph::vertices_size_type offset = u * (u + 1) / 2;
    typename Graph::MatrixIter f = g.m_matrix.begin() + offset;
    typename Graph::MatrixIter l = g.m_matrix.end() + u;
    typename Graph::OutEdgePolicies oep(u, g.m_vertex_set.size());
    typename Graph::unfiltered_out_edge_iter first(f, oep), last(l, oep);
    detail::does_edge_exist pred;
    typename Graph::FilteredOutEdgePolicy fp(pred, last);
    typedef typename Graph::out_edge_iterator out_edge_iterator;
    return std::make_pair(out_edge_iterator(first, fp), 
                          out_edge_iterator(last, fp));
  }
  
  // O(N)
  template <typename D, typename VP, typename EP, typename GP, typename A>  
  typename adjacency_matrix<D,VP,EP,GP,A>::degree_size_type
  out_degree(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
             const adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    typename adjacency_matrix<D,VP,EP,GP,A>::degree_size_type n = 0;
    typename adjacency_matrix<D,VP,EP,GP,A>::out_edge_iterator f, l;
    for (tie(f, l) = out_edges(u, g); f != l; ++f)
      ++n;
    return n;
  }

  // O(1)
  template <typename D, typename VP, typename EP, typename GP, typename A,
    typename Dir, typename Vertex>  
  typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor
  source(const detail::matrix_edge_desc_impl<Dir,Vertex>& e,
         const adjacency_matrix<D,VP,EP,GP,A>&)
  {
    return e.m_source;
  }

  // O(1)
  template <typename D, typename VP, typename EP, typename GP, typename A,
    typename Dir, typename Vertex>  
  typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor
  target(const detail::matrix_edge_desc_impl<Dir,Vertex>& e,
         const adjacency_matrix<D,VP,EP,GP,A>&)
  {
    return e.m_target;
  }

  //=========================================================================
  // Functions required by the AdjacencyGraph concept 

  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::adjacency_iterator,
            typename adjacency_matrix<D,VP,EP,GP,A>::adjacency_iterator>
  adjacent_vertices
    (typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
     const adjacency_matrix<D,VP,EP,GP,A>& g_)
  {
      typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      typedef typename Graph::adjacency_iterator adjacency_iterator;
      typename Graph::out_edge_iterator first, last;
      boost_cryray::tie(first, last) = out_edges(u, g);
      return std::make_pair(adjacency_iterator(first, &g),
                            adjacency_iterator(last, &g));
  }

  //=========================================================================
  // Functions required by the VertexListGraph concept 

  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::vertex_iterator,
            typename adjacency_matrix<D,VP,EP,GP,A>::vertex_iterator>
  vertices(const adjacency_matrix<D,VP,EP,GP,A>& g_) {
    typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
    Graph& g = const_cast<Graph&>(g_);
    return std::make_pair(g.m_vertex_set.begin(), g.m_vertex_set.end());
  }

  template <typename D, typename VP, typename EP, typename GP, typename A>
  typename adjacency_matrix<D,VP,EP,GP,A>::vertices_size_type
  num_vertices(const adjacency_matrix<D,VP,EP,GP,A>& g) {
    return g.m_vertex_set.size();
  }
  
  //=========================================================================
  // Functions required by the EdgeListGraph concept 
  
  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::edge_iterator,
            typename adjacency_matrix<D,VP,EP,GP,A>::edge_iterator>
  edges(const adjacency_matrix<D,VP,EP,GP,A>& g_)
  {
    typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
    Graph& g = const_cast<Graph&>(g_);
    typename Graph::EdgePolicies ep(g.m_matrix.begin(), 
                                    g.m_vertex_set.size()); 
    typename Graph::unfiltered_edge_iter
      first(g.m_matrix.begin(), ep),
      last(g.m_matrix.end(), ep);
    detail::does_edge_exist pred;
    typename Graph::FilteredEdgePolicy fp(pred, last);
    typedef typename Graph::edge_iterator edge_iterator;
    return std::make_pair(edge_iterator(first, fp),
                          edge_iterator(last, fp));
  }

  template <typename D, typename VP, typename EP, typename GP, typename A>
  typename adjacency_matrix<D,VP,EP,GP,A>::edges_size_type
  num_edges(const adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
    typename Graph::edges_size_type num_e = 0;
    typename Graph::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
      num_e += out_degree(*vi, g);
    return num_e;
  }
  
  //=========================================================================
  // Functions required by the MutableGraph concept 

  // O(1)
  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor, bool>
  add_edge(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
           typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor v,
           const EP& ep,
           adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    typedef typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor 
      edge_descriptor;
    if (detail::get_edge_exists(g.get_edge(u,v)) == false) {
      detail::set_property(g.get_edge(u,v), ep);
      detail::set_edge_exists(g.get_edge(u,v), true);
      return std::make_pair
        (edge_descriptor(true, u, v, &detail::get_property(g.get_edge(u,v))), 
         true);
    } else
      return std::make_pair
        (edge_descriptor(true, u, v, &detail::get_property(g.get_edge(u,v))),
         false);
  }
  // O(1)
  template <typename D, typename VP, typename EP, typename GP, typename A>
  std::pair<typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor, bool>
  add_edge(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
           typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor v,
           adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    EP ep;
    return add_edge(u, v, ep, g);
  }

  // O(1)  
  template <typename D, typename VP, typename EP, typename GP, typename A>
  void
  remove_edge(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
              typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor v,
              adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    detail::set_edge_exists(g.get_edge(u,v), false);
  }

  // O(1)
  template <typename D, typename VP, typename EP, typename GP, typename A>
  void
  remove_edge(typename adjacency_matrix<D,VP,EP,GP,A>::edge_descriptor e,
              adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    remove_edge(source(e, g), target(e, g), g);
  }

  
  template <typename D, typename VP, typename EP, typename GP, typename A>
  inline typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor
  add_vertex(adjacency_matrix<D,VP,EP,GP,A>& g) {
    // UNDER CONSTRUCTION
    assert(false);
    return *vertices(g).first;
  }

  template <typename D, typename VP, typename EP, typename GP, typename A>
  inline typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor
  add_vertex(const VP& vp, adjacency_matrix<D,VP,EP,GP,A>& g) {
    // UNDER CONSTRUCTION
    assert(false);
    return *vertices(g).first;
  }

  template <typename D, typename VP, typename EP, typename GP, typename A>
  inline void
  remove_vertex(typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor u,
                adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    // UNDER CONSTRUCTION
    assert(false);
  }

  // O(V)
  template <typename VP, typename EP, typename GP, typename A>
  void
  clear_vertex
    (typename adjacency_matrix<directedS,VP,EP,GP,A>::vertex_descriptor u,
     adjacency_matrix<directedS,VP,EP,GP,A>& g)
  {
    typename adjacency_matrix<directedS,VP,EP,GP,A>::vertex_iterator 
      vi, vi_end;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
      remove_edge(u, *vi, g);
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
      remove_edge(*vi, u, g);
  }

  // O(V)
  template <typename VP, typename EP, typename GP, typename A>
  void
  clear_vertex
    (typename adjacency_matrix<undirectedS,VP,EP,GP,A>::vertex_descriptor u,
     adjacency_matrix<undirectedS,VP,EP,GP,A>& g)
  {
    typename adjacency_matrix<undirectedS,VP,EP,GP,A>::vertex_iterator
      vi, vi_end;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
      remove_edge(u, *vi, g);
  }

  //=========================================================================
  // Vertex Property Map
  
  template <typename GraphPtr, typename Vertex, typename T, typename R, 
    typename Tag> 
  class adj_matrix_vertex_property_map 
    : public put_get_helper<R,
         adj_matrix_vertex_property_map<GraphPtr, Vertex, T, R, Tag> >
  {
  public:
    typedef T value_type;
    typedef R reference;
    typedef Vertex key_type;
    typedef boost_cryray::lvalue_property_map_tag category;
    adj_matrix_vertex_property_map() { }
    adj_matrix_vertex_property_map(GraphPtr g) : m_g(g) { }
    inline reference operator[](key_type v) const {
      return get_property_value(m_g->m_vertex_properties[v], Tag());
    }
    GraphPtr m_g;
  };

  template <class Property, class Vertex>
  struct adj_matrix_vertex_id_map
    : public boost_cryray::put_get_helper<Vertex,
        adj_matrix_vertex_id_map<Property, Vertex> >
  {
    typedef Vertex value_type;
    typedef Vertex reference;
    typedef Vertex key_type;
    typedef boost_cryray::readable_property_map_tag category;
     adj_matrix_vertex_id_map() { }
    template <class Graph>
    inline adj_matrix_vertex_id_map(const Graph&) { }
    inline value_type operator[](key_type v) const { return v; }
  };

  namespace detail {

    struct adj_matrix_any_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind_ {
        typedef typename property_value<Property,Tag>::type Value;
        typedef typename boost_cryray::graph_traits<Graph>::vertex_descriptor Vertex;
        
        typedef adj_matrix_vertex_property_map<Graph*, Vertex, Value, Value&,
          Tag> type;
        typedef adj_matrix_vertex_property_map<const Graph*, Vertex, Value, 
          const Value&, Tag> const_type;
      };
    };
    struct adj_matrix_id_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind_ {
        typedef typename Graph::vertex_descriptor Vertex;
        typedef adj_matrix_vertex_id_map<Property, Vertex> type;
        typedef adj_matrix_vertex_id_map<Property, Vertex> const_type;
      };
    };

    template <class Tag>
    struct adj_matrix_choose_vertex_pa_helper {
      typedef adj_matrix_any_vertex_pa type;
    };
    template <>
    struct adj_matrix_choose_vertex_pa_helper<vertex_index_t> {
      typedef adj_matrix_id_vertex_pa type;
    };

    template <class Tag, class Graph, class Property>
    struct adj_matrix_choose_vertex_pa {
      typedef typename adj_matrix_choose_vertex_pa_helper<Tag>::type Helper;
      typedef typename Helper::template bind_<Tag,Graph,Property> Bind;
      typedef typename Bind::type type;
      typedef typename Bind::const_type const_type;
    };

    struct adj_matrix_vertex_property_selector {
      template <class Graph, class Property, class Tag>
      struct bind_ {
        typedef adj_matrix_choose_vertex_pa<Tag,Graph,Property> Choice;
        typedef typename Choice::type type;
        typedef typename Choice::const_type const_type;
      };
    };

  } // namespace detail

  template <>
  struct vertex_property_selector<adjacency_matrix_class_tag> {
    typedef detail::adj_matrix_vertex_property_selector type;
  };
  
  //=========================================================================
  // Edge Property Map


  template <typename Directed, typename Property, typename Vertex, 
    typename T, typename R, typename Tag> 
  class adj_matrix_edge_property_map 
    : public put_get_helper<R,
         adj_matrix_edge_property_map<Directed, Property, Vertex, T, R, Tag> >
  {
  public:
    typedef T value_type;
    typedef R reference;
    typedef detail::matrix_edge_desc_impl<Directed, Vertex> key_type;
    typedef boost_cryray::lvalue_property_map_tag category;
    inline reference operator[](key_type e) const {
      Property& p = *(Property*)e.get_property();
      return get_property_value(p, Tag());
    }
  };
  struct adj_matrix_edge_property_selector {
    template <class Graph, class Property, class Tag>
    struct bind_ {
      typedef typename property_value<Property,Tag>::type T;
      typedef typename Graph::vertex_descriptor Vertex;
      typedef adj_matrix_edge_property_map<typename Graph::directed_category,
        Property, Vertex, T, T&, Tag> type;
      typedef adj_matrix_edge_property_map<typename Graph::directed_category,
        Property, Vertex, T, const T&, Tag> const_type;
    };
  };
  template <>
  struct edge_property_selector<adjacency_matrix_class_tag> {
    typedef adj_matrix_edge_property_selector type;
  };

  //=========================================================================
  // Functions required by PropertyGraph

  namespace detail {

    template <typename Property, typename D, typename VP, typename EP, 
              typename GP, typename A>
    typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
      Property>::type
    get_dispatch(adjacency_matrix<D,VP,EP,GP,A>& g, Property,
                 vertex_property_tag)
    {
      typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
      typedef typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
        Property>::type PA;
      return PA(&g);
    }
    template <typename Property, typename D, typename VP, typename EP, 
              typename GP, typename A>
    typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
      Property>::type
    get_dispatch(adjacency_matrix<D,VP,EP,GP,A>& g, Property,
                 edge_property_tag)
    {
      typedef typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
        Property>::type PA;
      return PA();
    }
    template <typename Property, typename D, typename VP, typename EP, 
              typename GP, typename A>
    typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
      Property>::const_type
    get_dispatch(const adjacency_matrix<D,VP,EP,GP,A>& g, Property,
                 vertex_property_tag)
    {
      typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
      typedef typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
        Property>::const_type PA;
      return PA(&g);
    }
    template <typename Property, typename D, typename VP, typename EP, 
              typename GP, typename A>
    typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
      Property>::const_type
    get_dispatch(const adjacency_matrix<D,VP,EP,GP,A>& g, Property,
                 edge_property_tag)
    {
      typedef typename boost_cryray::property_map<adjacency_matrix<D,VP,EP,GP,A>, 
        Property>::const_type PA;
      return PA();
    }

  } // namespace detail

  template <typename Property, typename D, typename VP, typename EP, 
            typename GP, typename A>
  inline
  typename property_map<adjacency_matrix<D,VP,EP,GP,A>, Property>::type
  get(Property p, adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    typedef typename property_kind<Property>::type Kind;
    return detail::get_dispatch(g, p, Kind());
  }

  template <typename Property, typename D, typename VP, typename EP, 
            typename GP, typename A>
  inline
  typename property_map<adjacency_matrix<D,VP,EP,GP,A>, Property>::const_type
  get(Property p, const adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    typedef typename property_kind<Property>::type Kind;
    return detail::get_dispatch(g, p, Kind());
  }

  template <typename Property, typename D, typename VP, typename EP, 
            typename GP, typename A, typename Key>
  inline
  typename property_traits<
    typename property_map<adjacency_matrix<D,VP,EP,GP,A>, Property>::const_type
  >::value_type
  get(Property p, const adjacency_matrix<D,VP,EP,GP,A>& g,
      const Key& key)
  {
    return get(get(p, g), key);
  }

  template <typename Property, typename D, typename VP, typename EP, 
            typename GP, typename A, typename Key, typename Value>
  inline void
  put(Property p, adjacency_matrix<D,VP,EP,GP,A>& g,
      const Key& key, const Value& value)
  {
    typedef adjacency_matrix<D,VP,EP,GP,A> Graph;
    typedef typename boost_cryray::property_map<Graph, Property>::type Map;
    Map pmap = get(p, g);
    put(pmap, key, value);
  }
  
  //=========================================================================
  // Other Functions

  template <typename D, typename VP, typename EP, typename GP, typename A>  
  typename adjacency_matrix<D,VP,EP,GP,A>::vertex_descriptor
  vertex(typename adjacency_matrix<D,VP,EP,GP,A>::vertices_size_type n,
         const adjacency_matrix<D,VP,EP,GP,A>& g)
  {
    return n;
  }


} // namespace boost_cryray

#endif // BOOST_ADJACENCY_MATRIX_HPP
