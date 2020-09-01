#pragma once

#include "../state.h"

template<typename _Object>
class CStateChimecsThreatenWalk : public CStateMonsterMoveToPointEx<_Object> {
	typedef CStateMonsterMoveToPointEx<_Object>		inherited;

public:
	IC					CStateChimecsThreatenWalk	(_Object *obj) : inherited(obj){}
	virtual	void		initialize					();	
	virtual	void		execute						();
	virtual bool		check_completion			();
	virtual bool		check_start_conditions		();
	
};

#include "chimera_cs_state_threaten_walk_inline.h"

