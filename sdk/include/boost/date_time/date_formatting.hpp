#ifndef DATE_TIME_DATE_FORMATTING_HPP___
#define DATE_TIME_DATE_FORMATTING_HPP___
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

#include "boost/date_time/iso_format.hpp"
#include <string>
#include <sstream>
#include <iomanip>


namespace boost_cryray {
namespace date_time {

  //! Formats a month as as string into an ostream
  template<class month_type, class format_type>
  class month_formatter
  {
  public:
    //! Formats a month as as string into an ostream
    /*! This function demands that month_type provide
     *  functions for converting to short and long strings
     *  if that capability is used.
     */
    static std::ostream& format_month(const month_type& month,
                                      std::ostream& os)
    {
      switch (format_type::month_format()) 
      {
        case month_as_short_string: 
        { 
          os << month.as_short_string(); 
          break;
        }
        case month_as_long_string: 
        { 
          os << month.as_long_string(); 
          break;
        }
        case month_as_integer: 
        { 
          os << std::setw(2) << std::setfill('0') << month.as_number();
          break;
        }
     
      }
      return os;
    } // format_month
  };


  //! Convert ymd to a standard string formatting policies
  template<class ymd_type, class format_type>
  class ymd_formatter
  {
  public:
    //! Convert ymd to a standard string formatting policies
    /*! This is standard code for handling date formatting with
     *  year-month-day based date information.  This function 
     *  uses the format_type to control whether the string will
     *  contain separator characters, and if so what the character
     *  will be.  In addtion, it can format the month as either
     *  an integer or a string as controled by the formatting 
     *  policy
     */ 
    static std::string ymd_to_string(ymd_type ymd)
    {
      typedef typename ymd_type::month_type month_type;
      std::ostringstream ss;
      ss << ymd.year;
      if (format_type::has_date_sep_chars()) {
        ss << format_type::month_sep_char();
      }
      //this name is a bit ugly, oh well....
      month_formatter<month_type,format_type>::format_month(ymd.month, ss);
      if (format_type::has_date_sep_chars()) {
        ss << format_type::day_sep_char();
      }
      ss  << std::setw(2) << std::setfill('0') 
          << ymd.day;
      return ss.str();
    }
  };


  //! Convert a date to string using format policies
  template<class date_type, class format_type>
  class date_formatter
  {
  public:
    //! Convert to a date to standard string using format policies
    static std::string date_to_string(date_type d)
    {
      typedef typename date_type::ymd_type ymd_type;
      if (d.is_not_a_date()) {
        return format_type::not_a_date();
      }
      if (d.is_neg_infinity()) {
        return format_type::neg_infinity();
      }
      if (d.is_pos_infinity()) {
        return format_type::pos_infinity();
      }
      ymd_type ymd = d.year_month_day();
      return ymd_formatter<ymd_type, format_type>::ymd_to_string(ymd);
    }    
  };
  
} } //namespace date_time



/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */
#endif

