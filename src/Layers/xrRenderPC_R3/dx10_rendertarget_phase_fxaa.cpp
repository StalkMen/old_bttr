#include "stdafx.h"

void CRenderTarget::phase_fxaa()
{
	//Constants
	u32 Offset = 0;
	u32 C = color_rgba(0, 0, 0, 255);

    float d_Z = EPS_S;
	float d_W = 1.0f;
	const float w = float(Device.dwWidth);
	const float h = float(Device.dwHeight);		
	
	//////////////////////////////////////////////////////////////////////////
	//Set MSAA/NonMSAA rendertarget
	u_setrt(rt_Generic_0, nullptr, nullptr, nullptr); //No ZB

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	// Half-pixel offset (DX9 only)
	constexpr Fvector2 p0{ 0.0f, 0.0f }, p1{ 1.0f, 1.0f };

	//Fill vertex buffer
	FVF::TL* pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0, float(h), d_Z, d_W, C, p0.x, p1.y); pv++;
	pv->set(0, 0, d_Z, d_W, C, p0.x, p0.y); pv++;
	pv->set(float(w), float(h), d_Z, d_W, C, p1.x, p1.y); pv++;
	pv->set(float(w), 0, d_Z, d_W, C, p1.x, p0.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//Set pass
	RCache.set_Element(s_fxaa->E[0]);

	//Set paramterers
	//RCache.set_c("taa_params", ps_taa_params.x, ps_taa_params.y, 0, 0);

	//Set geometry
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
	////////////////////////////////////////////////////////////////////////////
	ref_rt& dest_rt = RImplementation.o.dx10_msaa ? rt_Generic : rt_Color;
	u_setrt(dest_rt, nullptr, nullptr, HW.pBaseZB);

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	// Half-pixel offset (DX9 only)
	constexpr Fvector2 p0_1{ 0.0f, 0.0f }, p1_1{ 1.0f, 1.0f };

	//Fill vertex buffer
	pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0, float(h), d_Z, d_W, C, p0_1.x, p1_1.y); pv++;
	pv->set(0, 0, d_Z, d_W, C, p0_1.x, p0_1.y); pv++;
	pv->set(float(w), float(h), d_Z, d_W, C, p1_1.x, p1_1.y); pv++;
	pv->set(float(w), 0, d_Z, d_W, C, p1_1.x, p0_1.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//Set pass
	RCache.set_Element(s_fxaa->E[1]);

	//Set geometry
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
	
	//Resolve RT
	HW.pContext->CopyResource(rt_Generic_0->pTexture->surface_get(), dest_rt->pTexture->surface_get());			
}

#pragma TODO("OldSerpskiStalker. Нижне находится вариант на базе С++17")
#if 0
void CRenderTarget::phase_fxaa()
{
	//Constants
	u32 Offset = 0;
	constexpr float d_Z = EPS_S;
	constexpr float d_W = 1.0f;
	const float w = float(Device.dwWidth);
	const float h = float(Device.dwHeight);
	constexpr u32 C = color_rgba(0, 0, 0, 255);

	//////////////////////////////////////////////////////////////////////////
	//Set MSAA/NonMSAA rendertarget
	u_setrt(rt_Generic_0, nullptr, nullptr, nullptr); //No ZB

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	// Half-pixel offset (DX9 only)
	constexpr Fvector2 p0{ 0.0f, 0.0f }, p1{ 1.0f, 1.0f };

	//Fill vertex buffer
	FVF::TL* pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0, float(h), d_Z, d_W, C, p0.x, p1.y); pv++;
	pv->set(0, 0, d_Z, d_W, C, p0.x, p0.y); pv++;
	pv->set(float(w), float(h), d_Z, d_W, C, p1.x, p1.y); pv++;
	pv->set(float(w), 0, d_Z, d_W, C, p1.x, p0.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//Set pass
	RCache.set_Element(s_fxaa->E[0]);

	//Set paramterers
	//RCache.set_c("taa_params", ps_taa_params.x, ps_taa_params.y, 0, 0);

	//Set geometry
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
	////////////////////////////////////////////////////////////////////////////
	ref_rt& dest_rt = RImplementation.o.dx10_msaa ? rt_Generic : rt_Color;
	u_setrt(dest_rt, nullptr, nullptr, HW.pBaseZB);

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	// Half-pixel offset (DX9 only)
	constexpr Fvector2 p0{ 0.0f, 0.0f }, p1{ 1.0f, 1.0f };

	//Fill vertex buffer
	pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0, float(h), d_Z, d_W, C, p0.x, p1.y); pv++;
	pv->set(0, 0, d_Z, d_W, C, p0.x, p0.y); pv++;
	pv->set(float(w), float(h), d_Z, d_W, C, p1.x, p1.y); pv++;
	pv->set(float(w), 0, d_Z, d_W, C, p1.x, p0.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//Set pass
	RCache.set_Element(s_fxaa->E[1]);

	//Set geometry
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);

	//Resolve RT
	HW.pContext->CopyResource(rt_Generic_0->pTexture->surface_get(), dest_rt->pTexture->surface_get());
}
#endif