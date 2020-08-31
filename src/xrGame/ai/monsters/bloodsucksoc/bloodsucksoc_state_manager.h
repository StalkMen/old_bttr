#pragma once
#include "../monster_state_manager.h"

class CAI_Bloodsucksoc;

class CStateManagerBloodsucksoc : public CMonsterStateManager<CAI_Bloodsucksoc> 
{
	typedef CMonsterStateManager<CAI_Bloodsucksoc> inherited;

public:
					CStateManagerBloodsucksoc	(CAI_Bloodsucksoc *monster); 
	virtual void	execute						();
	virtual void	remove_links				(CObject* object) { inherited::remove_links(object); }
	bool			check_vampire();
};
