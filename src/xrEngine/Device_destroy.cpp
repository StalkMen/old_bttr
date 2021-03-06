#include "stdafx.h"

#include "../Include/xrRender/DrawUtils.h"
#include "render.h"
#include "IGame_Persistent.h"
#include "xr_IOConsole.h"
#include "Discord.h"

void CRenderDevice::_Destroy(BOOL bKeepTextures)
{
    EnvCryRay.DU->OnDeviceDestroy();

    // before destroy
    b_is_Ready = FALSE;
    Statistic->OnDeviceDestroy();
    EnvCryRay.Render->destroy();
    m_pRender->OnDeviceDestroy(bKeepTextures);
    //Resources->OnDeviceDestroy (bKeepTextures);
    //RCache.OnDeviceDestroy ();

    Memory.mem_compact();
}

void CRenderDevice::Destroy(void)
{
    if (!b_is_Ready) 
		return;
	
	if (renderer_value == 0 || renderer_value == 1)
		Log("# Destroying Direct3D10");
	else if (renderer_value == 2 || renderer_value == 3)
		Log("# Destroying Direct3D11");
	
    ShowCursor(TRUE);
    ClipCursor(NULL);
    m_pRender->ValidateHW();

    _Destroy(FALSE);

    // real destroy
    m_pRender->DestroyHW();

    //xr_delete (Resources);
    //HW.DestroyDevice ();

    ChangeDisplaySettings(NULL, 0);

    seqRender.R.clear();
    seqAppActivate.R.clear();
    seqAppDeactivate.R.clear();
    seqAppStart.R.clear();
    seqAppEnd.R.clear();
    seqFrame.R.clear();
    seqFrameMT.R.clear();
    seqDeviceReset.R.clear();
    seqParallel.clear();

    EnvCryRay.RenderFactory->DestroyRenderDeviceRender(m_pRender);
    m_pRender = 0;
    xr_delete(Statistic);
}

#include "IGame_Level.h"
#include "CustomHUD.h"
extern BOOL bNeed_re_create_env;
extern u32 g_screenmode;
extern void GetMonitorResolution(u32& horizontal, u32& vertical);
void CRenderDevice::Reset(bool precache)
{
    u32 dwWidth_before = dwWidth;
    u32 dwHeight_before = dwHeight;

    ShowCursor(TRUE);
    u32 tm_start = TimerAsync();
    
    m_pRender->Reset(m_hWnd, dwWidth, dwHeight, fWidth_2, fHeight_2);

    if (g_pGamePersistent)
    {
        //. g_pGamePersistent->Environment().OnDeviceCreate();
        //bNeed_re_create_env = TRUE;
        g_pGamePersistent->Environment().bNeed_re_create_env = TRUE;
    }
    _SetupStates();
    if (precache)
        PreCache(20, true, false);
    u32 tm_end = TimerAsync();
    Msg("~ RESET [%d ms]", tm_end - tm_start);

    // TODO: Remove this! It may hide crash
    Memory.mem_compact();

    seqDeviceReset.Process(rp_DeviceReset);

    if (dwWidth_before != dwWidth || dwHeight_before != dwHeight)
    {
        seqResolutionChanged.Process(rp_ScreenResolutionChanged);
    }

    if (g_screenmode == 1)
    {
        u32 w, h;
        GetMonitorResolution(w, h);
        SetWindowLongPtr(Device.m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(Device.m_hWnd, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
    }

    ShowCursor(FALSE);
    RECT winRect;
    GetWindowRect(m_hWnd, &winRect);
    ClipCursor(&winRect);
}
