#include "stdafx.h"

#include "blender_dlaa.h"

CBlender_dlaa::CBlender_dlaa() { description.CLS = 0; }
CBlender_dlaa::~CBlender_dlaa() { }

void CBlender_dlaa::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	C.r_Pass("stub_screen_space", "dlaa_main", FALSE, FALSE, FALSE);

		C.r_dx10Texture		("s_image",					"$user$generic0");
		C.r_dx10Texture		("s_position",				"$user$position");
		C.r_dx10Texture		("s_distort",				"$user$generic1");

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_rtlinear");

	C.r_End();
} 