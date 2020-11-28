#include "stdafx.h"

BLENDER::AA::CBlender_FXAA::CBlender_FXAA()	{ description.CLS = 0; }
BLENDER::AA::CBlender_FXAA::~CBlender_FXAA()	{	}

void	BLENDER::AA::CBlender_FXAA::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	switch (C.iElement)
	{	
	case 0:
		C.r_Pass("stub_screen_space", "fxaa_main", FALSE, FALSE, FALSE);
		C.r_dx10Texture("s_image", r2_RT_generic0);
		C.r_dx10Sampler("smp_nofilter");		
		C.r_dx10Sampler("smp_rtlinear");
		C.r_End();
		break;
	}
}