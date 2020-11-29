#include "stdafx.h"
#pragma hdrstop

BLENDER::CBlender_accum_reflected::CBlender_accum_reflected	()	{	description.CLS		= 0;	}
BLENDER::CBlender_accum_reflected::~CBlender_accum_reflected	()	{	}

void BLENDER::CBlender_accum_reflected::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	BOOL		blend		= RImplementation.o.fp16_blend;
	D3DBLEND	dest		= blend?D3DBLEND_ONE:D3DBLEND_ZERO;

	C.r_Pass			("accum_volume",	"accum_indirect_nomsaa",false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
	
	C.r_dx10Texture		("s_position",		r2_RT_P);
	C.r_dx10Texture		("s_normal",		r2_RT_N);
	C.r_dx10Texture		("s_material",		r2_material);
	C.r_dx10Texture		("s_accumulator",	r2_RT_accum		);

	C.r_dx10Sampler		("smp_nofilter");
	C.r_dx10Sampler		("smp_material");
	C.r_End				();
}

BLENDER::CBlender_accum_reflected_msaa::CBlender_accum_reflected_msaa	()	{	description.CLS		= 0;	}
BLENDER::CBlender_accum_reflected_msaa::~CBlender_accum_reflected_msaa	()	{	}

//	TODO: DX10: implement CBlender_accum_reflected::Compile
void BLENDER::CBlender_accum_reflected_msaa::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	BOOL		blend		= RImplementation.o.fp16_blend;
	D3DBLEND	dest		= blend?D3DBLEND_ONE:D3DBLEND_ZERO;
	
   if( Name )
      ::Render->m_MSAASample = atoi( Definition );
   else
      ::Render->m_MSAASample = -1;

	C.r_Pass			("accum_volume",	"accum_indirect_msaa",false,	FALSE,FALSE,blend,D3DBLEND_ONE,dest);
	
	C.r_dx10Texture		("s_position",		r2_RT_P);
	C.r_dx10Texture		("s_normal",		r2_RT_N);
	C.r_dx10Texture		("s_material",		r2_material);
	C.r_dx10Texture		("s_accumulator",	r2_RT_accum		);

	C.r_dx10Sampler		("smp_nofilter");
	C.r_dx10Sampler		("smp_material");
	C.r_End				();
 
   ::Render->m_MSAASample = -1;
}

