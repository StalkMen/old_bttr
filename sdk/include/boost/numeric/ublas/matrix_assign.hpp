//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef BOOST_UBLAS_MATRIX_ASSIGN_H
#define BOOST_UBLAS_MATRIX_ASSIGN_H

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>

// Iterators based on ideas of Jeremy Siek

namespace boost_cryray { namespace numeric { namespace ublas {

    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    bool equals (const matrix_expression<E1> &e1, const matrix_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME type_traits<BOOST_UBLAS_TYPENAME promote_traits<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                     BOOST_UBLAS_TYPENAME E2::value_type>::promote_type>::real_type real_type;
        // Check, that the values match at least half.
        static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
#ifndef __GNUC__
        return norm_inf (e1 - e2) < sqrt_epsilon *
               std::max<real_type> (std::max<real_type> (norm_inf (e1),
                                                         norm_inf (e2)),
                                    std::numeric_limits<real_type>::min ());
#else
        // GCC 3.1, oops?!
        return norm_inf (e1 - e2) < sqrt_epsilon *
               std::max (real_type (std::max (real_type (norm_inf (e1)), real_type (norm_inf (e2)))),
                         real_type (std::numeric_limits<real_type>::min ()));
#endif
    }

    // Restart for sparse (proxy) assignments
    template<class E>
    BOOST_UBLAS_INLINE
    void restart (const matrix_expression<E> &e, typename E::size_type index1, typename E::size_type index2,
                  typename E::const_iterator1 &it1e, typename E::const_iterator1 &it1e_end,
                  typename E::const_iterator2 &it2e, typename E::const_iterator2 &it2e_end, row_major_tag) {
        it1e = e ().find_first1 (0, index1, 0);
        it1e_end = e ().find_first1 (0, e ().size1 (), 0);
        it2e = e ().find_first2 (1, index1, index2);
        it2e_end = e ().find_first2 (1, index1, e ().size2 ());
        if (it2e != it2e_end && it2e.index2 () == index2)
            ++ it2e;
    }
    template<class E>
    BOOST_UBLAS_INLINE
    void restart (const matrix_expression<E> &e, typename E::size_type index1, typename E::size_type index2,
                  typename E::const_iterator2 &it2e, typename E::const_iterator2 &it2e_end,
                  typename E::const_iterator1 &it1e, typename E::const_iterator1 &it1e_end, column_major_tag) {
        it2e = e ().find_first2 (0, 0, index2);
        it2e_end = e ().find_first2 (0, 0, e ().size2 ());
        it1e = e ().find_first1 (1, index1, index2);
        it1e_end = e ().find_first1 (1, e ().size1 (), index2);
        if (it1e != it1e_end && it1e.index1 () == index1)
            ++ it1e;
    }
    template<class E>
    BOOST_UBLAS_INLINE
    void restart (matrix_expression<E> &e, typename E::size_type index1, typename E::size_type index2,
                  typename E::iterator1 &it1e, typename E::iterator1 &it1e_end,
                  typename E::iterator2 &it2e, typename E::iterator2 &it2e_end, row_major_tag) {
        it1e = e ().find_first1 (0, index1, 0);
        it1e_end = e ().find_first1 (0, e ().size1 (), 0);
        it2e = e ().find_first2 (1, index1, index2);
        it2e_end = e ().find_first2 (1, index1, e ().size2 ());
        if (it2e != it2e_end && it2e.index2 () == index2)
            ++ it2e;
    }
    template<class E>
    BOOST_UBLAS_INLINE
    void restart (matrix_expression<E> &e, typename E::size_type index1, typename E::size_type index2,
                  typename E::iterator2 &it2e, typename E::iterator2 &it2e_end,
                  typename E::iterator1 &it1e, typename E::iterator1 &it1e_end, column_major_tag) {
        it2e = e ().find_first2 (0, 0, index2);
        it2e_end = e ().find_first2 (0, 0, e ().size2 ());
        it1e = e ().find_first1 (1, index1, index2);
        it1e_end = e ().find_first1 (1, e ().size1 (), index2);
        if (it1e != it1e_end && it1e.index1 () == index1)
            ++ it1e;
    }

