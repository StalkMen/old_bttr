// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef RETURN_VALUE_POLICY_DWA2002131_HPP
# define RETURN_VALUE_POLICY_DWA2002131_HPP
# include <boost/python/default_call_policies.hpp>

namespace boost_cryray { namespace python { 

template <class ResultConverterGenerator, class BasePolicy_ = default_call_policies>
struct return_value_policy : BasePolicy_
{
    typedef ResultConverterGenerator result_converter;
};

}} // namespace boost_cryray::python

#endif // RETURN_VALUE_POLICY_DWA2002131_HPP
