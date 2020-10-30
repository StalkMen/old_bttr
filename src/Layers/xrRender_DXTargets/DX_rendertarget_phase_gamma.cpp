#include "stdafx.h"
#include "../../xrEngine/igame_persistent.h"
#include "../../xrEngine/environment.h"
#include "../../xrEngine/Rain.h"
#include "../xrRender/dxEnvironmentRender.h"

void CRenderTarget::phase_gamma()
{
	//Set backbuffer RT
	ref_rt& dest_rt = RImplementation.o.dx10_msaa ? rt_Generic : rt_Color;
	
	//Set parameters
	RCache.set_c("gamma", render_gamma, 0, 0, 0);
	
	//Perform screen space rendering
	RenderScreenQuad(dest_rt, s_gamma->E[0]);
	
	//Resolve RT
	HW.pRenderContext->CopyResource(rt_Generic_0->pTexture->surface_get(), dest_rt->pTexture->surface_get());
};

