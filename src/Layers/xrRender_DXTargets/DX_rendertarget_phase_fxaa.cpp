#include "stdafx.h"

void CRenderTarget::phase_fxaa()
{
	u32 Offset;
	Fvector2 p0, p1;
	float d_Z = EPS_S;
	float d_W = 1.0f;
	u32	C = color_rgba(0, 0, 0, 255);

	float _w = float(Device.dwWidth);
	float _h = float(Device.dwHeight);

	p0.set(0.0f, 0.0f);
	p1.set(1.0f, 1.0f);

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	ref_rt& dest_rt = RImplementation.o.dx10_msaa ? rt_Generic : rt_Color;
	u_setrt(dest_rt, nullptr, nullptr, DEVICE_HW::XRAY::HW.pBaseZB);

	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	//Fill vertex buffer
	FVF::TL* pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0, float(_h), d_Z, d_W, C, p0.x, p1.y); pv++;
	pv->set(0, 0, d_Z, d_W, C, p0.x, p0.y); pv++;
	pv->set(float(_w), float(_h), d_Z, d_W, C, p1.x, p1.y); pv++;
	pv->set(float(_w), 0, d_Z, d_W, C, p1.x, p0.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//Set pass
	RCache.set_Element(s_fxaa->E[0]);

	//Set geometry
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);

	//Resolve RT
	DEVICE_HW::XRAY::HW.pRenderContext->CopyResource(rt_Generic_0->pTexture->surface_get(), dest_rt->pTexture->surface_get());
}