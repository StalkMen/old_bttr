#pragma once

class CAI_Bloodsuckcs;
class CAlienEffectcs;
class CAlienEffectcsPP;

class CBloodsuckcsAlien {
	
	CAI_Bloodsuckcs				*m_object;
	
	bool						m_active;

	CAlienEffectcs				*m_effector;
	CAlienEffectcsPP			*m_effector_pp;
	
	bool						m_crosshair_show;

public:
			CBloodsuckcsAlien	();
			~CBloodsuckcsAlien	();
	
	void	init_external		(CAI_Bloodsuckcs *obj);	
	void	reinit				();

	void	activate			();
	void	deactivate			();

	bool	active				() {return m_active;}

};