    // Iterating row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_matrix_assign_scalar (const F &f, M &m, const T &t, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        difference_type size2 (m.size2 ());
        typename M::iterator1 it1 (m.begin1 ());
        BOOST_UBLAS_CHECK (size2 == 0 || m.end1 () - it1 == size1, bad_size ());
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            BOOST_UBLAS_CHECK (it1.end () - it2 == size2, bad_size ());
            difference_type temp_size2 (size2);
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            while (-- temp_size2 >= 0)
                functor_type () (*it2, t), ++ it2;
#else
            DD (temp_size2, 4, r, (functor_type () (*it2, t), ++ it2));
#endif
            ++ it1;
        }
    }
    // Iterating column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_matrix_assign_scalar (const F &f, M &m, const T &t, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (m.size2 ());
        difference_type size1 (m.size1 ());
        typename M::iterator2 it2 (m.begin2 ());
        BOOST_UBLAS_CHECK (size1 == 0 || m.end2 () - it2 == size2, bad_size ());
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            BOOST_UBLAS_CHECK (it2.end () - it1 == size1, bad_size ());
            difference_type temp_size1 (size1);
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            while (-- temp_size1 >= 0)
                functor_type () (*it1, t), ++ it1;
#else
            DD (temp_size1, 4, r, (functor_type () (*it1, t), ++ it1));
#endif
            ++ it2;
        }
    }
    // Indexing row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_matrix_assign_scalar (const F &f, M &m, const T &t, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        difference_type size2 (m.size2 ());
        for (difference_type i = 0; i < size1; ++ i) {
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            for (difference_type j = 0; j < size2; ++ j)
                functor_type () (m (i, j), t);
#else
            difference_type j (0);
            DD (size2, 4, r, (functor_type () (m (i, j), t), ++ j));
#endif
        }
    }
    // Indexing column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_matrix_assign_scalar (const F &f, M &m, const T &t, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (m.size2 ());
        difference_type size1 (m.size1 ());
        for (difference_type j = 0; j < size2; ++ j)
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size1; ++ i) {
                functor_type () (m (i, j), t);
#else
            difference_type i (0);
            DD (size1, 4, r, (functor_type () (m (i, j), t), ++ i));
#endif
        }
    }

    // Dense (proxy) case
    template<class F, class M, class T, class C>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t, dense_proxy_tag, C c) {
        typedef F functor_type;
        typedef C orientation_category;
#ifdef BOOST_UBLAS_USE_INDEXING
        indexing_matrix_assign_scalar (functor_type (), m, t, orientation_category ());
#elif BOOST_UBLAS_USE_ITERATING
        iterating_matrix_assign_scalar (functor_type (), m, t, orientation_category ());
#else
        typedef typename M::difference_type difference_type;
        difference_type size1 (m.size1 ());
        difference_type size2 (m.size2 ());
        if (size1 >= BOOST_UBLAS_ITERATOR_THRESHOLD &&
            size2 >= BOOST_UBLAS_ITERATOR_THRESHOLD)
            iterating_matrix_assign_scalar (functor_type (), m, t, orientation_category ());
        else
            indexing_matrix_assign_scalar (functor_type (), m, t, orientation_category ());
#endif
    }
    // Packed (proxy) row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t, packed_proxy_tag, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        difference_type size1 (m.end1 () - it1);
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            difference_type size2 (it1.end () - it2);
            while (-- size2 >= 0)
                functor_type () (*it2, t), ++ it2;
            ++ it1;
        }
    }
    // Packed (proxy) column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t, packed_proxy_tag, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        difference_type size2 (m.end2 () - it2);
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            difference_type size1 (it2.end () - it1);
            while (-- size1 >= 0)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
    }
    // Sparse (proxy) row major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t, sparse_proxy_tag, row_major_tag) {
        typedef F functor_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end)
                functor_type () (*it2, t), ++ it2;
            ++ it1;
        }
    }
    // Sparse (proxy) column major case
    template<class F, class M, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t, sparse_proxy_tag, column_major_tag) {
        typedef F functor_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end)
                functor_type () (*it1, t), ++ it1;
            ++ it2;
        }
    }

    // Dispatcher
    template<class F, class M, class T>
    BOOST_UBLAS_INLINE
    void matrix_assign_scalar (const F &f, M &m, const T &t) {
        typedef F functor_type;
        typedef typename M::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;
        matrix_assign_scalar (functor_type (), m, t, storage_category (), orientation_category ());
    }

    template<class LS, class A, class RI1, class RI2>
    struct matrix_assign_traits {
        typedef LS storage_category;
    };

    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<dense_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<packed_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<packed_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_assign_traits<sparse_tag, computed_assign_tag, dense_random_access_iterator_tag, dense_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<sparse_tag, computed_assign_tag, packed_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct matrix_assign_traits<sparse_tag, computed_assign_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    // Iterating row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), e ().size1 ()));
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), e ().size2 ()));
        typename M::iterator1 it1 (m.begin1 ());
        BOOST_UBLAS_CHECK (size2 == 0 || m.end1 () - it1 == size1, bad_size ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        BOOST_UBLAS_CHECK (size2 == 0 || e ().end1 () - it1e == size1, bad_size ());
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            BOOST_UBLAS_CHECK (it1.end () - it2 == size2, bad_size ());
            typename E::const_iterator2 it2e (it1e.begin ());
            BOOST_UBLAS_CHECK (it1e.end () - it2e == size2, bad_size ());
            difference_type temp_size2 (size2);
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            while (-- temp_size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
#else
            DD (temp_size2, 2, r, (functor_type () (*it2, *it2e), ++ it2, ++ it2e));
#endif
            ++ it1, ++ it1e;
        }
    }
    // Iterating column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), e ().size2 ()));
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), e ().size1 ()));
        typename M::iterator2 it2 (m.begin2 ());
        BOOST_UBLAS_CHECK (size1 == 0 || m.end2 () - it2 == size2, bad_size ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        BOOST_UBLAS_CHECK (size1 == 0 || e ().end2 () - it2e == size2, bad_size ());
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            BOOST_UBLAS_CHECK (it2.end () - it1 == size1, bad_size ());
            typename E::const_iterator1 it1e (it2e.begin ());
            BOOST_UBLAS_CHECK (it2e.end () - it1e == size1, bad_size ());
            difference_type temp_size1 (size1);
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            while (-- temp_size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
#else
            DD (temp_size1, 2, r, (functor_type () (*it1, *it1e), ++ it1, ++ it1e));
#endif
            ++ it2, ++ it2e;
        }
    }
    // Indexing row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, row_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), e ().size1 ()));
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), e ().size2 ()));
        for (difference_type i = 0; i < size1; ++ i) {
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            for (difference_type j = 0; j < size2; ++ j)
                functor_type () (m (i, j), e () (i, j));
#else
            difference_type j (0);
            DD (size2, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ j));
