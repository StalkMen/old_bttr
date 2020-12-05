#include "stdafx.h"
#include "dxRenderDeviceRender.h"

#include "ResourceManager.h"

dxRenderDeviceRender::dxRenderDeviceRender()
    :	Resources(0)
{
    ;
}

void dxRenderDeviceRender::Copy(IRenderDeviceRender &_in)
{
    *this = *(dxRenderDeviceRender*)&_in;
}

void dxRenderDeviceRender::setGamma(float fGamma)
{
    m_Gamma.Gamma(fGamma);
}

void dxRenderDeviceRender::setBrightness(float fGamma)
{
    m_Gamma.Brightness(fGamma);
}

void dxRenderDeviceRender::setContrast(float fGamma)
{
    m_Gamma.Contrast(fGamma);
}

void dxRenderDeviceRender::updateGamma()
{
    m_Gamma.Update();
}

void dxRenderDeviceRender::OnDeviceDestroy( BOOL bKeepTextures)
{
    m_WireShader.destroy();
    m_SelectionShader.destroy();

    Resources->OnDeviceDestroy( bKeepTextures);
    RCache.OnDeviceDestroy();
}

void dxRenderDeviceRender::ValidateHW()
{
    DEVICE_HW::CRYRAY_RENDER::HW.Validate();
}

void dxRenderDeviceRender::DestroyHW()
{
    xr_delete					(Resources);
    DEVICE_HW::CRYRAY_RENDER::HW.DestroyDevice			();
}

void  dxRenderDeviceRender::Reset( HWND hWnd, u32 &dwWidth, u32 &dwHeight, float &fWidth_2, float &fHeight_2)
{
#ifdef DEBUG
    _SHOW_REF("*ref -CRenderDevice::ResetTotal: DeviceREF:",DEVICE_HW::CRYRAY_RENDER::HW.pDevice);
#endif // DEBUG	

    Resources->reset_begin	();
    Memory.mem_compact		();
    DEVICE_HW::CRYRAY_RENDER::HW.Reset				(hWnd);

    dwWidth					= DEVICE_HW::CRYRAY_RENDER::HW.m_ChainDesc.BufferDesc.Width;
    dwHeight				= DEVICE_HW::CRYRAY_RENDER::HW.m_ChainDesc.BufferDesc.Height;

    fWidth_2				= float(dwWidth/2);
    fHeight_2				= float(dwHeight/2);
    Resources->reset_end	();

#ifdef DEBUG
    _SHOW_REF("*ref +CRenderDevice::ResetTotal: DeviceREF:",DEVICE_HW::CRYRAY_RENDER::HW.pDevice);
#endif // DEBUG
}

void dxRenderDeviceRender::SetupStates()
{
    DEVICE_HW::CRYRAY_RENDER::HW.Caps.Update				();
	SSManager.SetMaxAnisotropy	(ps_r__tf_Anisotropic);
    SSManager.SetMipLODBias		(ps_r__tf_Mipbias);
}

void dxRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
    // Signal everyone - device created
    RCache.OnDeviceCreate		();
    m_Gamma.Update				();
    Resources->OnDeviceCreate	(shName);
    EnvCryRay.Render->create			();
    Device.Statistic->OnDeviceCreate	();

    if (!g_dedicated_server)
    {
        m_WireShader.create			("editor\\wire");
        m_SelectionShader.create	("editor\\selection");

        DUImpl.OnDeviceCreate			();
    }
}

void dxRenderDeviceRender::Create( HWND hWnd, u32 &dwWidth, u32 &dwHeight, float &fWidth_2, float &fHeight_2, bool move_window)
{
    DEVICE_HW::CRYRAY_RENDER::HW.CreateDevice		(hWnd, move_window);
    dwWidth				= DEVICE_HW::CRYRAY_RENDER::HW.m_ChainDesc.BufferDesc.Width;
    dwHeight			= DEVICE_HW::CRYRAY_RENDER::HW.m_ChainDesc.BufferDesc.Height;
    fWidth_2			= float(dwWidth/2)			;
    fHeight_2			= float(dwHeight/2)			;
    Resources			= xr_new<CResourceManager>		();
}

void dxRenderDeviceRender::SetupGPU( BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF)
{
    DEVICE_HW::CRYRAY_RENDER::HW.Caps.bForceGPU_SW		= bForceGPU_SW;
    DEVICE_HW::CRYRAY_RENDER::HW.Caps.bForceGPU_NonPure	= bForceGPU_NonPure;
    DEVICE_HW::CRYRAY_RENDER::HW.Caps.bForceGPU_REF		= bForceGPU_REF;
}

