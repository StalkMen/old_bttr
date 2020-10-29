#include "stdafx.h"
#pragma hdrstop
#include "Blender_gamma.h"

CBlender_gamma::CBlender_gamma() { description.CLS = 0; }
CBlender_gamma::~CBlender_gamma() { }

void CBlender_gamma::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	C.r_Pass("stub_screen_space", "render_gamma", FALSE, FALSE, FALSE);
	C.r_dx10Texture("s_image", r2_RT_generic0);
	
	C.r_dx10Texture("s_gamma", "shaders\\gamma");
	
	C.r_dx10Sampler("smp_rtlinear");
	C.r_End();
}