#endif
        }
    }
    // Indexing column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_matrix_assign (const F &f, M &m, const matrix_expression<E> &e, column_major_tag) {
        typedef F functor_type;
        typedef typename M::difference_type difference_type;
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), e ().size2 ()));
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), e ().size1 ()));
        for (difference_type j = 0; j < size2; ++ j)
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
            for (difference_type i = 0; i < size1; ++ i) {
                functor_type () (m (i, j), e () (i, j));
#else
            difference_type i (0);
            DD (size1, 2, r, (functor_type () (m (i, j), e () (i, j)), ++ i));
#endif
        }
    }

    // Dense (proxy) case
    template<class F, class M, class E, class C>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F &f, M &m, const matrix_expression<E> &e, full, dense_proxy_tag, C c) {
        typedef F functor_type;
        typedef C orientation_category;
#ifdef BOOST_UBLAS_USE_INDEXING
        indexing_matrix_assign (functor_type (), m, e, orientation_category ());
#elif BOOST_UBLAS_USE_ITERATING
        iterating_matrix_assign (functor_type (), m, e, orientation_category ());
#else
        typedef typename M::difference_type difference_type;
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), e ().size1 ()));
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), e ().size2 ()));
        if (size1 >= BOOST_UBLAS_ITERATOR_THRESHOLD &&
            size2 >= BOOST_UBLAS_ITERATOR_THRESHOLD)
            iterating_matrix_assign (functor_type (), m, e, orientation_category ());
        else
            indexing_matrix_assign (functor_type (), m, e, orientation_category ());
#endif
    }
    // Packed (proxy) row major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F1 &f1, M &m, const matrix_expression<E> &e, const F2 &f2, packed_proxy_tag, row_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, row_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, row_major_tag ());
        indexing_matrix_assign (functor1_type (), cm, e, row_major_tag ());
