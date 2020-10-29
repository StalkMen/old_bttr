#include "stdafx.h"

void CRenderTarget::phase_dlaa()
{
	RenderScreenQuad(rt_Generic, s_dlaa->E[0], float(1.0));
	
	HW.pRenderContext->CopyResource(rt_Generic_0->pTexture->surface_get(), rt_Generic->pTexture->surface_get());
};
