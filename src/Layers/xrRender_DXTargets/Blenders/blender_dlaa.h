#pragma once

class CBlender_dlaa : public IBlender
{
public:
	virtual		LPCSTR		getComment() { return "DLAA Shader"; }
	virtual		BOOL		canBeDetailed() { return FALSE; }
	virtual		BOOL		canBeLMAPped() { return FALSE; }

	virtual		void		Compile(CBlender_Compile& C);

	CBlender_dlaa();
	virtual ~CBlender_dlaa();
};