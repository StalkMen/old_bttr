#include "stdafx.h"

BLENDER::CBlender_motion_vector::CBlender_motion_vector()	{ description.CLS = 0; }
BLENDER::CBlender_motion_vector::~CBlender_motion_vector()	{	}

void BLENDER::CBlender_motion_vector::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	C.r_Pass("stub_screen_space", "motion_vector", FALSE, FALSE, FALSE);
	C.r_dx10Texture("s_position", r2_RT_P);
	
	C.r_dx10Sampler("smp_nofilter");		
	C.r_dx10Sampler("smp_rtlinear");
	C.r_End();
}