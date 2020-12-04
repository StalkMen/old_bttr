// preprocessed version of 'boost/mpl/aux_/template_arity.hpp' header
// see the original for copyright information

namespace boost_cryray { namespace mpl { namespace aux {

template< bool >
struct template_arity_impl
{
    template< typename F > struct result_
    {
        static int const value = -1;
    };
};

template<>
struct template_arity_impl<true>
{
    template< typename F > struct result_
    {
        enum { value = F::arity };

    };
};

template< typename F >
struct template_arity
    : template_arity_impl< ::boost_cryray::mpl::aux::has_rebind<F>::value >
        ::template result_<F>
{
};

}}} // namespace boost_cryray::mpl::aux

