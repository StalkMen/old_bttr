#pragma once
#include "../monster_state_manager.h"

class CPseudoGigant_jumper;

class CStateManagerGigant_jumper : public CMonsterStateManager<CPseudoGigant_jumper> {
	typedef CMonsterStateManager<CPseudoGigant_jumper> inherited;
public:

					CStateManagerGigant_jumper	(CPseudoGigant_jumper *monster); 
	virtual void	execute				();
	virtual void	remove_links		(CObject* object) { inherited::remove_links(object);}
};
