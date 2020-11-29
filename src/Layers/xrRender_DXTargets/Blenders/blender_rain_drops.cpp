#include "stdafx.h"
#pragma hdrstop

BLENDER::GAME::CBlender_rain_drops::CBlender_rain_drops() { description.CLS = 0; }
BLENDER::GAME::CBlender_rain_drops::~CBlender_rain_drops() { }

void BLENDER::GAME::CBlender_rain_drops::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	C.r_Pass("stub_notransform_aa_AA", "rain_drops_shader", FALSE, FALSE, FALSE);
	C.r_dx10Texture("s_image", r2_RT_generic0);
	C.r_dx10Texture("s_rain_drops0", "shaders\\rain_drops");
	C.r_dx10Sampler("smp_rtlinear");
	C.r_End();
}
