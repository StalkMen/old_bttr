/*=============================================================================
    Spirit v1.6.0
    Copyright (c) 2002-2003 Joel de Guzman
    Copyright (c) 2002-2003 Hartmut Kaiser
    Copyright (c) 2003 Martin Wille
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_PARSER_TRAITS_HPP)
#define BOOST_SPIRIT_PARSER_TRAITS_HPP

#include "boost/type_traits.hpp"
#include "boost/static_assert.hpp"

#include "boost/spirit/core/parser.hpp"
#include "boost/spirit/core/meta/impl/parser_traits.ipp"

///////////////////////////////////////////////////////////////////////////////
namespace boost_cryray { namespace spirit {

///////////////////////////////////////////////////////////////////////////////
//
//  Parser traits templates
//
//      Used to determine the type and several other characteristics of a given
//      parser type.
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// The is_parser traits template can be used to tell wether a given
// class is a parser.
//
///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct is_parser
{
    BOOST_STATIC_CONSTANT(bool, value =
        (::boost_cryray::is_base_and_derived<parser<T>, T>::value));

//  [JDG 2/3/03] simplified implementation by
//  using boost_cryray::is_base_and_derived
};

///////////////////////////////////////////////////////////////////////////////
//
//  The is_unary_composite traits template can be used to tell if a given
//  parser is a unary parser as for instance kleene_star or optional.
//
///////////////////////////////////////////////////////////////////////////////
template <typename UnaryT>
struct is_unary_composite {

    BOOST_STATIC_CONSTANT(bool, value = (::boost_cryray::is_convertible<
        typename UnaryT::parser_category_t, unary_parser_category>::value));
};

///////////////////////////////////////////////////////////////////////////////
//
//  The is_acction_parser traits template can be used to tell if a given
//  parser is a action parser, i.e. it is a composite consisting of a
//  auxilliary parser and an attached semantic action.
//
///////////////////////////////////////////////////////////////////////////////
template <typename ActionT>
struct is_action_parser {

    BOOST_STATIC_CONSTANT(bool, value = (::boost_cryray::is_convertible<
        typename ActionT::parser_category_t, action_parser_category>::value));
};

///////////////////////////////////////////////////////////////////////////////
//
//  The is_binary_composite traits template can be used to tell if a given
//  parser is a binary parser as for instance sequence or difference.
//
///////////////////////////////////////////////////////////////////////////////
template <typename BinaryT>
struct is_binary_composite {

    BOOST_STATIC_CONSTANT(bool, value = (::boost_cryray::is_convertible<
        typename BinaryT::parser_category_t, binary_parser_category>::value));
};

///////////////////////////////////////////////////////////////////////////////
//
//  The is_composite_parser traits template can be used to tell if a given
//  parser is a unary or a binary parser composite type.
//
///////////////////////////////////////////////////////////////////////////////
template <typename CompositeT>
struct is_composite_parser {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::is_unary_composite<CompositeT>::value ||
        ::boost_cryray::spirit::is_binary_composite<CompositeT>::value));
};

///////////////////////////////////////////////////////////////////////////////
template <typename ParserT>
struct is_alternative {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_alternative));
};

template <typename ParserT>
struct is_sequence {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_sequence));
};

template <typename ParserT>
struct is_sequential_or {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_sequential_or));
};

template <typename ParserT>
struct is_intersection {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_intersection));
};

template <typename ParserT>
struct is_difference {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_difference));
};

template <typename ParserT>
struct is_exclusive_or {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_exclusive_or));
};

template <typename ParserT>
struct is_optional {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_optional));
};

template <typename ParserT>
struct is_kleene_star {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_kleene_star));
};

template <typename ParserT>
struct is_positive {

    BOOST_STATIC_CONSTANT(bool, value = (
        ::boost_cryray::spirit::impl::parser_type_traits<ParserT>::is_positive));
};

///////////////////////////////////////////////////////////////////////////////
//
//  Parser extraction templates
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  The unary_subject template can be used to return the type of the
//  parser used as the subject of an unary parser.
//  If the parser under inspection is not an unary type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename UnaryT>
struct unary_subject {

    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_unary_composite<UnaryT>::value);
    typedef typename UnaryT::subject_t type;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The get_unary_subject template function returns the parser object, which
//  is used as the subject of an unary parser.
//  If the parser under inspection is not an unary type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename UnaryT>
inline typename unary_subject<UnaryT>::type const &
get_unary_subject(UnaryT const &unary_)
{
    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_unary_composite<UnaryT>::value);
    return unary_.subject();
}

///////////////////////////////////////////////////////////////////////////////
//
//  The binary_left_subject and binary_right_subject templates can be used to
//  return the types of the parsers used as the left and right subject of an
//  binary parser.
//  If the parser under inspection is not a binary type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename BinaryT>
struct binary_left_subject {

    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_binary_composite<BinaryT>::value);
    typedef typename BinaryT::left_t type;
};

template <typename BinaryT>
struct binary_right_subject {

    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_binary_composite<BinaryT>::value);
    typedef typename BinaryT::right_t type;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The get_binary_left_subject and get_binary_right_subject template functions
//  return the parser object, which is used as the left or right subject of a
//  binary parser.
//  If the parser under inspection is not a binary type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename BinaryT>
inline typename binary_left_subject<BinaryT>::type const &
get_binary_left_subject(BinaryT const &binary_)
{
    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_binary_composite<BinaryT>::value);
    return binary_.left();
}

template <typename BinaryT>
inline typename binary_right_subject<BinaryT>::type const &
get_binary_right_subject(BinaryT const &binary_)
{
    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_binary_composite<BinaryT>::value);
    return binary_.right();
}

///////////////////////////////////////////////////////////////////////////////
//
//  The action_subject template can be used to return the type of the
//  parser used as the subject of an action parser.
//  If the parser under inspection is not an action type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename ActionT>
struct action_subject {

    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_action_parser<ActionT>::value);
    typedef typename ActionT::subject_t type;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The get_action_subject template function returns the parser object, which
//  is used as the subject of an action parser.
//  If the parser under inspection is not an action type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename ActionT>
inline typename action_subject<ActionT>::type const &
get_action_subject(ActionT const &action_)
{
    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_action_parser<ActionT>::value);
    return action_.subject();
}

///////////////////////////////////////////////////////////////////////////////
//
//  The semantic_action template can be used to return the type of the
//  attached semantic action of an action parser.
//  If the parser under inspection is not an action type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename ActionT>
struct semantic_action {

    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_action_parser<ActionT>::value);
    typedef typename ActionT::predicate_t type;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The get_semantic_action template function returns the attached semantic
//  action of an action parser.
//  If the parser under inspection is not an action type parser the compilation
//  will fail.
//
///////////////////////////////////////////////////////////////////////////////
template <typename ActionT>
inline typename semantic_action<ActionT>::type const &
get_semantic_action(ActionT const &action_)
{
    BOOST_STATIC_ASSERT(::boost_cryray::spirit::is_action_parser<ActionT>::value);
    return action_.predicate();
}

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost_cryray::spirit

#endif // !defined(BOOST_SPIRIT_PARSER_TRAITS_HPP)
