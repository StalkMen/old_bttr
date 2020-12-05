#include "stdafx.h"
#pragma hdrstop

#include "hwcaps.h"
#include "hw.h"
#include <nvapi.h>
#include "../xrEngine/AMD.h"
#include "../xrEngine/NVIDIA.h"

namespace
{
	u32 GetNVGpuNum()
	{
		NvLogicalGpuHandle  logicalGPUs[NVAPI_MAX_LOGICAL_GPUS];
		NvU32               logicalGPUCount;
		NvPhysicalGpuHandle physicalGPUs[NVAPI_MAX_PHYSICAL_GPUS];
		NvU32               physicalGPUCount;

		int iGpuNum = 0;

		NvAPI_Status	status;
		status = NvAPI_Initialize();

		if (status != NVAPI_OK)
		{
			Msg("! NVIDIA is missing.");
			return iGpuNum;
		}

		// enumerate logical gpus
		status = NvAPI_EnumLogicalGPUs(logicalGPUs, &logicalGPUCount);
		if (status != NVAPI_OK)
		{
			Msg("! NVIDIA_EnumLogicalGPUs failed!");
			return iGpuNum;
			// error
		}

		// enumerate physical gpus
		status = NvAPI_EnumPhysicalGPUs(physicalGPUs, &physicalGPUCount);
		if (status != NVAPI_OK)
		{
			Msg("! NVIDIA_EnumPhysicalGPUs failed!");
			return iGpuNum;
			// error
		}

		Msg	("- NVIDIA MGPU: Logical(%d), Physical(%d)", physicalGPUCount, logicalGPUCount);

		//	Assume that we are running on logical GPU with most physical GPUs connected.
		for ( u32 i = 0; i<logicalGPUCount; ++i )
		{
			status = NvAPI_GetPhysicalGPUsFromLogicalGPU( logicalGPUs[i], physicalGPUs, &physicalGPUCount);
			if (status == NVAPI_OK)
				iGpuNum = _max( iGpuNum, physicalGPUCount);
		}

		if (iGpuNum>1)
		{
			Msg	("- NVIDIA MGPU: %d-Way SLI detected.", iGpuNum);
		}

		return iGpuNum;
	}

	u32 GetATIGpuNum()
	{
		int iGpuNum = 1;

		if (iGpuNum>1)
		{
			Msg	("# ATI MGPU: %d-Way CrossFire detected.", iGpuNum);
		}

		return iGpuNum;
	}

	u32 GetGpuNum()
	{
		u32 res = GetNVGpuNum();

		res = _max( res, GetATIGpuNum() );

		res = _max( res, 2 );

		res = _min( res, CHWCaps::MAX_GPUS );

		//	It's vital to have at least one GPU, else
		//	code will fail.
		VERIFY(res>0);
		
#ifdef DIRECTX10
		Msg("# DX10 starting rendering as GPU: %d.", res);
#else	
		Msg("# DX11 starting rendering as GPU: %d.", res);
#endif		
		return res;
	}

	u32 DeviceStatistics()
	{
		try
		{
			if (!AMDData)
				AMDData = xr_new<CAMDReader>();
			if (!NvData)
				NvData = xr_new<CNvReader>();

			if (CAMDReader::bAMDSupportADL)
			{
				u32 _AMD = AMDData->GetGPUCount() > 0 ? AMDData->GetGPUCount() : 1;
				Msg("~ [AMD]: Information about a certain number of GPUs from the video card: %i.", _AMD);
				return _AMD;
			}
			else if (CNvReader::bSupport)
			{
				u32 _NVIDIA = NvData->GetGPUCount() > 0 ? NvData->GetGPUCount() : 1;
				Msg("~ [NVIDIA]: Information about a certain number of GPUs from the video card: %i.", _NVIDIA);
				return _NVIDIA;
			}
			else
			{
				return 1;
			}
		}
		catch (...)
		{
			return 1;
		}
	}
}

void CHWCaps::Update()
{
	// ***************** GEOMETRY
	geometry_major				= 4;
	geometry_minor				= 0;
	geometry.bSoftware			= FALSE;
	geometry.bPointSprites		= FALSE;
	geometry.bNPatches			= FALSE;
	DWORD cnt					= 256;
	clamp<DWORD>(cnt,0,256);
	geometry.dwRegisters		= cnt;
	geometry.dwInstructions		= 256;
	geometry.dwClipPlanes		= _min(6,15);
	geometry.bVTF				= TRUE;

	// ***************** PIXEL processing
	raster_major				= 4;
	raster_minor				= 0;
	raster.dwStages				= 16;
	raster.bNonPow2				= TRUE;
	raster.bCubemap				= TRUE;
	raster.dwMRT_count			= 4;
	//raster.b_MRT_mixdepth		= FALSE;
	raster.b_MRT_mixdepth		= TRUE;
	raster.dwInstructions		= 256;

	// ***************** Info
	Msg							("~ GPU shading: vs(%x/%d.%d/%d), ps(%x/%d.%d/%d)",
		0,	geometry_major, geometry_minor, CAP_VERSION(geometry_major,	geometry_minor),
		0,	raster_major,	raster_minor,	CAP_VERSION(raster_major,	raster_minor)
		);

	// *******1********** Vertex cache
	//	TODO: DX10: Find a way to detect cache size
	geometry.dwVertexCache = 24;
	Msg					("~ GPU vertex cache: %s, %d","unrecognized",u32(geometry.dwVertexCache));

	// *******1********** Compatibility : vertex shader
	if (0==raster_major)		geometry_major=0;		// Disable VS if no PS

	//
	bTableFog			=	FALSE;	//BOOL	(caps.RasterCaps&D3DPRASTERCAPS_FOGTABLE);

	// Detect if stencil available
	bStencil			=	TRUE;

	// Scissoring
	bScissor	= TRUE;

	// Stencil relative caps
	soInc=D3DSTENCILOP_INCRSAT;
	soDec=D3DSTENCILOP_DECRSAT;
	dwMaxStencilValue=(1<<8)-1;

	// DEV INFO

	iGPUNum = GetGpuNum();
	iGPUStats = DeviceStatistics();

//	Msg("# [%s] Detected GpuNum : [%u]", __FUNCTION__, iGPUNum);
}
