#ifndef BOOST_BIND_MAKE_ADAPTABLE_HPP_INCLUDED
#define BOOST_BIND_MAKE_ADAPTABLE_HPP_INCLUDED

//
//  make_adaptable.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

namespace boost_cryray
{

namespace _bi
{

template<class F> void instantiate(F)
{
}

template<class R, class F> class af0
{
public:

    typedef R result_type;

    explicit af0(F f): f_(f)
    {
    }

    result_type operator()()
    {
        return f_();
    }

private:

    F f_;
};

template<class R, class A1, class F> class af1
{
public:

    typedef R result_type;
    typedef A1 argument_type;
    typedef A1 arg1_type;

    explicit af1(F f): f_(f)
    {
    }

    result_type operator()(A1 a1)
    {
        return f_(a1);
    }

private:

    F f_;
};

template<class R, class A1, class A2, class F> class af2
{
public:

    typedef R result_type;
    typedef A1 first_argument_type;
    typedef A2 second_argument_type;
    typedef A1 arg1_type;
    typedef A2 arg2_type;

    explicit af2(F f): f_(f)
    {
    }

    result_type operator()(A1 a1, A2 a2)
    {
        return f_(a1, a2);
    }

private:

    F f_;
};

template<class R, class A1, class A2, class A3, class F> class af3
{
public:

    typedef R result_type;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
    typedef A3 arg3_type;

    explicit af3(F f): f_(f)
    {
    }

    result_type operator()(A1 a1, A2 a2, A3 a3)
    {
        return f_(a1, a2, a3);
    }

private:

    F f_;
};

template<class R, class A1, class A2, class A3, class A4, class F> class af4
{
public:

    typedef R result_type;
    typedef A1 arg1_type;
    typedef A2 arg2_type;
    typedef A3 arg3_type;
    typedef A4 arg4_type;

    explicit af4(F f): f_(f)
    {
    }

    result_type operator()(A1 a1, A2 a2, A3 a3, A4 a4)
    {
        return f_(a1, a2, a3, a4);
    }

private:

    F f_;
};

} // namespace _bi

template<class R, class F> _bi::af0<R, F> make_adaptable(F f)
{
    _bi::instantiate( &_bi::af0<R, F>::operator() ); // for early error detection
    return _bi::af0<R, F>(f);
}

template<class R, class A1, class F> _bi::af1<R, A1, F> make_adaptable(F f)
{
    instantiate( &_bi::af1<R, A1, F>::operator() );
    return _bi::af1<R, A1, F>(f);
}

template<class R, class A1, class A2, class F> _bi::af2<R, A1, A2, F> make_adaptable(F f)
{
    instantiate( &_bi::af2<R, A1, A2, F>::operator() );
    return _bi::af2<R, A1, A2, F>(f);
}

template<class R, class A1, class A2, class A3, class F> _bi::af3<R, A1, A2, A3, F> make_adaptable(F f)
{
    instantiate( &_bi::af3<R, A1, A2, A3, F>::operator() );
    return _bi::af3<R, A1, A2, A3, F>(f);
}

template<class R, class A1, class A2, class A3, class A4, class F> _bi::af4<R, A1, A2, A3, A4, F> make_adaptable(F f)
{
    instantiate( &_bi::af4<R, A1, A2, A3, A4, F>::operator() );
    return _bi::af4<R, A1, A2, A3, A4, F>(f);
}

} // namespace boost_cryray

#endif // #ifndef BOOST_BIND_MAKE_ADAPTABLE_HPP_INCLUDED
