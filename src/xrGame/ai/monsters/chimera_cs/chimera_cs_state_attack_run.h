#pragma once

#include "../state.h"
template<typename _Object>
class CStateChimecsAttackRun : public CState<_Object> {
	typedef CState<_Object> inherited;
	typedef CState<_Object>*	state_ptr;
	
	TTime				m_time_action_change;
	EAction				action;

public:
						CStateChimecsAttackRun	(_Object *obj);
	virtual				~CStateChimecsAttackRun	();

	virtual void		initialize				();
	virtual	void		execute					();
	virtual void		finalize_cs				();
	virtual void		critical_finalize_cs	();
			bool		check_home_point_cs		();	
	virtual void		remove_links			(CObject* object) { inherited::remove_links(object);}

private:
	        void		choose_action			();

};

#include "chimera_cs_state_attack_run_inline.h"

