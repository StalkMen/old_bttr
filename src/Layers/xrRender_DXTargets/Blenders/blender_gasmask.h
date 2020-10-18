#pragma once

class CBlender_gasmask : public IBlender
{
public:
	virtual		LPCSTR		getComment()	{ return "Gasmask"; }
	virtual		BOOL		canBeDetailed()	{ return FALSE; }
	virtual		BOOL		canBeLMAPped()	{ return FALSE; }

	virtual		void		Compile(CBlender_Compile& C);

	CBlender_gasmask();
	virtual ~CBlender_gasmask();
};