#endif
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        difference_type it1_size (it1_end - it1);
        difference_type it1e_size (it1e_end - it1e);
        difference_type diff1 (0);
        if (it1_size > 0 && it1e_size > 0)
            diff1 = it1.index1 () - it1e.index1 ();
        if (diff1 != 0) {
            difference_type size1 = std::min (diff1, it1e_size);
            if (size1 > 0) {
                it1e += size1;
                it1e_size -= size1;
                diff1 -= size1;
            }
            size1 = std::min (- diff1, it1_size);
            if (size1 > 0) {
                it1_size -= size1;
                while (-- size1 >= 0) {
                    typename M::iterator2 it2 (it1.begin ());
                    typename M::iterator2 it2_end (it1.end ());
                    difference_type size2 (it2_end - it2);
                    while (-- size2 >= 0)
                        functor1_type () (*it2, value_type ()), ++ it2;
                    ++ it1;
                }
                diff1 += size1;
            }
        }
        difference_type size1 (std::min (it1_size, it1e_size));
        it1_size -= size1;
        it1e_size -= size1;
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            difference_type it2_size (it2_end - it2);
            difference_type it2e_size (it2e_end - it2e);
            difference_type diff2 (0);
            if (it2_size > 0 && it2e_size > 0) {
                diff2 = it2.index2 () - it2e.index2 ();
                difference_type size2 = std::min (diff2, it2e_size);
                if (size2 > 0) {
                    it2e += size2;
                    it2e_size -= size2;
                    diff2 -= size2;
                }
                size2 = std::min (- diff2, it2_size);
                if (size2 > 0) {
                    it2_size -= size2;
                    while (-- size2 >= 0)
                        functor1_type () (*it2, value_type ()), ++ it2;
                    diff2 += size2;
                }
            }
            difference_type size2 (std::min (it2_size, it2e_size));
            it2_size -= size2;
            it2e_size -= size2;
            while (-- size2 >= 0)
                functor1_type () (*it2, *it2e), ++ it2, ++ it2e;
            size2 = it2_size;
            while (-- size2 >= 0)
                functor1_type () (*it2, value_type ()), ++ it2;
            ++ it1, ++ it1e;
        }
        size1 = it1_size;
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            difference_type size2 (it2_end - it2);
            while (-- size2 >= 0)
                functor1_type () (*it2, value_type ()), ++ it2;
            ++ it1;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }
    // Packed (proxy) column major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F1 &f1, M &m, const matrix_expression<E> &e, const F2 &f2, packed_proxy_tag, column_major_tag) {
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, column_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, column_major_tag ());
        indexing_matrix_assign (functor1_type (), cm, e, column_major_tag ());
#endif
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        difference_type it2_size (it2_end - it2);
        difference_type it2e_size (it2e_end - it2e);
        difference_type diff2 (0);
        if (it2_size > 0 && it2e_size > 0)
            diff2 = it2.index2 () - it2e.index2 ();
        if (diff2 != 0) {
            difference_type size2 = std::min (diff2, it2e_size);
            if (size2 > 0) {
                it2e += size2;
                it2e_size -= size2;
                diff2 -= size2;
            }
            size2 = std::min (- diff2, it2_size);
            if (size2 > 0) {
                it2_size -= size2;
                while (-- size2 >= 0) {
                    typename M::iterator1 it1 (it2.begin ());
                    typename M::iterator1 it1_end (it2.end ());
                    difference_type size1 (it1_end - it1);
                    while (-- size1 >= 0)
                        functor1_type () (*it1, value_type ()), ++ it1;
                    ++ it2;
                }
                diff2 += size2;
            }
        }
        difference_type size2 (std::min (it2_size, it2e_size));
        it2_size -= size2;
        it2e_size -= size2;
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            difference_type it1_size (it1_end - it1);
            difference_type it1e_size (it1e_end - it1e);
            difference_type diff1 (0);
            if (it1_size > 0 && it1e_size > 0) {
                diff1 = it1.index1 () - it1e.index1 ();
                difference_type size1 = std::min (diff1, it1e_size);
                if (size1 > 0) {
                    it1e += size1;
                    it1e_size -= size1;
                    diff1 -= size1;
                }
                size1 = std::min (- diff1, it1_size);
                if (size1 > 0) {
                    it1_size -= size1;
                    while (-- size1 >= 0)
                        functor1_type () (*it1, value_type ()), ++ it1;
                    diff1 += size1;
                }
            }
            difference_type size1 (std::min (it1_size, it1e_size));
            it1_size -= size1;
            it1e_size -= size1;
            while (-- size1 >= 0)
                functor1_type () (*it1, *it1e), ++ it1, ++ it1e;
            size1 = it1_size;
            while (-- size1 >= 0)
                functor1_type () (*it1, value_type ()), ++ it1;
            ++ it2, ++ it2e;
        }
        size2 = it2_size;
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            difference_type size1 (it1_end - it1);
            while (-- size1 >= 0)
                functor1_type () (*it1, value_type ()), ++ it1;
            ++ it2;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }
    // Sparse row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F &f, M &m, const matrix_expression<E> &e, full, sparse_tag, row_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, row_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, row_major_tag ());
        indexing_matrix_assign (functor_type (), cm, e, row_major_tag ());
