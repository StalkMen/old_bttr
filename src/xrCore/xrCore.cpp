// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"

#pragma comment(lib,"winmm.lib")

#ifdef DEBUG
# include <malloc.h>
#endif // DEBUG

XRCORE_API xrCore Core;
XRCORE_API u32 build_id;
XRCORE_API LPCSTR build_date;

namespace CPU
{
extern void Detect();
};

static u32 init_counter = 0;

void xrCore::_initialize(LPCSTR _ApplicationName, LogCallback cb, BOOL init_fs, LPCSTR fs_fname)
{
    xr_strcpy(ApplicationName, _ApplicationName);
    if (0 == init_counter)
    {
        // Init COM so we can use CoCreateInstance
        // HRESULT co_res =
        Params = xr_strdup(GetCommandLine());
        xr_strlwr(Params);
        if (!strstr(Params, "-editor"))
            CoInitializeEx(NULL, COINIT_MULTITHREADED);

        string_path fn, dr, di;

        // application path
        GetModuleFileName(GetModuleHandle(MODULE_NAME), fn, sizeof(fn));
        _splitpath(fn, dr, di, 0, 0);
        strconcat(sizeof(ApplicationPath), ApplicationPath, dr, di);

        GetCurrentDirectory(sizeof(WorkingPath), WorkingPath);

        // User/Comp Name
        DWORD sz_user = sizeof(UserName);
        GetUserName(UserName, &sz_user);

        DWORD sz_comp = sizeof(CompName);
        GetComputerName(CompName, &sz_comp);

        // Mathematics & PSI detection
        CPU::Detect();

        Memory._initialize(strstr(Params, "-mem_debug") ? TRUE : FALSE);

        DUMP_PHASE;

        /*
            По сути это не рекомендуемый Microsoft, но повсеместно используемый способ повышения точности
            соблюдения и измерения временных интревалов функциями Sleep, QueryPerformanceCounter,
            timeGetTime и GetTickCount.
            Функция действует на всю операционную систему в целом (!) и нет необходимости вызывать её при
            старте нового потока. Вызов timeEndPeriod специалисты Microsoft считают обязательным.
            Есть подозрения, что Windows сама устанавливает максимальную точность при старте таких
            приложений как, например, игры. Тогда есть шанс, что вызов timeBeginPeriod здесь бессмысленен.
            Недостатком данного способа является то, что он приводит к общему замедлению работы как
            текущего приложения, так и всей операционной системы.
            Ещё можно посмотреть ссылки:
            https://msdn.microsoft.com/en-us/library/vs/alm/dd757624(v=vs.85).aspx
            https://users.livejournal.com/-winnie/151099.html
            https://github.com/tebjan/TimerTool
        */
        timeBeginPeriod(1);

        InitLog();
        _initialize_cpu();

        // Debug._initialize ();

        rtc_initialize();

        xr_FS = xr_new<CLocatorAPI>();

        xr_EFS = xr_new<EFS_Utils>();
        //. R_ASSERT (co_res==S_OK);
    }
    if (init_fs)
    {
        u32 flags = 0;
        if (0 != strstr(Params, "-build")) flags |= CLocatorAPI::flBuildCopy;
        if (0 != strstr(Params, "-ebuild")) flags |= CLocatorAPI::flBuildCopy | CLocatorAPI::flEBuildCopy;
#ifdef DEBUG
        if (strstr(Params, "-cache")) flags |= CLocatorAPI::flCacheFiles;
        else flags &= ~CLocatorAPI::flCacheFiles;
#endif // DEBUG
        flags |= CLocatorAPI::flScanAppRoot;

#ifndef ELocatorAPIH
        if (0 != strstr(Params, "-file_activity")) flags |= CLocatorAPI::flDumpFileActivity;
#endif
        FS._initialize(flags, 0, fs_fname);
//        Msg("'%s' build %d, %s\n", "xrCore", build_id, build_date);
        Msg("[CryRay Engine (%s)] build date: [" __DATE__ " " __TIME__ "]", GetBuildConfiguration());
        EFS._initialize();
//#ifdef DEBUG
        Msg("# Process heap 0x%08x", GetProcessHeap());
//#endif // DEBUG
    }
    SetLogCB(cb);
    init_counter++;
}

#include "compression_ppmd_stream.h"
extern compression::ppmd::stream* trained_model;

void xrCore::_destroy()
{
    --init_counter;
    if (0 == init_counter)
    {
        FS._destroy();
        EFS._destroy();
        xr_delete(xr_FS);
        xr_delete(xr_EFS);

        if (trained_model)
        {
            void* buffer = trained_model->buffer();
            xr_free(buffer);
            xr_delete(trained_model);
        }

        xr_free(Params);
        Memory._destroy();
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        _clear87();
        _control87(_PC_53, MCW_PC);
        _control87(_RC_CHOP, MCW_RC);
        _control87(_RC_NEAR, MCW_RC);
        _control87(_MCW_EM, MCW_EM);
    }
        //. LogFile.reserve (256);
    break;
    case DLL_THREAD_ATTACH:
        if (!strstr(GetCommandLine(), "-editor"))
            CoInitializeEx(NULL, COINIT_MULTITHREADED);
        timeBeginPeriod(1);
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
#ifdef USE_MEMORY_MONITOR
        memory_monitor::flush_each_time(true);
#endif // USE_MEMORY_MONITOR
        break;
    }
    return TRUE;
}