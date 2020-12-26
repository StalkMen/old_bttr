#include "stdafx.h"
#pragma hdrstop

BLENDER::CBlender_accum_point::CBlender_accum_point	()	{	description.CLS		= 0;	}
BLENDER::CBlender_accum_point::~CBlender_accum_point	()	{	}

void	BLENDER::CBlender_accum_point::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	//BOOL	b_HW_smap		= RImplementation.o.HW_smap;
	//BOOL	b_HW_PCF		= RImplementation.o.HW_smap_PCF;
	BOOL		blend		= RImplementation.o.fp16_blend;
	D3DBLEND	dest		= blend?D3DBLEND_ONE:D3DBLEND_ZERO;
	switch (C.iElement)
	{
	case SE_L_FILL:			// fill projective
		
		C.r_Pass			("stub_notransform","copy_nomsaa",						false,	FALSE,	FALSE);
		
		C.r_dx10Texture		("s_base",			C.L_textures[0]);
		C.r_dx10Sampler		("smp_nofilter");
		C.r_End				();
		break;
	case SE_L_UNSHADOWED:	// unshadowed
		C.r_Pass			("accum_volume",	"accum_omni_unshadowed_nomsaa",	false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
		
		C.r_dx10Texture		("s_position",		r2_RT_P);
		C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
		C.r_dx10Texture		("s_material",		r2_material);
		C.r_dx10Texture		("s_lmap",			C.L_textures[0]);

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_material");
		C.r_dx10Sampler		("smp_rtlinear");
		C.r_End				();
		break;
	case SE_L_NORMAL:		// normal
		C.r_Pass			("accum_volume",	"accum_omni_normal_nomsaa",		false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
		
		C.r_dx10Texture		("s_position",		r2_RT_P);
		C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
		C.r_dx10Texture		("s_material",		r2_material);
		C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
		C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_material");
		C.r_dx10Sampler		("smp_rtlinear");
		jitter				(C);
		C.r_dx10Sampler		("smp_smap");
		C.r_End				();
		break;
	case SE_L_FULLSIZE:		// normal-fullsize
		C.r_Pass			("accum_volume",	"accum_omni_normal_nomsaa",		false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
		
		C.r_dx10Texture		("s_position",		r2_RT_P);
		C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
		C.r_dx10Texture		("s_material",		r2_material);
		C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
		C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_material");
		C.r_dx10Sampler		("smp_rtlinear");
		jitter				(C);
		C.r_dx10Sampler		("smp_smap");
		C.r_End				();
		break;
	case SE_L_TRANSLUENT:	// shadowed + transluency
		C.r_Pass			("accum_volume",	"accum_omni_transluent_nomsaa",	false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
		
		C.r_dx10Texture		("s_position",		r2_RT_P);
		C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
		C.r_dx10Texture		("s_material",		r2_material);
		C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
		C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

		C.r_dx10Sampler		("smp_nofilter");
		C.r_dx10Sampler		("smp_material");
		C.r_dx10Sampler		("smp_rtlinear");
		jitter				(C);
		C.r_dx10Sampler		("smp_smap");
		C.r_End				();
		break;
	}
}


BLENDER::CBlender_accum_point_msaa::CBlender_accum_point_msaa	()	{	description.CLS		= 0;	}
BLENDER::CBlender_accum_point_msaa::~CBlender_accum_point_msaa	()	{	}

//	TODO: DX10: Implement CBlender_accum_point::Compile
void	BLENDER::CBlender_accum_point_msaa::Compile(CBlender_Compile& C)
	{
	IBlender::Compile		(C);

   if( Name )
	   EnvCryRay.Render->m_MSAASample = atoi( Definition );
   else
	   EnvCryRay.Render->m_MSAASample = -1;

	//BOOL	b_HW_smap		= RImplementation.o.HW_smap;
	//BOOL	b_HW_PCF		= RImplementation.o.HW_smap_PCF;
	BOOL		blend		= RImplementation.o.fp16_blend;
	D3DBLEND	dest		= blend?D3DBLEND_ONE:D3DBLEND_ZERO;
	switch (C.iElement)
		{
		case SE_L_FILL:			// fill projective
			//C.r_Pass			("null", 			"copy",						false,	FALSE,	FALSE);
			C.r_Pass			("stub_notransform","copy_msaa",						false,	FALSE,	FALSE);
			//C.r_Sampler			("s_base",			C.L_textures[0]	);
			C.r_dx10Texture		("s_base",			C.L_textures[0]);
			C.r_dx10Sampler		("smp_nofilter");
			C.r_End				();
			break;
		case SE_L_UNSHADOWED:	// unshadowed
			C.r_Pass			("accum_volume",	"accum_omni_unshadowed_msaa",	false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
			
			C.r_dx10Texture		("s_position",		r2_RT_P);
			C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
			C.r_dx10Texture		("s_material",		r2_material);
			C.r_dx10Texture		("s_lmap",			C.L_textures[0]);

			C.r_dx10Sampler		("smp_nofilter");
			C.r_dx10Sampler		("smp_material");
			C.r_dx10Sampler		("smp_rtlinear");
			C.r_End				();
			break;
		case SE_L_NORMAL:		// normal
			C.r_Pass			("accum_volume",	"accum_omni_normal_msaa",		false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
			
			C.r_dx10Texture		("s_position",		r2_RT_P);
			C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);		
			C.r_dx10Texture		("s_material",		r2_material);
			C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
			C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

			C.r_dx10Sampler		("smp_nofilter");
			C.r_dx10Sampler		("smp_material");
			C.r_dx10Sampler		("smp_rtlinear");
			jitter				(C);
			C.r_dx10Sampler		("smp_smap");
			C.r_End				();
			break;
		case SE_L_FULLSIZE:		// normal-fullsize
			C.r_Pass			("accum_volume",	"accum_omni_normal_msaa",		false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
			
			C.r_dx10Texture		("s_position",		r2_RT_P);
			C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
			C.r_dx10Texture		("s_material",		r2_material);
			C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
			C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

			C.r_dx10Sampler		("smp_nofilter");
			C.r_dx10Sampler		("smp_material");
			C.r_dx10Sampler		("smp_rtlinear");
			jitter				(C);
			C.r_dx10Sampler		("smp_smap");
			C.r_End				();
			break;
		case SE_L_TRANSLUENT:	// shadowed + transluency
			C.r_Pass			("accum_volume",	"accum_omni_transluent_msaa",	false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
			
			C.r_dx10Texture		("s_position",		r2_RT_P);
			C.r_dx10Texture		("s_diffuse",		r2_RT_albedo);			
			C.r_dx10Texture		("s_material",		r2_material);
			C.r_dx10Texture		("s_lmap",			C.L_textures[0]);
			C.r_dx10Texture		("s_smap",			r2_RT_smap_depth);

			C.r_dx10Sampler		("smp_nofilter");
			C.r_dx10Sampler		("smp_material");
			C.r_dx10Sampler		("smp_rtlinear");
			jitter				(C);
			C.r_dx10Sampler		("smp_smap");
			C.r_End				();
			break;
		}
	EnvCryRay.Render->m_MSAASample = -1;
}

