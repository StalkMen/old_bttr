#pragma once
#include "../monster_state_manager.h"

class CBurcs;

class CStateManagerBurcs : public CMonsterStateManager<CBurcs> {
	typedef CMonsterStateManager<CBurcs> inherited;
public:
					CStateManagerBurcs		(CBurcs *monster); 
	virtual void	execute					();
	virtual void	setup_substates			();
	virtual void	remove_links			(CObject* object) { inherited::remove_links(object);}
};

