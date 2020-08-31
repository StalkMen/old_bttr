#pragma once
#include "../monster_state_manager.h"

class CAI_Bloodsuckcs;

class CStateManagerBloodsuckcs : public CMonsterStateManager<CAI_Bloodsuckcs> {
	typedef CMonsterStateManager<CAI_Bloodsuckcs> inherited;

public:
					CStateManagerBloodsuckcs	(CAI_Bloodsuckcs *monster); 
	virtual void	execute						();
	virtual void	update						();
			void	drag_object					();
	virtual void	remove_links				(CObject* object) { inherited::remove_links(object);}
};
