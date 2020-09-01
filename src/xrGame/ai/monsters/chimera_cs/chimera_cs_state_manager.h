#pragma once

#include "../monster_state_manager.h"

class CChimecs;

class CStateManagerChimecs : public CMonsterStateManager<CChimecs> {
	
	typedef CMonsterStateManager<CChimecs> inherited;

public:
						CStateManagerChimecs	(CChimecs *obj);
	virtual				~CStateManagerChimecs	();

	virtual	void		execute					();
    virtual void		remove_links(CObject* object) { inherited::remove_links(object); }
};

