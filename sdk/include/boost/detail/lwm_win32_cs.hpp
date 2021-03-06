#ifndef BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
#define BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/lwm_win32_cs.hpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifdef BOOST_USE_WINDOWS_H
#  include <windows.h>
#endif

namespace boost_cryray
{

namespace detail
{

#ifndef BOOST_USE_WINDOWS_H

struct CRITICAL_SECTION
{
    struct critical_section_debug * DebugInfo;
    long LockCount;
    long RecursionCount;
    void * OwningThread;
    void * LockSemaphore;
#if defined(_WIN64)
    unsigned __int64 SpinCount;
#else
    unsigned long SpinCount;
#endif
};

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION *);
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION *);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION *);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION *);

#endif // #ifndef BOOST_USE_WINDOWS_H

class lightweight_mutex
{
private:

    CRITICAL_SECTION cs_;

    lightweight_mutex(lightweight_mutex const &);
    lightweight_mutex & operator=(lightweight_mutex const &);

public:

    lightweight_mutex()
    {
        InitializeCriticalSection(&cs_);
    }

    ~lightweight_mutex()
    {
        DeleteCriticalSection(&cs_);
    }

    class scoped_lock;
    friend class scoped_lock;

    class scoped_lock
    {
    private:

        lightweight_mutex & m_;

        scoped_lock(scoped_lock const &);
        scoped_lock & operator=(scoped_lock const &);

    public:

        explicit scoped_lock(lightweight_mutex & m): m_(m)
        {
            EnterCriticalSection(&m_.cs_);
        }

        ~scoped_lock()
        {
            LeaveCriticalSection(&m_.cs_);
        }
    };
};

} // namespace detail

} // namespace boost_cryray

#endif // #ifndef BOOST_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
