#include "stdafx.h"
/*Code is correct.
	For screen space effects/blits
*/

void CRenderTarget::RenderScreenQuad(ref_rt& rt, ref_selement& sh)
{
	u32 Offset = 0;
	u32 C = color_rgba(0, 0, 0, 255);
	
	//Prepare resolution (with additional resizing)
	float width = float(Device.dwWidth);
	float height = float(Device.dwHeight);
	
	//set rendertarget 
	u_setrt(rt, NULL, NULL, DEVICE_HW::XRAY::HW.pBaseZB);
	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	//vertex buffer
	FVF::TL* pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0,		height,	EPS_S, 1.0f, C, 0, 1); 
	pv++;
	pv->set(0,		0,		EPS_S, 1.0f, C, 0, 0); 
	pv++;
	pv->set(width,	height,	EPS_S, 1.0f, C, 1, 1); 
	pv++;
	pv->set(width,	0,		EPS_S, 1.0f, C, 1, 0); 
	pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//draw
	RCache.set_Element(sh);
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
}