#endif
        m.clear ();
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        while (it1e != it1e_end) {
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            while (it2e != it2e_end) {
                value_type t (*it2e);
                if (t != value_type ())
                    m.insert (it2e.index1 (), it2e.index2 (), t);
                ++ it2e;
            }
            ++ it1e;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }
    // Sparse column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F &f, M &m, const matrix_expression<E> &e, full, sparse_tag, column_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F functor_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, column_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, column_major_tag ());
        indexing_matrix_assign (functor_type (), cm, e, column_major_tag ());
#endif
        m.clear ();
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        while (it2e != it2e_end) {
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            while (it1e != it1e_end) {
                value_type t (*it1e);
                if (t != value_type ())
                    m.insert (it1e.index1 (), it1e.index2 (), t);
                ++ it1e;
            }
            ++ it2e;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }
    // Sparse proxy row major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F1 &f1, M &m, const matrix_expression<E> &e, const F2 &f2, sparse_proxy_tag, row_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, row_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, row_major_tag ());
        indexing_matrix_assign (functor1_type (), cm, e, row_major_tag ());
#endif
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::const_iterator1 it1e (e ().begin1 ());
        typename E::const_iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index1 () - it1e.index1 ();
            if (compare == 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index2 () - it2e.index2 ();
                    if (compare == 0) {
                        functor1_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
                        functor1_type () (*it2, value_type ());
                        ++ it2;
                    } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                            // The proxies could reference the same container.
                            restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        } else {
                            ++ it2e;
                        }
#else
                        ++ it2e;
#endif
                    }
                }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                while (it2e != it2e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                    } else {
                        ++ it2e;
                    }
                }
#endif
                while (it2 != it2_end) {
                    functor1_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1, ++ it1e;
            } else if (compare < 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                while (it2 != it2_end) {
                    functor1_type () (*it2, value_type ());
                    ++ it2;
                }
                ++ it1;
            } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator2 it2;
                typename M::iterator2 it2_end;
                typename E::const_iterator2 it2e (it1e.begin ());
                typename E::const_iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                    } else {
                        ++ it2e;
                    }
                }
#endif
                ++ it1e;
            }
        }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        while (it1e != it1e_end) {
            typename M::iterator2 it2;
            typename M::iterator2 it2_end;
            typename E::const_iterator2 it2e (it1e.begin ());
            typename E::const_iterator2 it2e_end (it1e.end ());
            while (it2e != it2e_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                    restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                } else {
                    ++ it2e;
                }
            }
            ++ it1e;
        }
#endif
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            while (it2 != it2_end) {
                functor1_type () (*it2, value_type ());
                ++ it2;
            }
            ++ it1;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }
    // Sparse proxy column major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_assign (const F1 &f1, M &m, const matrix_expression<E> &e, const F2 &f2, sparse_proxy_tag, column_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
#ifdef BOOST_UBLAS_TYPE_CHECK
        matrix<value_type, column_major> cm (m.size1 (), m.size2 ());
        indexing_matrix_assign (scalar_assign<value_type, value_type> (), cm, m, column_major_tag ());
        indexing_matrix_assign (functor1_type (), cm, e, column_major_tag ());
#endif
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::const_iterator2 it2e (e ().begin2 ());
        typename E::const_iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index2 () - it2e.index2 ();
            if (compare == 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index1 () - it1e.index1 ();
                    if (compare == 0) {
                        functor1_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
                        functor1_type () (*it1, value_type ());
                        ++ it1;
                    } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                            // The proxies could reference the same container.
                            restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        } else {
                            ++ it1e;
                        }
#else
                        ++ it1e;
#endif
                    }
                }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                while (it1e != it1e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                    } else {
                        ++ it1e;
                    }
                }
#endif
                while (it1 != it1_end) {
                    functor1_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2, ++ it2e;
            } else if (compare < 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                while (it1 != it1_end) {
                    functor1_type () (*it1, value_type ());
                    ++ it1;
                }
                ++ it2;
            } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator1 it1;
                typename M::iterator1 it1_end;
                typename E::const_iterator1 it1e (it2e.begin ());
                typename E::const_iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                    } else {
                        ++ it1e;
                    }
                }
#endif
                ++ it2e;
            }
        }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        while (it2e != it2e_end) {
            typename M::iterator1 it1;
            typename M::iterator1 it1_end;
            typename E::const_iterator1 it1e (it2e.begin ());
            typename E::const_iterator1 it1e_end (it2e.end ());
            while (it1e != it1e_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                    restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                } else {
                    ++ it1e;
                }
            }
            ++ it2e;
        }
