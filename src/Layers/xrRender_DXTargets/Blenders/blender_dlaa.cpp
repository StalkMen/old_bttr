#include "stdafx.h"

BLENDER::AA::CBlender_dlaa::CBlender_dlaa() { description.CLS = 0; }
BLENDER::AA::CBlender_dlaa::~CBlender_dlaa() { }

void BLENDER::AA::CBlender_dlaa::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

		C.r_Pass("stub_notransform_aa_AA", "dlaa_main", FALSE, FALSE, FALSE);

		C.r_dx10Texture		("s_image",					"$user$generic0");
		C.r_dx10Texture		("s_position",				"$user$position");
		C.r_dx10Texture		("s_distort",				"$user$generic1");

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_rtlinear");

	C.r_End();
} 