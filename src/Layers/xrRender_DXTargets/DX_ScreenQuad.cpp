#include "stdafx.h"
/*niech mnie chuj pierdoli po same pomidory... it's worth to check this code.
something seems to be wrong with it*/

void CRenderTarget::RenderScreenQuad(ref_rt& rt, ref_selement& sh, float res)
{
	Fvector2 p0, p1;
	u32 Offset = 0;
	u32 C = color_rgba(0, 0, 0, 255);
	
	//Prepare resolution (with additional resizing)
	float width = float(Device.dwWidth)*res;
	float height = float(Device.dwHeight)*res;
	
	//halfpixel for dx11
	p0.set(0.5f / width, 0.5f / height);
	p1.set((width + 0.5f) / width, (height + 0.5f) / height);

	//set rendertarget 
	u_setrt(rt, NULL, NULL, HW.pBaseZB);
	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);

	//vertex buffer
	FVF::TL* pv = (FVF::TL*)RCache.Vertex.Lock(4, g_combine->vb_stride, Offset);
	pv->set(0,		height,		EPS_S, 1.0f, C, p0.x, p1.y); pv++;
	pv->set(0,		0,			EPS_S, 1.0f, C, p0.x, p0.y); pv++;
	pv->set(width,	height,		EPS_S, 1.0f, C, p1.x, p1.y); pv++;
	pv->set(width,	0,			EPS_S, 1.0f, C, p1.x, p0.y); pv++;
	RCache.Vertex.Unlock(4, g_combine->vb_stride);

	//draw
	RCache.set_Element(sh);
	RCache.set_Geometry(g_combine);
	RCache.Render(D3DPT_TRIANGLELIST, Offset, 0, 4, 0, 2);
}