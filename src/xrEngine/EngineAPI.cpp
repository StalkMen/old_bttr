// EngineAPI.cpp: implementation of the CEngineAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineAPI.h"
#include "../xrcdb/xrXRC.h"

//#include "securom_api.h"

extern xr_token* vid_quality_token;
extern "C" { typedef bool _declspec(dllexport) SupportsDX10Rendering();  typedef bool _declspec(dllexport) SupportsDX11Rendering(); };
extern u32 renderer_value; //con cmd

LPCSTR dx10_name = "xrRender_DX10.dll";
LPCSTR dx11_name = "xrRender_DX11.dll";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __cdecl dummy(void)
{
};

CEngineAPI::CEngineAPI()
{
    hGame = 0;
    hRender = 0;
    hTuner = 0;
    pCreate = 0;
    pDestroy = 0;
    tune_pause = dummy;
    tune_resume = dummy;
}

CEngineAPI::~CEngineAPI()
{
    // destroy quality token here
    if (vid_quality_token)
    {
        xr_free(vid_quality_token);
        vid_quality_token = NULL;
    }
}

ENGINE_API bool is_enough_address_space_available()
{
    SYSTEM_INFO system_info;
        GetSystemInfo(&system_info);
        return (*(u32*)&system_info.lpMaximumApplicationAddress) > 0x90000000;
}

void CEngineAPI::InitializeNotDedicated()
{
    if (psDeviceFlags.test(rsR4))
    {
        // try to initialize DX11
        Log("# Loading DLL:", dx11_name);
        hRender = LoadLibrary(dx11_name);
        if (0 == hRender)
            R_ASSERT("! ...Failed - incompatible hardware/pre-Vista OS.");
    }

    if (psDeviceFlags.test(rsR3))
    {
        // try to initialize DX10
        Log("# Loading DLL:", dx10_name);
        hRender = LoadLibrary(dx10_name);
        if (0 == hRender)
            R_ASSERT("! ...Failed - incompatible hardware/pre-Vista OS.");
    }
}

void CEngineAPI::Initialize(void)
{
    //////////////////////////////////////////////////////////////////////////
    // render
    InitializeNotDedicated();

    if (0 == hRender)
    {
        psDeviceFlags.set(rsR4, FALSE);
        psDeviceFlags.set(rsR3, TRUE);

        renderer_value = 0; //con cmd

        Log("# Loading DLL:", dx10_name);
        hRender = LoadLibrary(dx10_name);
        if (0 == hRender) 
            R_CHK(GetLastError());
        R_ASSERT(hRender);
    }

    Device.ConnectToRender();

    // game
    {
        LPCSTR g_name = "xrGame.dll";
        Log("# Loading DLL:", g_name);
        hGame = LoadLibrary(g_name);
        if (0 == hGame) R_CHK(GetLastError());
        R_ASSERT2(hGame, "Game DLL raised exception during loading or there is no game DLL at all");
        pCreate = (Factory_Create*)GetProcAddress(hGame, "xrFactory_Create");
        R_ASSERT(pCreate);
        pDestroy = (Factory_Destroy*)GetProcAddress(hGame, "xrFactory_Destroy");
        R_ASSERT(pDestroy);
    }

    //////////////////////////////////////////////////////////////////////////
    // vTune
    tune_enabled = FALSE;
    if (strstr(Core.Params, "-tune"))
    {
        LPCSTR g_name = "vTuneAPI.dll";
        Log("# Loading DLL:", g_name);
        hTuner = LoadLibrary(g_name);
        if (0 == hTuner) R_CHK(GetLastError());
        R_ASSERT2(hTuner, "Intel vTune is not installed");
        tune_enabled = TRUE;
        tune_pause = (VTPause*)GetProcAddress(hTuner, "VTPause");
        R_ASSERT(tune_pause);
        tune_resume = (VTResume*)GetProcAddress(hTuner, "VTResume");
        R_ASSERT(tune_resume);
    }
}

void CEngineAPI::Destroy(void)
{
    if (hGame) { FreeLibrary(hGame); hGame = 0; }
    if (hRender) { FreeLibrary(hRender); hRender = 0; }
    pCreate = 0;
    pDestroy = 0;
    Engine.Event._destroy();
    XRC.r_clear_compact();
}

void CEngineAPI::CreateRendererList()
{
    // TODO: ask renderers if they are supported!
    if (vid_quality_token != NULL)
        return;

    bool bSupports_dx10 = false;
    bool bSupports_dx11 = false;

    // try to initialize DX10
    Log("# Loading DLL:", dx10_name);
    // Hide "d3d10.dll not found" message box for XP
    SetErrorMode(SEM_FAILCRITICALERRORS);
    hRender = LoadLibrary(dx10_name);
    // Restore error handling
    SetErrorMode(0);
    if (hRender)
    {
        SupportsDX10Rendering* DX10 = (SupportsDX10Rendering*)GetProcAddress(hRender, "SupportsDX10Rendering");
        R_ASSERT(DX10);
        bSupports_dx10 = DX10();
        FreeLibrary(hRender);
    }

    // try to initialize R4
    Log("# Loading DLL:", dx11_name);
    // Hide "d3d10.dll not found" message box for XP
    SetErrorMode(SEM_FAILCRITICALERRORS);
    hRender = LoadLibrary(dx11_name);
    // Restore error handling
    SetErrorMode(0);
    if (hRender)
    {
        SupportsDX11Rendering* DX11 = (SupportsDX11Rendering*)GetProcAddress(hRender, "SupportsDX11Rendering");
        R_ASSERT(DX11);
        bSupports_dx11 = DX11();
        FreeLibrary(hRender);
    }


    hRender = 0;
    bool proceed = true;
    xr_vector<LPCSTR> _tmp;

    if (proceed &= bSupports_dx10, proceed)
    {
        _tmp.push_back("support_DX10");
        _tmp.push_back("support_DX10_1");
    }
    if (proceed &= bSupports_dx11, proceed)
        _tmp.push_back("support_DX11");

    u32 _cnt = _tmp.size() + 1;
    vid_quality_token = xr_alloc<xr_token>(_cnt);

    vid_quality_token[_cnt - 1].id = -1;
    vid_quality_token[_cnt - 1].name = NULL;

    Msg("* Available render modes[%d]:", _tmp.size());
    for (u32 i = 0; i < _tmp.size(); ++i)
    {
        vid_quality_token[i].id = i;
        vid_quality_token[i].name = _tmp[i];
        Msg("[%s]", _tmp[i]);
    }
}