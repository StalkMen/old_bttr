// preprocessed version of 'boost/mpl/quote.hpp' header
// see the original for copyright information

namespace boost_cryray {
namespace mpl {

template< bool > struct quote_impl
{
    template< typename T > struct result_
        : T
    {
    };
};

template<> struct quote_impl<false>
{
    template< typename T > struct result_
    {
        typedef T type;
    };
};

template<
      template< typename P1 > class F
    >
struct quote1
{
    template< typename U1 > struct apply

        : quote_impl< aux::has_type< F<U1> >::value >
            ::template result_< F<U1> >

    {
    };
};

template<
      template< typename P1, typename P2 > class F
    >
struct quote2
{
    template< typename U1, typename U2 > struct apply

        : quote_impl< aux::has_type< F<U1,U2> >::value >
            ::template result_< F<U1,U2> >

    {
    };
};

template<
      template< typename P1, typename P2, typename P3 > class F
    >
struct quote3
{
    template< typename U1, typename U2, typename U3 > struct apply

        : quote_impl< aux::has_type< F<U1,U2,U3> >::value >
            ::template result_< F<U1,U2,U3> >

    {
    };
};

template<
      template< typename P1, typename P2, typename P3, typename P4 > class F
    >
struct quote4
{
    template<
          typename U1, typename U2, typename U3, typename U4
        >
    struct apply

        : quote_impl< aux::has_type< F<U1,U2,U3,U4> >::value >
            ::template result_< F<U1,U2,U3,U4> >

    {
    };
};

template<
      template<
          typename P1, typename P2, typename P3, typename P4
        , typename P5
        >
      class F
    >
struct quote5
{
    template<
          typename U1, typename U2, typename U3, typename U4
        , typename U5
        >
    struct apply

        : quote_impl< aux::has_type< F<U1,U2,U3,U4,U5> >::value >
            ::template result_< F<U1,U2,U3,U4,U5> >

    {
    };
};

} // namespace mpl
} // namespace boost_cryray

