#include "stdafx.h"

void	CRenderTarget::phase_smap_spot_clear()
{
	/*
	if (RImplementation.b_HW_smap)		u_setrt	(rt_smap_surf, NULL, NULL, rt_smap_d_depth->pRT);
	else								u_setrt	(rt_smap_surf, NULL, NULL, rt_smap_d_ZB);
	CHK_DX								(DEVICE_HW::CRYRAY_RENDER::HW.pDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER,	0xffffffff,	1.0f, 0L));
	*/
#ifdef DIRECTX11
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->ClearDepthStencilView( rt_smap_depth->pZRT, D3D_CLEAR_DEPTH, 1.0f, 0L);
#else
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice->ClearDepthStencilView( rt_smap_depth->pZRT, D3D10_CLEAR_DEPTH, 1.0f, 0L);
#endif
}

void	CRenderTarget::phase_smap_spot		(light* L)
{
	// Targets + viewport
	//	TODO: DX10: CHeck if we don't need old-style SMAP
	if (RImplementation.o.HW_smap)		u_setrt	(rt_smap_surf, NULL, NULL, rt_smap_depth->pZRT);
	//else								u_setrt	(rt_smap_surf, NULL, NULL, rt_smap_ZB);
	else								VERIFY(!"Use HW SMap only for DX10!");
#ifdef DIRECTX11
	D3D_VIEWPORT VP					=	{(float)L->X.S.posX, (float)L->X.S.posY, (float)L->X.S.size, (float)L->X.S.size, 0, 1};
	//CHK_DX								(DEVICE_HW::CRYRAY_RENDER::HW.pDevice->SetViewport(&VP));
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->RSSetViewports(1, &VP);
#else
	D3D_VIEWPORT VP					=	{L->X.S.posX,L->X.S.posY,L->X.S.size,L->X.S.size,0,1 };
	//CHK_DX								(DEVICE_HW::CRYRAY_RENDER::HW.pDevice->SetViewport(&VP));
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice->RSSetViewports(1, &VP);
#endif
	// Misc		- draw only front-faces //back-faces
	RCache.set_CullMode					( CULL_CCW	);
	RCache.set_Stencil					( FALSE		);
	// no transparency
	#pragma todo("can optimize for multi-lights covering more than say 50%...")
	if (RImplementation.o.HW_smap)		RCache.set_ColorWriteEnable	(FALSE);
	//CHK_DX								(DEVICE_HW::CRYRAY_RENDER::HW.pDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER,	0xffffffff,	1.0f, 0L));
	//	Do it once per smap generation pass in phase_smap_spot_clear
#ifdef DIRECTX11
	//DEVICE_HW::CRYRAY_RENDER::HW.pContext->ClearDepthStencilView( rt_smap_depth->pZRT, D3D_CLEAR_DEPTH, 1.0f, 0L);
#else
	//DEVICE_HW::CRYRAY_RENDER::HW.pDevice->ClearDepthStencilView( rt_smap_depth->pZRT, D3D10_CLEAR_DEPTH, 1.0f, 0L);
#endif
}

void	CRenderTarget::phase_smap_spot_tsh	(light* L)
{
	VERIFY(!"Implement clear of the buffer for tsh!");
	VERIFY							(RImplementation.o.Tshadows);
	RCache.set_ColorWriteEnable		();
	if (IRender_Light::OMNIPART == L->flags.type)	{
		// omni-part
		//CHK_DX							(DEVICE_HW::CRYRAY_RENDER::HW.pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET,	0xffffffff,	1.0f, 0L));
		FLOAT ColorRGBA[4] = {1.0f, 1.0f, 1.0f, 1.0f};
#ifdef DIRECTX11
		DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
#else
		DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice->ClearRenderTargetView(RCache.get_RT(), ColorRGBA);
#endif
	} else {
		// real-spot
		// Select color-mask
		ref_shader		shader			= L->s_spot;
		if (!shader)	shader			= s_accum_spot;
		RCache.set_Element				(shader->E[ SE_L_FILL ]	);

		// Fill vertex buffer
		Fvector2						p0,p1;
		u32		Offset;
		u32		C						= color_rgba	(255,255,255,255);
		float	_w						= float(L->X.S.size);
		float	_h						= float(L->X.S.size);
		float	d_Z						= EPS_S;
		float	d_W						= 1.f;
		p0.set							(.5f/_w, .5f/_h);
		p1.set							((_w+.5f)/_w, (_h+.5f)/_h );

		FVF::TL* pv						= (FVF::TL*) RCache.Vertex.Lock	(4,g_combine->vb_stride,Offset);
		pv->set							(EPS,			float(_h+EPS),	d_Z,	d_W, C, p0.x, p1.y);	pv++;
		pv->set							(EPS,			EPS,			d_Z,	d_W, C, p0.x, p0.y);	pv++;
		pv->set							(float(_w+EPS),	float(_h+EPS),	d_Z,	d_W, C, p1.x, p1.y);	pv++;
		pv->set							(float(_w+EPS),	EPS,			d_Z,	d_W, C, p1.x, p0.y);	pv++;
		RCache.Vertex.Unlock			(4,g_combine->vb_stride);
		RCache.set_Geometry				(g_combine);

		// draw
		RCache.Render					(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}
