#pragma once
#include "../monster_state_manager.h"

class CAI_CBocs;

class CStateManagerBocs : public CMonsterStateManager<CAI_CBocs> {
	typedef CMonsterStateManager<CAI_CBocs> inherited;

public:
						CStateManagerBocs	(CAI_CBocs *obj);
	virtual				~CStateManagerBocs	();

	virtual	void		execute					();
	virtual void		remove_links			(CObject* object) { inherited::remove_links(object);}
};
