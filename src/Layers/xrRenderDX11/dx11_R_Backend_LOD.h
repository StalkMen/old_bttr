#pragma once

class R_LOD
{
public:
	R_constant*		c_LOD;

public:
	R_LOD		();

	void			unmap() {c_LOD = 0;}
	void			set_LOD(R_constant* C) {c_LOD = C;}
	void			set_LOD(float camera_distance);
};
