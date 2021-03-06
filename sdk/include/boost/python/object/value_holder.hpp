#if !defined(BOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

# ifndef VALUE_HOLDER_DWA20011215_HPP
#  define VALUE_HOLDER_DWA20011215_HPP 

#  include <boost/python/object/value_holder_fwd.hpp>

#  include <boost/python/instance_holder.hpp>
#  include <boost/python/type_id.hpp>

#  include <boost/python/object/inheritance.hpp>
#  include <boost/python/object/forward.hpp>

#  include <boost/python/detail/force_instantiate.hpp>
#  include <boost/python/detail/preprocessor.hpp>

#  include <boost/preprocessor/comma_if.hpp>
#  include <boost/preprocessor/enum_params.hpp>
#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/repeat.hpp>
#  include <boost/preprocessor/debug/line.hpp>

#  include <boost/preprocessor/repetition/enum_params.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace boost_cryray { namespace python { namespace objects { 

#  define BOOST_PYTHON_UNFORWARD_LOCAL(z, n, _) BOOST_PP_COMMA_IF(n) (typename unforward<A##n>::type)(a##n)

template <class Held>
struct value_holder : instance_holder
{
    typedef Held value_type;

    // Forward construction to the held object
#  define BOOST_PP_ITERATION_PARAMS_1 (4, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/object/value_holder.hpp>, 1))
#  include BOOST_PP_ITERATE()

 private: // required holder implementation
    void* holds(type_info);

 private: // data members
    Held m_held;
};

template <class Held, class BackReferenceType>
struct value_holder_back_reference : instance_holder
{
    typedef Held value_type;
    
    // Forward construction to the held object
#  define BOOST_PP_ITERATION_PARAMS_1 (4, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/object/value_holder.hpp>, 2))
#  include BOOST_PP_ITERATE()

private: // required holder implementation
    void* holds(type_info);

 private: // data members
    BackReferenceType m_held;
};

#  undef BOOST_PYTHON_UNFORWARD_LOCAL

template <class Held>
void* value_holder<Held>::holds(type_info dst_t)
{
    type_info src_t = python::type_id<Held>();
    return src_t == dst_t ? &m_held
        : find_static_type(&m_held, src_t, dst_t);
}

template <class Held, class BackReferenceType>
void* value_holder_back_reference<Held,BackReferenceType>::holds(
    type_info dst_t)
{
    type_info src_t = python::type_id<Held>();
    Held* x = &m_held;
    
    if (dst_t == src_t)
        return x;
    else if (dst_t == python::type_id<BackReferenceType>())
        return &m_held;
    else
        return find_static_type(x, src_t, dst_t);
}

}}} // namespace boost_cryray::python::objects

# endif // VALUE_HOLDER_DWA20011215_HPP

// --------------- value_holder ---------------

#elif BOOST_PP_ITERATION_DEPTH() == 1 && BOOST_PP_ITERATION_FLAGS() == 1
# line BOOST_PP_LINE(__LINE__, value_holder.hpp(value_holder))

# define N BOOST_PP_ITERATION()

# if (N != 0)
    template <BOOST_PP_ENUM_PARAMS_Z(1, N, class A)>
# endif
    value_holder(
      PyObject* BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_held(
            BOOST_PP_REPEAT_1ST(N, BOOST_PYTHON_UNFORWARD_LOCAL, nil)
            )
    {}

# undef N

// --------------- value_holder_back_reference ---------------

#elif BOOST_PP_ITERATION_DEPTH() == 1 && BOOST_PP_ITERATION_FLAGS() == 2
# line BOOST_PP_LINE(__LINE__, value_holder.hpp(value_holder_back_reference))

# define N BOOST_PP_ITERATION()

# if (N != 0)
    template <BOOST_PP_ENUM_PARAMS_Z(1, N, class A)>
# endif
    value_holder_back_reference(
        PyObject* p BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_held(
            p BOOST_PP_COMMA_IF(N)
            BOOST_PP_REPEAT_1ST(N, BOOST_PYTHON_UNFORWARD_LOCAL, nil)
            )
    {
    }

# undef N

#endif
