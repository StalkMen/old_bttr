// preprocessed version of 'boost/mpl/aux_/fold_backward_impl.hpp' header
// see the original for copyright information

namespace boost_cryray {
namespace mpl {
namespace aux {

// forward declaration
template<
      nttp_long N
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct fold_backward_impl;

template< nttp_long N >
struct fold_backward_chunk;

template<>
struct fold_backward_chunk<0>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef fwd_state0 bkwd_state0;
        typedef bkwd_state0 state;
        typedef iter0 iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template<>
struct fold_backward_chunk<1>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef typename apply2<ForwardOp, fwd_state0, typename iter0::type>::type fwd_state1;
        typedef typename iter0::next iter1;
        

        typedef fwd_state1 bkwd_state1;
        typedef typename apply2<BackwardOp, bkwd_state1, typename iter0::type>::type bkwd_state0;
        typedef bkwd_state0 state;
        typedef iter1 iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template<>
struct fold_backward_chunk<2>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef typename apply2<ForwardOp, fwd_state0, typename iter0::type>::type fwd_state1;
        typedef typename iter0::next iter1;
        typedef typename apply2<ForwardOp, fwd_state1, typename iter1::type>::type fwd_state2;
        typedef typename iter1::next iter2;
        

        typedef fwd_state2 bkwd_state2;
        typedef typename apply2<BackwardOp, bkwd_state2, typename iter1::type>::type bkwd_state1;
        typedef typename apply2<BackwardOp, bkwd_state1, typename iter0::type>::type bkwd_state0;
        

        typedef bkwd_state0 state;
        typedef iter2 iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template<>
struct fold_backward_chunk<3>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef typename apply2<ForwardOp, fwd_state0, typename iter0::type>::type fwd_state1;
        typedef typename iter0::next iter1;
        typedef typename apply2<ForwardOp, fwd_state1, typename iter1::type>::type fwd_state2;
        typedef typename iter1::next iter2;
        typedef typename apply2<ForwardOp, fwd_state2, typename iter2::type>::type fwd_state3;
        typedef typename iter2::next iter3;
        

        typedef fwd_state3 bkwd_state3;
        typedef typename apply2<BackwardOp, bkwd_state3, typename iter2::type>::type bkwd_state2;
        typedef typename apply2<BackwardOp, bkwd_state2, typename iter1::type>::type bkwd_state1;
        typedef typename apply2<BackwardOp, bkwd_state1, typename iter0::type>::type bkwd_state0;
        

        typedef bkwd_state0 state;
        typedef iter3 iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template<>
struct fold_backward_chunk<4>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef typename apply2<ForwardOp, fwd_state0, typename iter0::type>::type fwd_state1;
        typedef typename iter0::next iter1;
        typedef typename apply2<ForwardOp, fwd_state1, typename iter1::type>::type fwd_state2;
        typedef typename iter1::next iter2;
        typedef typename apply2<ForwardOp, fwd_state2, typename iter2::type>::type fwd_state3;
        typedef typename iter2::next iter3;
        typedef typename apply2<ForwardOp, fwd_state3, typename iter3::type>::type fwd_state4;
        typedef typename iter3::next iter4;
        

        typedef fwd_state4 bkwd_state4;
        typedef typename apply2<BackwardOp, bkwd_state4, typename iter3::type>::type bkwd_state3;
        typedef typename apply2<BackwardOp, bkwd_state3, typename iter2::type>::type bkwd_state2;
        typedef typename apply2<BackwardOp, bkwd_state2, typename iter1::type>::type bkwd_state1;
        typedef typename apply2<BackwardOp, bkwd_state1, typename iter0::type>::type bkwd_state0;
        

        typedef bkwd_state0 state;
        typedef iter4 iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template< nttp_long N > 
struct fold_backward_chunk
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        > 
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;
        typedef typename apply2<ForwardOp, fwd_state0, typename iter0::type>::type fwd_state1;
        typedef typename iter0::next iter1;
        typedef typename apply2<ForwardOp, fwd_state1, typename iter1::type>::type fwd_state2;
        typedef typename iter1::next iter2;
        typedef typename apply2<ForwardOp, fwd_state2, typename iter2::type>::type fwd_state3;
        typedef typename iter2::next iter3;
        typedef typename apply2<ForwardOp, fwd_state3, typename iter3::type>::type fwd_state4;
        typedef typename iter3::next iter4;
        

        typedef fold_backward_impl<
              ( (N - 4) < 0 ? 0 : N - 4 )
            , iter4
            , Last
            , fwd_state4
            , BackwardOp
            , ForwardOp
            > nested_chunk;
            
        typedef typename nested_chunk::state bkwd_state4;
        typedef typename apply2<BackwardOp, bkwd_state4, typename iter3::type>::type bkwd_state3;
        typedef typename apply2<BackwardOp, bkwd_state3, typename iter2::type>::type bkwd_state2;
        typedef typename apply2<BackwardOp, bkwd_state2, typename iter1::type>::type bkwd_state1;
        typedef typename apply2<BackwardOp, bkwd_state1, typename iter0::type>::type bkwd_state0;
        

        typedef bkwd_state0 state;
        typedef typename nested_chunk::iterator iterator;
    };
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct fold_backward_step;

template<
      typename Last
    , typename State
    >
struct fold_backward_null_step
{
    typedef Last iterator;
    typedef State state;
};

template<> 
struct fold_backward_chunk< -1 >
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        > 
    struct result_
    {
        typedef typename if_<
              typename is_same< First,Last >::type
            , fold_backward_null_step< Last,State >
            , fold_backward_step< First,Last,State,BackwardOp,ForwardOp >
            >::type res_;

        typedef typename res_::state state;
        typedef typename res_::iterator iterator;
    };

    // ETI workaround
    template<> struct result_<int, int, int, int, int>
    {
        typedef int state;
        typedef int iterator;
    };

};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct fold_backward_step
{
    typedef fold_backward_chunk< -1 >::template result_<
          typename First::next
        , Last
        , typename apply2<ForwardOp, State, typename First::type>::type
        , BackwardOp
        , ForwardOp
        > nested_step;

    typedef typename apply2<BackwardOp, typename nested_step::state, typename First::type>::type state;
    typedef typename nested_step::iterator iterator;
};

template<
      nttp_long N
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct fold_backward_impl
    : fold_backward_chunk<N>
        ::template result_< First,Last,State,BackwardOp,ForwardOp >
{
};

} // namespace aux
} // namespace mpl
} // namespace boost_cryray

