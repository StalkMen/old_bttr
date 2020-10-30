#include "stdafx.h"

void CRenderTarget::phase_dlaa()
{
	//Set backbuffer RT
	ref_rt& dest_rt = RImplementation.o.dx10_msaa ? rt_Generic : rt_Color;

	//Perform screen space rendering
	RenderScreenQuad(dest_rt, s_dlaa->E[0]);
	
	//Resolve RT
	HW.pRenderContext->CopyResource(rt_Generic_0->pTexture->surface_get(), dest_rt->pTexture->surface_get());
};