#endif
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            while (it1 != it1_end) {
                functor1_type () (*it1, value_type ());
                ++ it1;
            }
            ++ it2;
        }
#ifdef BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check)
            BOOST_UBLAS_CHECK (equals (m, cm), external_logic ());
#endif
    }

    // Dispatcher
    template<class F, class M, class E>
    BOOST_UBLAS_INLINE
    void matrix_assign (const F &f, M &m, const matrix_expression<E> &e) {
        typedef F functor_type;
        typedef typename matrix_assign_traits<BOOST_UBLAS_TYPENAME M::storage_category,
                                              BOOST_UBLAS_TYPENAME F::assign_category,
                                              BOOST_UBLAS_TYPENAME E::const_iterator1::iterator_category,
                                              BOOST_UBLAS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
        // FIXME: can't we improve the dispatch here?
        // typedef typename E::orientation_category orientation_category;
        typedef typename M::orientation_category orientation_category;
        matrix_assign (functor_type (), m, e, full (), storage_category (), orientation_category ());
    }
    template<class F1, class M, class E, class F2>
    BOOST_UBLAS_INLINE
    void matrix_assign (const F1 &f1, M &m, const matrix_expression<E> &e, const F2 &f2) {
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename matrix_assign_traits<BOOST_UBLAS_TYPENAME M::storage_category,
                                              BOOST_UBLAS_TYPENAME F1::assign_category,
                                              BOOST_UBLAS_TYPENAME E::const_iterator1::iterator_category,
                                              BOOST_UBLAS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
        // FIXME: can't we improve the dispatch here?
        // typedef typename E::orientation_category orientation_category;
        typedef typename M::orientation_category orientation_category;
        matrix_assign (functor1_type (), m, e, functor2_type (), storage_category (), orientation_category ());
    }

    template<class LS, class RI1, class RI2>
    struct matrix_swap_traits {
        typedef LS storage_category;
    };

    template<>
    struct matrix_swap_traits<dense_proxy_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct matrix_swap_traits<packed_proxy_tag, sparse_bidirectional_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    // Dense (proxy) row major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F &f, M &m, matrix_expression<E> &e, full, dense_proxy_tag, row_major_tag) {
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), size_type (e ().end1 () - it1e)));
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            typename E::iterator2 it2e (it1e.begin ());
            difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), size_type (it1e.end () - it2e)));
            while (-- size2 >= 0)
                functor_type () (*it2, *it2e), ++ it2, ++ it2e;
            ++ it1, ++ it1e;
        }
    }
    // Dense (proxy) column major case
    template<class F, class M, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F &f, M &m, matrix_expression<E> &e, full, dense_proxy_tag, column_major_tag) {
        typedef F functor_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (BOOST_UBLAS_SAME (m.size2 (), size_type (e ().end2 () - it2e)));
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename E::iterator1 it1e (it2e.begin ());
            difference_type size1 (BOOST_UBLAS_SAME (m.size1 (), size_type (it2e.end () - it1e)));
            while (-- size1 >= 0)
                functor_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
    }
    // Packed (proxy) row major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F1 &f1, M &m, matrix_expression<E> &e, const F2 &f2, packed_proxy_tag, row_major_tag) {
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        difference_type size1 (BOOST_UBLAS_SAME (m.end1 () - it1, e ().end1 () - it1e));
        while (-- size1 >= 0) {
            typename M::iterator2 it2 (it1.begin ());
            typename E::iterator2 it2e (it1e.begin ());
            difference_type size2 (BOOST_UBLAS_SAME (it1.end () - it2, it1e.end () - it2e));
            while (-- size2 >= 0)
                functor1_type () (*it2, *it2e), ++ it2, ++ it2e;
            ++ it1, ++ it1e;
        }
    }
    // Packed (proxy) column major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F1 &f1, M &m, matrix_expression<E> &e, const F2 &f2, packed_proxy_tag, column_major_tag) {
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        difference_type size2 (BOOST_UBLAS_SAME (m.end2 () - it2, e ().end2 () - it2e));
        while (-- size2 >= 0) {
            typename M::iterator1 it1 (it2.begin ());
            typename E::iterator1 it1e (it2e.begin ());
            difference_type size1 (BOOST_UBLAS_SAME (it2.end () - it1, it2e.end () - it1e));
            while (-- size1 >= 0)
                functor1_type () (*it1, *it1e), ++ it1, ++ it1e;
            ++ it2, ++ it2e;
        }
    }
    // Sparse (proxy) row major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F1 &f1, M &m, matrix_expression<E> &e, const F2 &f2, sparse_proxy_tag, row_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator1 it1 (m.begin1 ());
        typename M::iterator1 it1_end (m.end1 ());
        typename E::iterator1 it1e (e ().begin1 ());
        typename E::iterator1 it1e_end (e ().end1 ());
        while (it1 != it1_end && it1e != it1e_end) {
            int compare = it1.index1 () - it1e.index1 ();
            if (compare == 0) {
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::iterator2 it2e (it1e.begin ());
                typename E::iterator2 it2e_end (it1e.end ());
                while (it2 != it2_end && it2e != it2e_end) {
                    int compare = it2.index2 () - it2e.index2 ();
                    if (compare == 0) {
                        functor1_type () (*it2, *it2e);
                        ++ it2, ++ it2e;
                    } else if (compare < 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it2.index1 ()), index2 (it2.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                            // The proxies could reference the same container.
                            restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                        } else {
                            ++ it2;
                        }
#else
                        ++ it2;
#endif
                    } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                            // The proxies could reference the same container.
                            restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        } else {
                            ++ it2e;
                        }
#else
                        ++ it2e;
#endif
                    }
                }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                while (it2e != it2e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                    } else {
                        ++ it2e;
                    }
                }
                while (it2 != it2_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2.index1 ()), index2 (it2.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                    } else {
                        ++ it2;
                    }
                }
