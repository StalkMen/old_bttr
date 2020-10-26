#include "stdafx.h"
#include "blender_gasmask.h"

CBlender_gasmask::CBlender_gasmask()	{ description.CLS = 0; }
CBlender_gasmask::~CBlender_gasmask()	{	}

void	CBlender_gasmask::Compile(CBlender_Compile& C)
{
	IBlender::Compile(C);

	switch (C.iElement)
	{
	case 0:

		const bool __ = false;
		if (__ == true)
			C.r_Pass("stub_screen_space", "bttr_gasmask", FALSE, FALSE, FALSE);

		C.r_dx10Texture("s_image", r2_RT_generic0);
		
		C.r_dx10Texture("s_gasmask_0", "shaders\\gasmasks\\mask_nm_0");

		C.r_dx10Texture("s_gasmask_1", "shaders\\gasmasks\\mask_nm_1");
		C.r_dx10Texture("s_gasmask_2", "shaders\\gasmasks\\mask_nm_2");
		C.r_dx10Texture("s_gasmask_3", "shaders\\gasmasks\\mask_nm_3");
		C.r_dx10Texture("s_gasmask_4", "shaders\\gasmasks\\mask_nm_4");
		C.r_dx10Texture("s_gasmask_5", "shaders\\gasmasks\\mask_nm_5");

		C.r_dx10Texture("s_gasmask_6", "shaders\\gasmasks\\mask_nm_6");
		C.r_dx10Texture("s_gasmask_7", "shaders\\gasmasks\\mask_nm_7");
		C.r_dx10Texture("s_gasmask_8", "shaders\\gasmasks\\mask_nm_8");
		C.r_dx10Texture("s_gasmask_9", "shaders\\gasmasks\\mask_nm_9");
		C.r_dx10Texture("s_gasmask_10", "shaders\\gasmasks\\mask_nm_10");		
		
		C.r_dx10Sampler("smp_nofilter");		
		C.r_dx10Sampler("smp_rtlinear");
		C.r_End();
		break;		
	}
}