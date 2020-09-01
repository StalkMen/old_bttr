#pragma once

#include "../state.h"

template<typename _Object>
class	CStateChimecsHuntingMoveToCover : public CState<_Object> {
protected:
	typedef CState<_Object> inherited;

public:
						CStateChimecsHuntingMoveToCover	(_Object *obj);

	virtual void		initialize						();
	virtual	void		execute							();
	virtual bool 		check_start_conditions			();
	virtual bool 		check_completion				();
};

#include "Chimecs_state_hunting_move_to_cover_inline.h"
