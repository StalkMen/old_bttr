//  Boost io/ios_state.hpp header file  --------------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_IO_IOS_STATE_HPP
#define BOOST_IO_IOS_STATE_HPP

#include <boost/io_fwd.hpp>  // self include

#include <ios>        // for std::ios_base, std::basic_ios, etc.
#include <locale>     // for std::locale
#include <ostream>    // for std::basic_ostream
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost_cryray
{
namespace io
{


//  Basic stream state saver class declarations  -----------------------------//

class ios_flags_saver
{
public:
    typedef ::std::ios_base            state_type;
    typedef ::std::ios_base::fmtflags  aspect_type;

    explicit  ios_flags_saver( state_type &s )
        : s_save_( s ), a_save_( s.flags() )
        {}
    ios_flags_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.flags(a) )
        {}
    ~ios_flags_saver()
        { s_save_.flags( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

class ios_precision_saver
{
public:
    typedef ::std::ios_base    state_type;
    typedef ::std::streamsize  aspect_type;

    explicit  ios_precision_saver( state_type &s )
        : s_save_( s ), a_save_( s.precision() )
        {}
    ios_precision_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.precision(a) )
        {}
    ~ios_precision_saver()
        { s_save_.precision( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

class ios_width_saver
{
public:
    typedef ::std::ios_base    state_type;
    typedef ::std::streamsize  aspect_type;

    explicit  ios_width_saver( state_type &s )
        : s_save_( s ), a_save_( s.width() )
        {}
    ios_width_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.width(a) )
        {}
    ~ios_width_saver()
        { s_save_.width( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};


//  Advanced stream state saver class template declarations  -----------------//

template < typename Ch, class Tr >
class basic_ios_iostate_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>  state_type;
    typedef ::std::ios_base::iostate  aspect_type;

    explicit  basic_ios_iostate_saver( state_type &s )
        : s_save_( s ), a_save_( s.rdstate() )
        {}
    basic_ios_iostate_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.rdstate() )
        { s.clear(a); }
    ~basic_ios_iostate_saver()
        { s_save_.clear( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

template < typename Ch, class Tr >
class basic_ios_exception_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>  state_type;
    typedef ::std::ios_base::iostate  aspect_type;

    explicit  basic_ios_exception_saver( state_type &s )
        : s_save_( s ), a_save_( s.exceptions() )
        {}
    basic_ios_exception_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.exceptions() )
        { s.exceptions(a); }
    ~basic_ios_exception_saver()
        { s_save_.exceptions( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

template < typename Ch, class Tr >
class basic_ios_tie_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>        state_type;
    typedef ::std::basic_ostream<Ch, Tr> *  aspect_type;

    explicit  basic_ios_tie_saver( state_type &s )
        : s_save_( s ), a_save_( s.tie() )
        {}
    basic_ios_tie_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.tie(a) )
        {}
    ~basic_ios_tie_saver()
        { s_save_.tie( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

template < typename Ch, class Tr >
class basic_ios_rdbuf_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>          state_type;
    typedef ::std::basic_streambuf<Ch, Tr> *  aspect_type;

    explicit  basic_ios_rdbuf_saver( state_type &s )
        : s_save_( s ), a_save_( s.rdbuf() )
        {}
    basic_ios_rdbuf_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.rdbuf(a) )
        {}
    ~basic_ios_rdbuf_saver()
        { s_save_.rdbuf( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

template < typename Ch, class Tr >
class basic_ios_fill_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>        state_type;
    typedef typename state_type::char_type  aspect_type;

    explicit  basic_ios_fill_saver( state_type &s )
        : s_save_( s ), a_save_( s.fill() )
        {}
    basic_ios_fill_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.fill(a) )
        {}
    ~basic_ios_fill_saver()
        { s_save_.fill( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};

template < typename Ch, class Tr >
class basic_ios_locale_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr> state_type;
    typedef ::std::locale aspect_type;

    explicit basic_ios_locale_saver( state_type &s )
        : s_save_( s ), a_save_( s.getloc() )
        {}
    basic_ios_locale_saver( state_type &s, aspect_type const &a )
        : s_save_( s ), a_save_( s.imbue(a) )
        {}
    ~basic_ios_locale_saver()
        { s_save_.imbue( a_save_ ); }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
};


//  User-defined stream state saver class declarations  ----------------------//

class ios_iword_saver
{
public:
    typedef ::std::ios_base  state_type;
    typedef int              index_type;
    typedef long             aspect_type;

    explicit ios_iword_saver( state_type &s, index_type i )
        : s_save_( s ), a_save_( s.iword(i) ), i_save_( i )
        {}
    ios_iword_saver( state_type &s, index_type i, aspect_type const &a )
        : s_save_( s ), a_save_( s.iword(i) ), i_save_( i )
        { s.iword(i) = a; }
    ~ios_iword_saver()
        { s_save_.iword( i_save_ ) = a_save_; }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
    index_type const   i_save_;
};

class ios_pword_saver
{
public:
    typedef ::std::ios_base  state_type;
    typedef int              index_type;
    typedef void *           aspect_type;

    explicit  ios_pword_saver( state_type &s, index_type i )
        : s_save_( s ), a_save_( s.pword(i) ), i_save_( i )
        {}
    ios_pword_saver( state_type &s, index_type i, aspect_type const &a )
        : s_save_( s ), a_save_( s.pword(i) ), i_save_( i )
        { s.pword(i) = a; }
    ~ios_pword_saver()
        { s_save_.pword( i_save_ ) = a_save_; }

private:
    state_type &       s_save_;
    aspect_type const  a_save_;
    index_type const   i_save_;
};


//  Combined stream state saver class (template) declarations  ---------------//

class ios_base_all_saver
{
public:
    typedef ::std::ios_base  state_type;

    explicit  ios_base_all_saver( state_type &s )
        : s_save_( s ), a1_save_( s.flags() ), a2_save_( s.precision() )
        , a3_save_( s.width() )
        {}

    ~ios_base_all_saver()
    {
        s_save_.width( a3_save_ );
        s_save_.precision( a2_save_ );
        s_save_.flags( a1_save_ );
    }

private:
    state_type &                s_save_;
    state_type::fmtflags const  a1_save_;
    ::std::streamsize const     a2_save_;
    ::std::streamsize const     a3_save_;
};

template < typename Ch, class Tr >
class basic_ios_all_saver
{
public:
    typedef ::std::basic_ios<Ch, Tr>  state_type;

    explicit  basic_ios_all_saver( state_type &s )
        : s_save_( s ), a1_save_( s.flags() ), a2_save_( s.precision() )
        , a3_save_( s.width() ), a4_save_( s.rdstate() )
        , a5_save_( s.exceptions() ), a6_save_( s.tie() )
        , a7_save_( s.rdbuf() ), a8_save_( s.fill() ), a9_save_( s.getloc() )
        {}

    ~basic_ios_all_saver()
    {
        s_save_.imbue( a9_save_ );
        s_save_.fill( a8_save_ );
        s_save_.rdbuf( a7_save_ );
        s_save_.tie( a6_save_ );
        s_save_.exceptions( a5_save_ );
        s_save_.clear( a4_save_ );
        s_save_.width( a3_save_ );
        s_save_.precision( a2_save_ );
        s_save_.flags( a1_save_ );
    }

private:
    state_type &                            s_save_;
    typename state_type::fmtflags const     a1_save_;
    ::std::streamsize const                 a2_save_;
    ::std::streamsize const                 a3_save_;
    typename state_type::iostate const      a4_save_;
    typename state_type::iostate const      a5_save_;
    ::std::basic_ostream<Ch, Tr> * const    a6_save_;
    ::std::basic_streambuf<Ch, Tr> * const  a7_save_;
    typename state_type::char_type const    a8_save_;
    ::std::locale const                     a9_save_;
};

class ios_all_word_saver
{
public:
    typedef ::std::ios_base  state_type;
    typedef int              index_type;

    ios_all_word_saver( state_type &s, index_type i )
        : s_save_( s ), i_save_( i ), a1_save_( s.iword(i) )
        , a2_save_( s.pword(i) )
        {}

    ~ios_all_word_saver()
    {
        s_save_.pword( i_save_ ) = a2_save_;
        s_save_.iword( i_save_ ) = a1_save_;
    }

private:
    state_type &      s_save_;
    index_type const  i_save_;
    long const        a1_save_;
    void * const      a2_save_;
};


}  // namespace io
}  // namespace boost_cryray


#endif  // BOOST_IO_IOS_STATE_HPP
