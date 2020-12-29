#include "stdafx.h"

void CRenderTarget::phase_rain()
{
   if( !RMSAA._opt.dx10_msaa )
      u_setrt	(rt_Color,NULL,NULL,DEVICE_HW::CRYRAY_RENDER::HW.pBaseZB);
   else
      u_setrt	(rt_Color,NULL,NULL,rt_MSAADepth->pZRT);
	//u_setrt	(rt_Normal,NULL,NULL,DEVICE_HW::CRYRAY_RENDER::HW.pBaseZB);
	RImplementation.rmNormal();
}