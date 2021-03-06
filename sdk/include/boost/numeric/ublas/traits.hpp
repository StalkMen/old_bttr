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

#ifndef BOOST_UBLAS_TRAITS_H
#define BOOST_UBLAS_TRAITS_H

#include <algorithm>
#include <cmath>
#include <complex>

#include <boost/numeric/ublas/config.hpp>

// Promote traits borrowed from Todd Veldhuizen

namespace boost_cryray { namespace numeric { namespace ublas {

    template<class T>
    struct type_traits {
        typedef type_traits<T> self_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef T real_type;
        typedef T precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 0);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 0);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t);
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t);
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t);

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t);
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t);

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };

    template<>
    struct type_traits<float> {
        typedef type_traits<float> self_type;
        typedef float value_type;
        typedef float const_reference;
        typedef float &reference;
        typedef float real_type;
        typedef double precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::fabsf (t);
#else
            return std::abs (t);
#endif
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::sqrtf (t);
#else
            return std::sqrt (t);
#endif
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
    template<>
    struct type_traits<double> {
        typedef type_traits<double> self_type;
        typedef double value_type;
        typedef double const_reference;
        typedef double &reference;
        typedef double real_type;
#ifndef BOOST_UBLAS_USE_LONG_DOUBLE
        typedef double precision_type;
#else
        typedef long double precision_type;
#endif

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::fabs (t);
#else
            return std::abs (t);
#endif
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::sqrt (t);
#else
            return std::sqrt (t);
#endif
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct type_traits<long double> {
        typedef type_traits<long double> self_type;
        typedef long double value_type;
        typedef long double const_reference;
        typedef long double &reference;
        typedef long double real_type;
        typedef long double precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::fabsl (t);
#else
            return std::abs (t);
#endif
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
#if defined (BOOST_NO_STDC_NAMESPACE) || defined (BOOST_UBLAS_NO_CMATH)
            return ::sqrtl (t);
#else
            return std::sqrt (t);
#endif
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#endif

    template<>
    struct type_traits<std::complex<float> > {
        typedef type_traits<std::complex<float> > self_type;
        typedef std::complex<float> value_type;
        typedef const std::complex<float> &const_reference;
        typedef std::complex<float> &reference;
        typedef float real_type;
        typedef std::complex<double> precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return std::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return std::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
    template<>
    struct type_traits<std::complex<double> > {
        typedef type_traits<std::complex<double> > self_type;
        typedef std::complex<double> value_type;
        typedef const std::complex<double> &const_reference;
        typedef std::complex<double> &reference;
        typedef double real_type;
#ifndef BOOST_UBLAS_USE_LONG_DOUBLE
        typedef std::complex<double> precision_type;
#else
        typedef std::complex<long double> precision_type;
#endif

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return std::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return std::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct type_traits<std::complex<long double> > {
        typedef type_traits<std::complex<long double> > self_type;
        typedef std::complex<long double> value_type;
        typedef const std::complex<long double> &const_reference;
        typedef std::complex<long double> &reference;
        typedef long double real_type;
        typedef std::complex<long double> precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return std::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return std::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#endif

    template<class T1, class T2>
    struct promote_traits {
        // Default promotion will badly fail, if the types are different.
        // Thanks to Kresimir Fresl for spotting this.
        BOOST_STATIC_ASSERT ((boost_cryray::is_same<T1, T2>::value));
        typedef T1 promote_type;
    };

    template<>
    struct promote_traits<float, double> {
        typedef double promote_type;
    };
    template<>
    struct promote_traits<double, float> {
        typedef double promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<float, long double> {
        typedef long double promote_type;
    };
    template<>
    struct promote_traits<long double, float> {
        typedef long double promote_type;
    };
    template<>
    struct promote_traits<double, long double> {
        typedef long double promote_type;
    };
    template<>
    struct promote_traits<long double, double> {
        typedef long double promote_type;
    };
#endif

    template<>
    struct promote_traits<float, std::complex<float> > {
        typedef std::complex<float> promote_type;
    };
    template<>
    struct promote_traits<std::complex<float>, float> {
        typedef std::complex<float> promote_type;
    };
    template<>
    struct promote_traits<float, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, float> {
        typedef std::complex<double> promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<float, std::complex<long double> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<long double>, float> {
        typedef std::complex<long double> promote_type;
    };
#endif

    template<>
    struct promote_traits<double, std::complex<float> > {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<float>, double> {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<double, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, double> {
        typedef std::complex<double> promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<double, std::complex<long double> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<long double>, double> {
        typedef std::complex<long double> promote_type;
    };
#endif

#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<long double, std::complex<float> > {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<float>, long double> {
        // Here we'd better go the conservative way.
        // typedef std::complex<float> promote_type;
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<long double, std::complex<double> > {
        // Here we'd better go the conservative way.
        // typedef std::complex<double> promote_type;
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, long double> {
        // Here we'd better go the conservative way.
        // typedef std::complex<double> promote_type;
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<long double, std::complex<long double> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<long double>, long double> {
        typedef std::complex<long double> promote_type;
    };
#endif

    template<>
    struct promote_traits<std::complex<float>, std::complex<double> > {
        typedef std::complex<double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, std::complex<float> > {
        typedef std::complex<double> promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<std::complex<float>, std::complex<long double> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<long double>, std::complex<float> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<double>, std::complex<long double> > {
        typedef std::complex<long double> promote_type;
    };
    template<>
    struct promote_traits<std::complex<long double>, std::complex<double> > {
        typedef std::complex<long double> promote_type;
    };
#endif

#ifdef BOOST_UBLAS_USE_INTERVAL
    template<>
    struct type_traits<boost_cryray::numeric::interval<float> > {
        typedef type_traits<boost_cryray::numeric::interval<float> > self_type;
        typedef boost_cryray::numeric::interval<float> value_type;
        typedef const boost_cryray::numeric::interval<float> &const_reference;
        typedef boost_cryray::numeric::interval<float> &reference;
        typedef boost_cryray::numeric::interval<float> real_type;
        typedef boost_cryray::numeric::interval<double> precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
            return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
            return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
    template<>
    struct type_traits<boost_cryray::numeric::interval<double> > {
        typedef type_traits<boost_cryray::numeric::interval<double> > self_type;
        typedef boost_cryray::numeric::interval<double> value_type;
        typedef const boost_cryray::numeric::interval<double> &const_reference;
        typedef boost_cryray::numeric::interval<double> &reference;
        typedef boost_cryray::numeric::interval<double> real_type;
#ifndef BOOST_UBLAS_USE_LONG_DOUBLE
        typedef boost_cryray::numeric::interval<double> precision_type;
#else
        typedef boost_cryray::numeric::interval<boost_cryray::numeric::interval<long double> > precision_type;
#endif

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
            return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
            return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct type_traits<boost_cryray::numeric::interval<long double> > {
        typedef type_traits<boost_cryray::numeric::interval<long double> > self_type;
        typedef boost_cryray::numeric::interval<long double> value_type;
        typedef const boost_cryray::numeric::interval<long double> &const_reference;
        typedef boost_cryray::numeric::interval<long double> &reference;
        typedef boost_cryray::numeric::interval<long double> real_type;
        typedef boost_cryray::numeric::interval<long double> precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 1);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 1);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                return t;
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                return 0;
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                return t;
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
            return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
            return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            return self_type::abs (t);
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#endif

#ifdef BOOST_UBLAS_USE_BOOST_COMPLEX
    template<>
    struct type_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        typedef type_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> > > self_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > value_type;
        typedef const boost_cryray::complex<boost_cryray::numeric::interval<float> > &const_reference;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > &reference;
        typedef boost_cryray::numeric::interval<float> real_type;
        typedef boost_cryray::complex<double> precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
    template<>
    struct type_traits<boost_cryray::complex<double> > {
        typedef type_traits<boost_cryray::complex<double> > self_type;
        typedef boost_cryray::complex<double> value_type;
        typedef const boost_cryray::complex<double> &const_reference;
        typedef boost_cryray::complex<double> &reference;
        typedef double real_type;
#ifndef BOOST_UBLAS_USE_LONG_DOUBLE
        typedef boost_cryray::complex<double> precision_type;
#else
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > precision_type;
#endif

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct type_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef type_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> > > self_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > value_type;
        typedef const boost_cryray::complex<boost_cryray::numeric::interval<long double> > &const_reference;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > &reference;
        typedef boost_cryray::numeric::interval<long double> real_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > precision_type;

        BOOST_STATIC_CONSTANT (std::size_t, plus_complexity = 2);
        BOOST_STATIC_CONSTANT (std::size_t, multiplies_complexity = 6);

        static
        BOOST_UBLAS_INLINE
        real_type real (const_reference t) {
                // return t.real ();
                return std::real (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type imag (const_reference t) {
                // return t.imag ();
                return std::imag (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type conj (const_reference t) {
                // return t.conj ();
                return std::conj (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type abs (const_reference t) {
                return boost_cryray::numeric::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        value_type sqrt (const_reference t) {
                return boost_cryray::numeric::sqrt (t);
        }

        static
        BOOST_UBLAS_INLINE
        real_type norm_1 (const_reference t) {
            // Oops, should have known that!
            return type_traits<real_type>::abs (self_type::real (t)) +
                   type_traits<real_type>::abs (self_type::imag (t));
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_2 (const_reference t) {
            return self_type::abs (t);
        }
        static
        BOOST_UBLAS_INLINE
        real_type norm_inf (const_reference t) {
            // Oops, should have known that!
            return std::max (type_traits<real_type>::abs (self_type::real (t)),
                             type_traits<real_type>::abs (self_type::imag (t)));
        }

        static
        BOOST_UBLAS_INLINE
        bool equals (const_reference t1, const_reference t2) {
            // Check, that the values match at least half.
            static real_type sqrt_epsilon (type_traits<real_type>::sqrt (std::numeric_limits<real_type>::epsilon ()));
            return self_type::norm_inf (t1 - t2) < sqrt_epsilon *
                   std::max (std::max (self_type::norm_inf (t1),
                                       self_type::norm_inf (t2)),
                             std::numeric_limits<real_type>::min ());
        }
    };
#endif
#endif

    template<>
    struct promote_traits<boost_cryray::numeric::interval<float>, boost_cryray::numeric::interval<double> > {
        typedef boost_cryray::numeric::interval<double> promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<double>, boost_cryray::numeric::interval<float> > {
        typedef boost_cryray::numeric::interval<double> promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<boost_cryray::numeric::interval<float>, boost_cryray::numeric::interval<long double> > {
        typedef boost_cryray::numeric::interval<long double> promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<long double>, boost_cryray::numeric::interval<float> > {
        typedef boost_cryray::numeric::interval<long double> promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<double>, boost_cryray::numeric::interval<long double> > {
        typedef boost_cryray::numeric::interval<long double> promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<long double>, boost_cryray::numeric::interval<double> > {
        typedef boost_cryray::numeric::interval<long double> promote_type;
    };
#endif

#ifdef BOOST_UBLAS_USE_BOOST_COMPLEX
    template<>
    struct promote_traits<boost_cryray::numeric::interval<float>, boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> >, boost_cryray::numeric::interval<float> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<float>, boost_cryray::complex<boost_cryray::numeric::interval<double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<double> >, boost_cryray::numeric::interval<float> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<boost_cryray::numeric::interval<float>, boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> >, boost_cryray::numeric::interval<float> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
#endif

    template<>
    struct promote_traits<boost_cryray::numeric::interval<double>, boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> >, boost_cryray::numeric::interval<double> > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<double>, boost_cryray::complex<boost_cryray::numeric::interval<double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<double> >, boost_cryray::numeric::interval<double> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<boost_cryray::numeric::interval<double>, boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> >, boost_cryray::numeric::interval<double> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
#endif

#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<boost_cryray::numeric::interval<long double>, boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> >, boost_cryray::numeric::interval<long double> > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<float> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<long double>, boost_cryray::complex<boost_cryray::numeric::interval<double> > > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<double> >, boost_cryray::numeric::interval<long double> > {
        // Here we'd better go the conservative way.
        // typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::numeric::interval<long double>, boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> >, boost_cryray::numeric::interval<long double> > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
#endif

    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> >, boost_cryray::complex<boost_cryray::numeric::interval<double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<double> >, boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<double> > promote_type;
    };
#ifdef BOOST_UBLAS_USE_LONG_DOUBLE
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<float> >, boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> >, boost_cryray::complex<boost_cryray::numeric::interval<float> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<double> >, boost_cryray::complex<boost_cryray::numeric::interval<long double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
    template<>
    struct promote_traits<boost_cryray::complex<boost_cryray::numeric::interval<long double> >, boost_cryray::complex<boost_cryray::numeric::interval<double> > > {
        typedef boost_cryray::complex<boost_cryray::numeric::interval<long double> > promote_type;
    };
#endif
#endif
#endif

    struct unknown_storage_tag {};
    struct sparse_proxy_tag: public unknown_storage_tag {};
    struct sparse_tag: public sparse_proxy_tag {};
    struct packed_proxy_tag: public sparse_proxy_tag {};
    struct packed_tag: public packed_proxy_tag {};
    struct dense_proxy_tag: public packed_proxy_tag {};
    struct dense_tag: public dense_proxy_tag {};

    template<class S1, class S2>
    struct storage_restrict_traits {
        typedef S1 storage_category;
    };

    template<>
    struct storage_restrict_traits<sparse_tag, dense_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<sparse_tag, packed_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<sparse_tag, sparse_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct storage_restrict_traits<packed_tag, dense_proxy_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<packed_tag, packed_proxy_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<packed_tag, sparse_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct storage_restrict_traits<packed_proxy_tag, sparse_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct storage_restrict_traits<dense_tag, dense_proxy_tag> {
        typedef dense_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<dense_tag, packed_proxy_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<dense_tag, sparse_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct storage_restrict_traits<dense_proxy_tag, packed_proxy_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct storage_restrict_traits<dense_proxy_tag, sparse_proxy_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    struct sparse_bidirectional_iterator_tag : public std::bidirectional_iterator_tag {};
    struct packed_random_access_iterator_tag : public std::random_access_iterator_tag {};
    struct dense_random_access_iterator_tag : public packed_random_access_iterator_tag {};

    // Thanks to Kresimir Fresl for convincing Comeau with iterator_base_traits ;-)
    template<class IC>
    struct iterator_base_traits {};

    template<>
    struct iterator_base_traits<std::forward_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef forward_iterator_base<std::forward_iterator_tag, I, T> type;
        };
    };

    template<>
    struct iterator_base_traits<std::bidirectional_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef bidirectional_iterator_base<std::bidirectional_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<sparse_bidirectional_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef bidirectional_iterator_base<sparse_bidirectional_iterator_tag, I, T> type;
        };
    };

    template<>
    struct iterator_base_traits<std::random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<std::bidirectional_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<packed_random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<packed_random_access_iterator_tag, I, T> type;
        };
    };
    template<>
    struct iterator_base_traits<dense_random_access_iterator_tag> {
        template<class I, class T>
        struct iterator_base {
            typedef random_access_iterator_base<dense_random_access_iterator_tag, I, T> type;
        };
    };

    template<class I1, class I2>
    struct iterator_restrict_traits {
        typedef I1 iterator_category;
    };

    template<>
    struct iterator_restrict_traits<packed_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<sparse_bidirectional_iterator_tag, packed_random_access_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };

    template<>
    struct iterator_restrict_traits<dense_random_access_iterator_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<sparse_bidirectional_iterator_tag, dense_random_access_iterator_tag> {
        typedef sparse_bidirectional_iterator_tag iterator_category;
    };

    template<>
    struct iterator_restrict_traits<dense_random_access_iterator_tag, packed_random_access_iterator_tag> {
        typedef packed_random_access_iterator_tag iterator_category;
    };
    template<>
    struct iterator_restrict_traits<packed_random_access_iterator_tag, dense_random_access_iterator_tag> {
        typedef packed_random_access_iterator_tag iterator_category;
    };

}}}

#endif


