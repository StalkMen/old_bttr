#include "stdafx.h"
#include "dx11_r_backend_lod.h"

extern ENGINE_API u32 optTessQuality_;

R_LOD::R_LOD()
{
	unmap();
}

void R_LOD::set_LOD(float camera_distance)
{
	if (c_LOD && optTessQuality_ == 0) // low quality
	{
		if (camera_distance <= 3.f)
			RCache.set_c(c_LOD, 2.f);
		else
			RCache.set_c(c_LOD, 1.f);
	}
	else if (c_LOD && optTessQuality_ == 1) // medium quality
	{
		if (camera_distance >= 5.f)
			RCache.set_c(c_LOD, 1.f);
		else if (camera_distance >= 2.f)
			RCache.set_c(c_LOD, 2.f);
		else
			RCache.set_c(c_LOD, 3.f);
	}
	else if (c_LOD && optTessQuality_ == 2) // optimal quality
	{
		if (camera_distance >= 7.f)
			RCache.set_c(c_LOD, 1.f);
		else if (camera_distance >= 4.f)
			RCache.set_c(c_LOD, 2.f);
		else if (camera_distance >= 2.f)
			RCache.set_c(c_LOD, 3.f);
		else
			RCache.set_c(c_LOD, 4.f);
	}
	else // overmuch quality
	{
		if (camera_distance >= 10.f)
			RCache.set_c(c_LOD, 1.f);
		else if (camera_distance >= 7.f)
			RCache.set_c(c_LOD, 2.f);
		else if (camera_distance >= 5.f)
			RCache.set_c(c_LOD, 4.f);
		else if (camera_distance >= 3.f)
			RCache.set_c(c_LOD, 6.f);
		else
			RCache.set_c(c_LOD, 8.f);
	}
}