#include "stdafx.h"

void CRenderTarget::phase_rain()
{
   if( !RImplementation.o.dx10_msaa )
      u_setrt	(rt_Color,NULL,NULL,DEVICE_HW::XRAY::HW.pBaseZB);
   else
      u_setrt	(rt_Color,NULL,NULL,rt_MSAADepth->pZRT);
	//u_setrt	(rt_Normal,NULL,NULL,DEVICE_HW::XRAY::HW.pBaseZB);
	RImplementation.rmNormal();
}