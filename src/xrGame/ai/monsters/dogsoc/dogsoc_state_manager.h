#pragma once

#include "../monster_state_manager.h"

class CAI_Dogsoc;

class CStateManagerdogsoc : public CMonsterStateManager<CAI_Dogsoc> {
	typedef CMonsterStateManager<CAI_Dogsoc> inherited;

public:

					CStateManagerdogsoc	(CAI_Dogsoc *monster); 
	virtual void	execute				();
	virtual void	remove_links		(CObject* object) { inherited::remove_links(object);}
};

