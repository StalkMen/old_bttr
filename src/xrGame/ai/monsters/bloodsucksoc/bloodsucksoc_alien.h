#pragma once

class CAI_Bloodsucksoc;
class CAlienEffectsoc;
class CAlienEffectsocPP;

class CBloodsucksocAlien {
	
	CAI_Bloodsucksoc			*m_object;
	
	bool						m_active;

	CAlienEffectsoc				*m_effector;
	CAlienEffectsocPP			*m_effector_pp;
	
	bool						m_crosshair_show;

public:
			CBloodsucksocAlien	();
			~CBloodsucksocAlien	();
	
	void	init_external		(CAI_Bloodsucksoc *obj);	
	void	reinit				();

	void	activate			();
	void	deactivate			();

	bool	active				() {return m_active;}

};