void dxRenderDeviceRender::overdrawBegin()
{
    //	TODO: DX10: Implement overdrawBegin
    VERIFY(!"dxRenderDeviceRender::overdrawBegin not implemented.");
}

void dxRenderDeviceRender::overdrawEnd()
{
    //	TODO: DX10: Implement overdrawEnd
    VERIFY(!"dxRenderDeviceRender::overdrawBegin not implemented.");
}

void dxRenderDeviceRender::DeferredLoad(BOOL E)
{
    Resources->DeferredLoad(E);
}

void dxRenderDeviceRender::ResourcesDeferredUpload()
{
    Resources->DeferredUpload();
}

void dxRenderDeviceRender::ResourcesGetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps)
{
    if (Resources)
        Resources->_GetMemoryUsage(m_base, c_base, m_lmaps, c_lmaps);
}

void dxRenderDeviceRender::ResourcesStoreNecessaryTextures()
{
    dxRenderDeviceRender::Instance().Resources->StoreNecessaryTextures();
}

void dxRenderDeviceRender::ResourcesDumpMemoryUsage()
{
    dxRenderDeviceRender::Instance().Resources->_DumpMemoryUsage();
}

dxRenderDeviceRender::DeviceState dxRenderDeviceRender::GetDeviceState()
{
    DEVICE_HW::CRYRAY_RENDER::HW.Validate		();

    const auto result = DEVICE_HW::CRYRAY_RENDER::HW.m_pSwapChain->Present(0, DXGI_PRESENT_TEST);

    switch (result)
    {
        // Check if the device is ready to be reset
        case DXGI_ERROR_DEVICE_RESET:
            return dsNeedReset;
    }

    return dsOK;
}

BOOL dxRenderDeviceRender::GetForceGPU_REF()
{
    return DEVICE_HW::CRYRAY_RENDER::HW.Caps.bForceGPU_REF;
}

u32 dxRenderDeviceRender::GetCacheStatPolys()
{
    return RCache.stat.polys;
}

void dxRenderDeviceRender::Begin()
{
    RCache.OnFrameBegin		();
    RCache.set_CullMode		(CULL_CW);
    RCache.set_CullMode		(CULL_CCW);
    if (DEVICE_HW::CRYRAY_RENDER::HW.Caps.SceneMode)	overdrawBegin	();
}

void dxRenderDeviceRender::Clear()
{	//Cleans stencil-depth buffer
    
	//LV: We don't use depth buffer on R3/R4 to get depth, so there's no need to clean it
	//DEVICE_HW::CRYRAY_RENDER::HW.pContext->ClearDepthStencilView(RCache.get_ZB(), 
    //    D3D_CLEAR_DEPTH|D3D_CLEAR_STENCIL, 1.0f, 0);

    if (psDeviceFlags.test(rsClearBB))
    {
        FLOAT ColorRGBA[4] = {0.0f,0.0f,0.0f,0.0f};
        DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
    }
}

void DoAsyncScreenshot();

void dxRenderDeviceRender::End()
{
    VERIFY	(DEVICE_HW::CRYRAY_RENDER::HW.pDevice);

    if (DEVICE_HW::CRYRAY_RENDER::HW.Caps.SceneMode)	overdrawEnd();

    RCache.OnFrameEnd	();
    Memory.dbg_check		();

    DoAsyncScreenshot();
    if (!Device.m_SecondViewport.IsSVPFrame() && !Device.m_SecondViewport.isCamReady) //--#SM+#-- +SecondVP+
    {
        DEVICE_HW::CRYRAY_RENDER::HW.m_pSwapChain->Present(!!psDeviceFlags.test(rsVSync), 0);
    }
}

void dxRenderDeviceRender::ResourcesDestroyNecessaryTextures()
{
    Resources->DestroyNecessaryTextures();
}

void dxRenderDeviceRender::ClearTarget()
{	//Cleans backbuffer
    FLOAT ColorRGBA[4] = {0.0f,0.0f,0.0f,0.0f};
    DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
}

void dxRenderDeviceRender::SetCacheXform(Fmatrix &mView, Fmatrix &mProject)
{
    RCache.set_xform_view(mView);
    RCache.set_xform_project(mProject);
}

bool dxRenderDeviceRender::HWSupportsShaderYUV2RGB()
{
    u32		v_dev	= CAP_VERSION(DEVICE_HW::CRYRAY_RENDER::HW.Caps.raster_major, DEVICE_HW::CRYRAY_RENDER::HW.Caps.raster_minor);
    u32		v_need	= CAP_VERSION(2,0);
    return (v_dev>=v_need);
}

void  dxRenderDeviceRender::OnAssetsChanged()
{
    Resources->m_textures_description.UnLoad();
    Resources->m_textures_description.Load();
}
