/*=============================================================================
    Spirit v1.6.0
    Copyright (c) 2001-2003 Daniel Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_FLUSH_MULTI_PASS_HPP
#define BOOST_SPIRIT_FLUSH_MULTI_PASS_HPP

///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/core.hpp"
#include "boost/spirit/iterator/multi_pass.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace boost_cryray { namespace spirit {

    namespace impl {

        template <typename T>
        void flush_iterator(T &) {}

        template <typename T1, typename T2, typename T3, typename T4>
        void flush_iterator(boost_cryray::spirit::multi_pass<
            T1, T2, T3, T4, boost_cryray::spirit::multi_pass_policies::std_deque> &i)
        {
            i.clear_queue();
        }

    }   // namespace impl

    ///////////////////////////////////////////////////////////////////////////
    //
    //  flush_multi_pass_parser
    //
    //      The flush_multi_pass_parser flushes an underlying
    //      multi_pass_iterator during the normal parsing process. This may
    //      be used at certain points during the parsing process, when it is
    //      clear, that no backtracking is needed anymore and the input
    //      gathered so far may be discarded.
    //
    ///////////////////////////////////////////////////////////////////////////
    class flush_multi_pass_parser
    :   public parser<flush_multi_pass_parser>
    {
    public:
        typedef flush_multi_pass_parser this_t;

        template <typename ScannerT>
        typename parser_result<this_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            impl::flush_iterator(scan.first);
            return scan.empty_match();
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  predefined flush_multi_pass_p object
    //
    //      This object should may used to flush a multi_pass_iterator along
    //      the way during the normal parsing process.
    //
    ///////////////////////////////////////////////////////////////////////////

    flush_multi_pass_parser const
        flush_multi_pass_p = flush_multi_pass_parser();

}} // namespace boost_cryray::spirit

#endif // BOOST_SPIRIT_FLUSH_MULTI_PASS_HPP
