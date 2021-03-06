#ifndef GREG_WEEKDAY_HPP___
#define GREG_WEEKDAY_HPP___
/* Copyright (c) 2000 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland
 */

#include "boost/date_time/constrained_value.hpp"
#include "boost/date_time/date_defs.hpp"
#include <stdexcept>
#include <string>

namespace boost_cryray {
namespace gregorian {

  //bring enum values into the namespace
  using date_time::Sunday;
  using date_time::Monday;
  using date_time::Tuesday;
  using date_time::Wednesday;
  using date_time::Thursday;
  using date_time::Friday;
  using date_time::Saturday;


  //! Exception that flags that a weekday number is incorrect
  struct bad_weekday : public std::out_of_range
  {
    bad_weekday() : std::out_of_range(std::string("Weekday os out of range 0..6")) {}
  };
  typedef CV::simple_exception_policy<unsigned short, 0, 6, bad_weekday> greg_weekday_policies;
  typedef CV::constrained_value<greg_weekday_policies> greg_weekday_rep;


  //! Represent a day within a week (range 0==Sun to 6==Sat)
  class greg_weekday : public greg_weekday_rep {
  public:
    typedef boost_cryray::date_time::weekdays weekday_enum;
    greg_weekday(unsigned short day_of_week_num) :
      greg_weekday_rep(day_of_week_num)
    {}

    unsigned short as_number() const {return value_;}
    const char* as_short_string() const;
    const char* as_long_string()  const;
    weekday_enum as_enum() const {return static_cast<weekday_enum>(value_);}


  };



} } //namespace gregorian

/* Copyright (c) 2000
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