#endif
                ++ it1, ++ it1e;
            } else if (compare < 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator2 it2 (it1.begin ());
                typename M::iterator2 it2_end (it1.end ());
                typename E::iterator2 it2e;
                typename E::iterator2 it2e_end;
                while (it2 != it2_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2.index1 ()), index2 (it2.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                    } else {
                        ++ it2;
                    }
                }
#endif
                ++ it1;
            } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator2 it2;
                typename M::iterator2 it2_end;
                typename E::iterator2 it2e (it1e.begin ());
                typename E::iterator2 it2e_end (it1e.end ());
                while (it2e != it2e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                        restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                    } else {
                        ++ it2e;
                    }
                }
#endif
                ++ it1e;
            }
        }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        while (it1e != it1e_end) {
            typename M::iterator2 it2;
            typename M::iterator2 it2_end;
            typename E::iterator2 it2e (it1e.begin ());
            typename E::iterator2 it2e_end (it1e.end ());
            while (it2e != it2e_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it2e.index1 ()), index2 (it2e.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                    restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                } else {
                    ++ it2e;
                }
            }
            ++ it1e;
        }
        while (it1 != it1_end) {
            typename M::iterator2 it2 (it1.begin ());
            typename M::iterator2 it2_end (it1.end ());
            typename E::iterator2 it2e;
            typename E::iterator2 it2e_end;
            while (it2 != it2_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it2.index1 ()), index2 (it2.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (m, index1, index2, it1, it1_end, it2, it2_end, row_major_tag ());
                    restart (e, index1, index2, it1e, it1e_end, it2e, it2e_end, row_major_tag ());
                } else {
                    ++ it2;
                }
            }
            ++ it1;
        }
#endif
    }
    // Sparse (proxy) column major case
    template<class F1, class M, class E, class F2>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void matrix_swap (const F1 &f1, M &m, matrix_expression<E> &e, const F2 &f2, sparse_proxy_tag, column_major_tag) {
        BOOST_UBLAS_CHECK (m.size1 () == e ().size1 (), bad_size ());
        BOOST_UBLAS_CHECK (m.size2 () == e ().size2 (), bad_size ());
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename M::size_type size_type;
        typedef typename M::value_type value_type;
        typename M::iterator2 it2 (m.begin2 ());
        typename M::iterator2 it2_end (m.end2 ());
        typename E::iterator2 it2e (e ().begin2 ());
        typename E::iterator2 it2e_end (e ().end2 ());
        while (it2 != it2_end && it2e != it2e_end) {
            int compare = it2.index2 () - it2e.index2 ();
            if (compare == 0) {
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::iterator1 it1e (it2e.begin ());
                typename E::iterator1 it1e_end (it2e.end ());
                while (it1 != it1_end && it1e != it1e_end) {
                    int compare = it1.index1 () - it1e.index1 ();
                    if (compare == 0) {
                        functor1_type () (*it1, *it1e);
                        ++ it1, ++ it1e;
                    } else if (compare < 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it1.index1 ()), index2 (it1.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                            // The proxies could reference the same container.
                            restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                        } else {
                            ++ it1;
                        }
#else
                        ++ it1;
#endif
                    } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                        // Sparse proxies don't need to be conformant.
                        // Thanks to Michael Stevens for suggesting this.
                        size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                        if (functor2_type::other (index1, index2)) {
                            functor1_type () (m (index1, index2), e () (index1, index2));
                            restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                            // The proxies could reference the same container.
                            restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        } else {
                            ++ it1e;
                        }
#else
                        ++ it1e;
#endif
                    }
                }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                while (it1e != it1e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                    } else {
                        ++ it1e;
                    }
                }
                while (it1 != it1_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1.index1 ()), index2 (it1.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                    } else {
                        ++ it1;
                    }
                }
