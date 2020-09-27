#pragma once

// Blender_Recorder_StandartBinding.cpp
class ShadersExternalData //--#SM+#--
{
public:
	Fmatrix m_script_params;
	Fvector4 hud_params;
	Fvector4 m_blender_mode;

	ShadersExternalData()
	{
		m_script_params = Fmatrix();
		hud_params.set(0.f, 0.f, 0.f, 0.f);
		m_blender_mode.set(0.f, 0.f, 0.f, 0.f);
	}
};