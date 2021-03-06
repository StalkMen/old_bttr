/*=============================================================================
    Spirit v1.6.0
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2001-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/

#if !defined(PARSE_TREE_UTILS_IPP)
#define PARSE_TREE_UTILS_IPP

///////////////////////////////////////////////////////////////////////////////
namespace boost_cryray {
namespace spirit {

///////////////////////////////////////////////////////////////////////////////
//
//  Returnes the first leaf node of the given parsetree.
//
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline tree_node<T> const &
get_first_leaf (tree_node<T> const &node)
{
    if (node.children.size() > 0)
        return get_first_leaf(*node.children.begin());
    return node;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Find a specified node through recursive search.
//
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline bool
find_node (tree_node<T> const &node, parser_id node_to_search,
    tree_node<T> const **found_node)
{
    if (node.value.id() == node_to_search) {
        *found_node = &node;
        return true;
    }
    if (node.children.size() > 0) {
        typedef typename tree_node<T>::const_tree_iterator const_tree_iterator;

        const_tree_iterator end = node.children.end();
        for (const_tree_iterator it = node.children.begin(); it != end; ++it)
        {
            if (find_node (*it, node_to_search, found_node))
                return true;
        }
    }
    return false;   // not found here
}

///////////////////////////////////////////////////////////////////////////////
//
//  The functions 'get_node_range' return a pair of iterators pointing at the
//  range, which containes the elements of a specified node.
//
///////////////////////////////////////////////////////////////////////////////
namespace impl {

template <typename T>
inline bool
get_node_range (typename tree_node<T>::const_tree_iterator const &start,
    parser_id node_to_search,
    std::pair<typename tree_node<T>::const_tree_iterator,
        typename tree_node<T>::const_tree_iterator> &nodes)
{
// look at this node first
tree_node<T> const &node = *start;

    if (node.value.id() == node_to_search) {
        if (node.children.size() > 0) {
        // full subrange
            nodes.first = node.children.begin();
            nodes.second = node.children.end();
        }
        else {
        // only this node
            nodes.first = start;
            nodes.second = start;
            std::advance(nodes.second, 1);
        }
        return true;
    }

// look at subnodes now
    if (node.children.size() > 0) {
        typedef typename tree_node<T>::const_tree_iterator const_tree_iterator;

        const_tree_iterator end = node.children.end();
        for (const_tree_iterator it = node.children.begin(); it != end; ++it)
        {
            if (get_node_range<T>(it, node_to_search, nodes))
                return true;
        }
    }
    return false;
}

} // end of namespace impl

template <typename T>
inline bool
get_node_range (tree_node<T> const &node, parser_id node_to_search,
    std::pair<typename tree_node<T>::const_tree_iterator,
        typename tree_node<T>::const_tree_iterator> &nodes)
{
    if (node.children.size() > 0) {
        typedef typename tree_node<T>::const_tree_iterator const_tree_iterator;

        const_tree_iterator end = node.children.end();
        for (const_tree_iterator it = node.children.begin(); it != end; ++it)
        {
            if (impl::get_node_range<T>(it, node_to_search, nodes))
                return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace spirit
}   // namespace boost_cryray

#endif // !defined(PARSE_TREE_UTILS_IPP)
