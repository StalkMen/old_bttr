// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#pragma warning(disable:4995)
#include "../../xrEngine/stdafx.h"
#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(default:4995)
#pragma warning(disable:4714)
#pragma warning( 4 : 4018 )
#pragma warning( 4 : 4244 )
#pragma warning(disable:4237)

#include <D3D11.h>
#include <D3Dx11core.h>
#include <D3DCompiler.h>

#include "../xrRender/xrD3DDefs.h"

#include "../xrRender/Debug/dxPixEventWrapper.h"

#define		R_R3	3
#define		R_R4	4
#define		RENDER	R_R4

#include "../../xrParticles/psystem.h"

#include "../xrRender/HW.h"
#include "../xrRender/Shader.h"
#include "../xrRender/R_Backend.h"
#include "../xrRender/R_Backend_Runtime.h"

#include "../xrRender/resourcemanager.h"

#include "../../xrEngine/vis_common.h"
#include "../../xrEngine/render.h"
#include "../../xrEngine/_d3d_extensions.h"
#include "../../xrEngine/igame_level.h"
#include "../xrRender/blenders\blender.h"
#include "../xrRender/blenders\blender_clsid.h"
#include "../xrRender/xrRender_console.h"
#include "..\xrRender_DXTargets\DX.h"
#include "../../build_render_config.h"
#include "../../xrEngine/ExternalData.h"

#include "../../Layers/xrRender_DXTargets/Blenders/blenders_namespaces.h"

/*
#ifdef RENDER_DX11_BUILD
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif
*/

IC	void	jitter(CBlender_Compile& C)
{
	C.r_dx10Texture	("jitter0",	JITTER(0));
	C.r_dx10Texture	("jitter1",	JITTER(1));
	C.r_dx10Texture	("jitter2",	JITTER(2));
	C.r_dx10Texture	("jitter3",	JITTER(3));
	C.r_dx10Texture	("jitter4",	JITTER(4));
	C.r_dx10Texture	("jitterMipped",	r2_jitter_mipped);
	C.r_dx10Sampler	("smp_jitter");
}

IC	void	samplers_pack(CBlender_Compile& C)
{
	C.r_dx10Sampler("smp_nofilter"); 	//D3DTADDRESS_CLAMP		D3DTEXF_POINT			D3DTEXF_NONE		D3DTEXF_POINT 
	C.r_dx10Sampler("smp_rtlinear");	//D3DTADDRESS_WRAP		D3DTEXF_LINEAR			D3DTEXF_LINEAR		D3DTEXF_LINEAR
	C.r_dx10Sampler("smp_base");		//D3DTADDRESS_WRAP		D3DTEXF_ANISOTROPIC		D3DTEXF_LINEAR		D3DTEXF_ANISOTROPIC
	C.r_dx10Sampler("smp_linear");		//D3DTADDRESS_CLAMP		D3DTEXF_LINEAR			D3DTEXF_NONE		D3DTEXF_LINEAR 
}