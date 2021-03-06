#ifndef BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED
#define BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/lightweight_test.hpp - lightweight test library
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  BOOST_TEST(expression)
//  BOOST_ERROR(message)
//
//  int boost_cryray::report_errors()
//

#include <boost/current_function.hpp>
#include <iostream>

namespace boost_cryray
{

namespace detail
{

inline int & test_errors()
{
    static int x = 0;
    return x;
}

inline void test_failed_impl(char const * expr, char const * file, int line, char const * function)
{
    std::cerr << file << "(" << line << "): test '" << expr << "' failed in function '" << function << "'" << std::endl;
    ++test_errors();
}

inline void error_impl(char const * msg, char const * file, int line, char const * function)
{
    std::cerr << file << "(" << line << "): " << msg << " in function '" << function << "'" << std::endl;
    ++test_errors();
}

} // namespace detail

inline int report_errors()
{
    int errors = detail::test_errors();

    if(errors == 0)
    {
        std::cerr << "No errors detected." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << errors << " error" << (errors == 1? "": "s") << " detected." << std::endl;
        return 1;
    }
}

} // namespace boost_cryray

#define BOOST_TEST(expr) ((expr)? (void)0: ::boost_cryray::detail::test_failed_impl(#expr, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION))
#define BOOST_ERROR(msg) ::boost_cryray::detail::error_impl(msg, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION)

#endif // #ifndef BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED
