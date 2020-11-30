#include "stdafx.h"
#pragma hdrstop

BLENDER::GAME::CBlender_rain_drops::CBlender_rain_drops() { description.CLS = 0; }
BLENDER::GAME::CBlender_rain_drops::~CBlender_rain_drops() { }

void BLENDER::GAME::CBlender_rain_drops::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	C.r_Pass("stub_screen_space", "rain_drops_shader", FALSE, FALSE, FALSE);
	C.r_dx10Texture("s_image", r2_RT_generic0);
	C.r_dx10Sampler("smp_rtlinear");
	C.r_End();
}