#endif
                ++ it2, ++ it2e;
            } else if (compare < 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator1 it1 (it2.begin ());
                typename M::iterator1 it1_end (it2.end ());
                typename E::iterator1 it1e;
                typename E::iterator1 it1e_end;
                while (it1 != it1_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1.index1 ()), index2 (it1.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                    } else {
                        ++ it1;
                    }
                }
#endif
                ++ it2;
            } else if (compare > 0) {
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
                typename M::iterator1 it1;
                typename M::iterator1 it1_end;
                typename E::iterator1 it1e (it2e.begin ());
                typename E::iterator1 it1e_end (it2e.end ());
                while (it1e != it1e_end) {
                    // Sparse proxies don't need to be conformant.
                    // Thanks to Michael Stevens for suggesting this.
                    size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                    if (functor2_type::other (index1, index2)) {
                        functor1_type () (m (index1, index2), e () (index1, index2));
                        // The proxies could reference the same container.
                        restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                        restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                    } else {
                        ++ it1e;
                    }
                }
#endif
                ++ it2e;
            }
        }
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        while (it2e != it2e_end) {
            typename M::iterator1 it1;
            typename M::iterator1 it1_end;
            typename E::iterator1 it1e (it2e.begin ());
            typename E::iterator1 it1e_end (it2e.end ());
            while (it1e != it1e_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it1e.index1 ()), index2 (it1e.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                    restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                } else {
                    ++ it1e;
                }
            }
            ++ it2e;
        }
        while (it2 != it2_end) {
            typename M::iterator1 it1 (it2.begin ());
            typename M::iterator1 it1_end (it2.end ());
            typename E::iterator1 it1e;
            typename E::iterator1 it1e_end;
            while (it1 != it1_end) {
                // Sparse proxies don't need to be conformant.
                // Thanks to Michael Stevens for suggesting this.
                size_type index1 (it1.index1 ()), index2 (it1.index2 ());
                if (functor2_type::other (index1, index2)) {
                    functor1_type () (m (index1, index2), e () (index1, index2));
                    // The proxies could reference the same container.
                    restart (m, index1, index2, it2, it2_end, it1, it1_end, column_major_tag ());
                    restart (e, index1, index2, it2e, it2e_end, it1e, it1e_end, column_major_tag ());
                } else {
                    ++ it1;
                }
            }
            ++ it2;
        }
#endif
    }

    // Dispatcher
    template<class F, class M, class E>
    BOOST_UBLAS_INLINE
    void matrix_swap (const F &f, M &m, matrix_expression<E> &e) {
        typedef F functor_type;
        typedef typename matrix_swap_traits<BOOST_UBLAS_TYPENAME M::storage_category,
                                            BOOST_UBLAS_TYPENAME E::const_iterator1::iterator_category,
                                            BOOST_UBLAS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
        // FIXME: can't we improve the dispatch here?
        // typedef typename E::orientation_category orientation_category;
        typedef typename M::orientation_category orientation_category;
        matrix_swap (functor_type (), m, e, full (), storage_category (), orientation_category ());
    }
    template<class F1, class M, class E, class F2>
    BOOST_UBLAS_INLINE
    void matrix_swap (const F1 &f1, M &m, matrix_expression<E> &e, const F2 &f2) {
        typedef F1 functor1_type;
        typedef F2 functor2_type;
        typedef typename matrix_swap_traits<BOOST_UBLAS_TYPENAME M::storage_category,
                                            BOOST_UBLAS_TYPENAME E::const_iterator1::iterator_category,
                                            BOOST_UBLAS_TYPENAME E::const_iterator2::iterator_category>::storage_category storage_category;
        // FIXME: can't we improve the dispatch here?
        // typedef typename E::orientation_category orientation_category;
        typedef typename M::orientation_category orientation_category;
        matrix_swap (functor1_type (), m, e, functor2_type (), storage_category (), orientation_category ());
    }

}}}

#endif




















