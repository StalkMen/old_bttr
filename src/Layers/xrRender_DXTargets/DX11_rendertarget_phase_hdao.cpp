#include "stdafx.h"

static unsigned int g_uGroupTexelDimension = 56;
static unsigned int g_uGroupTexelOverlap = 12;
static unsigned int g_uGroupTexelDimensionAfterOverlap = g_uGroupTexelDimension - 2 * g_uGroupTexelOverlap;
extern ENGINE_API u32 ps_r_ssao;
extern ENGINE_API u32 ps_r_ssao_mode;

void CRenderTarget::phase_hdao	()
{
   if( ps_r_ssao > 0 )
   {
       ShaderElement* S;

       if( !RMSAA._opt.dx10_msaa )
          S = (&*(s_hdao_cs->E[0]));
       else
          S = (&*(s_hdao_cs_msaa->E[0]));

       SPass&	P = *(S->passes[0]);
       RCache.set_States		(P.state);
       RCache.set_Constants	    (P.constants);
       RCache.set_Textures	    (P.T);
       RCache.set_CS			(P.cs);

       // set the cs shader output
       UINT UAVInitialCounts=1;
       ID3D11UnorderedAccessView* uav[1]={0};
       ID3D11RenderTargetView* oldrtv[8]={0,0,0,0,0,0,0,0};
       ID3D11DepthStencilView* olddsv;
       ID3D11RenderTargetView* rtv[8]={0,0,0,0,0,0,0,0};
       ID3D11ShaderResourceView* srv[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
       //ID3D11ShaderResourceView* new_srv[2]={rt_ssao_temp1->pTexture->m_pSRView,rt_ssao_temp2->pTexture->m_pSRView};
       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->OMGetRenderTargets( 8, oldrtv, &olddsv );
       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->OMSetRenderTargets( 8, rtv, NULL );
       //DEVICE_HW::CRYRAY_RENDER::HW.pContext->CSSetShaderResources( 0, 2, new_srv );
       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->CSSetUnorderedAccessViews( 0, 1, &rt_ssao_temp->pUAView, &UAVInitialCounts );

       int iGroupsX = (int)ceil( (float)dwWidth / (float)g_uGroupTexelDimensionAfterOverlap );
       int iGroupsY = (int)ceil( (float)dwHeight / (float)g_uGroupTexelDimensionAfterOverlap );
       RCache.Compute( iGroupsX, iGroupsY, 1 );

       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->CSSetUnorderedAccessViews( 0, 1, uav, &UAVInitialCounts );
       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->CSSetShaderResources( 0, 16, srv );
       DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->OMSetRenderTargets( 8, oldrtv, olddsv );
   }
}
