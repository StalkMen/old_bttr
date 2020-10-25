#include "stdafx.h"
#include "../../xrEngine/igame_persistent.h"
#include "../../xrEngine/environment.h"
#include "../../xrEngine/Rain.h"
#include "../xrRender/dxEnvironmentRender.h"

void CRenderTarget::phase_gamma()
{
	RCache.set_c("gamma", render_gamma, 0, 0, 0);
	RenderScreenQuad(rt_Generic, s_gamma->E[0], float(1.0));
	
	HW.pRenderContext->CopyResource(rt_Generic_0->pTexture->surface_get(), rt_Generic->pTexture->surface_get());
};

