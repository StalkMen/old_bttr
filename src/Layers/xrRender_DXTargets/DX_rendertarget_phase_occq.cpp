#include "stdafx.h"

void	CRenderTarget::phase_occq	()
{
	if( !RMSAA._opt.dx10_msaa )
		u_setrt						( Device.dwWidth,Device.dwHeight,DEVICE_HW::CRYRAY_RENDER::HW.pBaseRT,NULL,NULL,DEVICE_HW::CRYRAY_RENDER::HW.pBaseZB);
	else
		u_setrt						( Device.dwWidth,Device.dwHeight,NULL,NULL,NULL,rt_MSAADepth->pZRT);
	RCache.set_Shader			( s_occq	);
	RCache.set_CullMode			( CULL_CCW	);
	RCache.set_Stencil			(TRUE,D3DCMP_LESSEQUAL,0x01,0xff,0x00);
	RCache.set_ColorWriteEnable	(FALSE		);
}
