#ifndef BOOST_CHECKED_DELETE_HPP_INCLUDED
#define BOOST_CHECKED_DELETE_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/checked_delete.hpp
//
//  Copyright (c) 1999, 2000, 2001, 2002 boost.org
//  Copyright (c) 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/utility/checked_delete.html for documentation.
//

namespace boost_cryray
{

// verify that types are complete for increased safety

template<class T> inline void checked_delete(T * x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}

template<class T> inline void checked_array_delete(T * x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete [] x;
}

template<class T> struct checked_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        boost_cryray::checked_delete(x);
    }
};

template<class T> struct checked_array_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        boost_cryray::checked_array_delete(x);
    }
};

} // namespace boost_cryray

#endif  // #ifndef BOOST_CHECKED_DELETE_HPP_INCLUDED
