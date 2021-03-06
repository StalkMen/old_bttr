// preprocessed version of 'boost/mpl/apply.hpp' header
// see the original for copyright information

namespace boost_cryray {
namespace mpl {

namespace aux {
template< int arity_ > struct apply_impl_chooser;
}

template< typename F >
struct apply0 : F
{
    enum { arity = 1 }; typedef F arg1;
 friend class apply0_rebind;
 typedef apply0_rebind rebind;
 };
 class apply0_rebind { public: template< typename U1 > struct apply : apply0<U1> { };
 
};

// workaround for the ETI bug
template<>
struct apply0<int>
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<0>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply0<
              F  
            > type;
    };
};

} // namespace aux

template<
      typename F, typename T1
    >
struct apply1
    : F::template apply<
          T1
        >
{
    enum { arity = 2 }; typedef F arg1;
 typedef T1 arg2;
 friend class apply1_rebind;
 typedef apply1_rebind rebind;
 };
 class apply1_rebind { public: template< typename U1, typename U2 > struct apply : apply1< U1,U2 > { };
 
};

// workaround for ETI bug
template<>
struct apply1< int,int >
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<1>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply1<
              F, T1
            > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2
    >
struct apply2
    : F::template apply<
          T1, T2
        >
{
    enum { arity = 3 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 friend class apply2_rebind;
 typedef apply2_rebind rebind;
 };
 class apply2_rebind { public: template< typename U1, typename U2, typename U3 > struct apply : apply2< U1,U2,U3 > { };
 
};

// workaround for ETI bug
template<>
struct apply2< int,int,int >
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<2>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply2<
              F, T1, T2
            > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3
    >
struct apply3
    : F::template apply<
          T1, T2, T3
        >
{
    enum { arity = 4 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 friend class apply3_rebind;
 typedef apply3_rebind rebind;
 };
 class apply3_rebind { public: template< typename U1, typename U2, typename U3, typename U4 > struct apply : apply3< U1,U2,U3,U4 > { };
 
};

// workaround for ETI bug
template<>
struct apply3< int,int,int,int >
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<3>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply3<
              F, T1, T2, T3
            > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
struct apply4
    : F::template apply<
          T1, T2, T3, T4
        >
{
    enum { arity = 5 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 friend class apply4_rebind;
 typedef apply4_rebind rebind;
 };
 class apply4_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct apply : apply4< U1,U2,U3,U4,U5 > { };
 
};

// workaround for ETI bug
template<>
struct apply4< int,int,int,int,int >
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<4>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply4<
              F, T1, T2, T3, T4
            > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct apply5
    : F::template apply<
          T1, T2, T3, T4, T5
        >
{
    enum { arity = 6 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 typedef T5 arg6;
 friend class apply5_rebind;
 typedef apply5_rebind rebind;
 };
 class apply5_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6 > struct apply : apply5< U1,U2,U3,U4,U5,U6 > { };
 
};

// workaround for ETI bug
template<>
struct apply5< int,int,int,int,int,int >
{
    typedef int type;
};

namespace aux {

template<>
struct apply_impl_chooser<5>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef apply5<
              F, T1, T2, T3, T4, T5
            > type;
    };
};

} // namespace aux

namespace aux {

template< typename T >
struct is_apply_arg
{
    enum { value = true };
};

template<>
struct is_apply_arg<void_>
{
    enum { value = false };
};

template<
      typename T1, typename T2, typename T3, typename T4, typename T5
    >
struct apply_count_args
{
    enum { value =
          is_apply_arg<T1>::value + is_apply_arg<T2>::value 
        + is_apply_arg<T3>::value + is_apply_arg<T4>::value 
        + is_apply_arg<T5>::value
        };
};

} // namespace aux

template<
      typename F, typename T1 = void_, typename T2 = void_
    , typename T3 = void_, typename T4 = void_, typename T5 = void_
    >
struct apply
    : aux::apply_impl_chooser< 
          aux::apply_count_args< T1,T2,T3,T4,T5 >::value
        >::template result_< F,T1,T2,T3,T4,T5 >::type
{
};

} // namespace mpl
} // namespace boost_cryray

