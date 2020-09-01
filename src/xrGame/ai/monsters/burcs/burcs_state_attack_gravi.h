#pragma once
#include "../state.h"

template<typename _Object>
class CStateBurcsAttackGravi : public CState<_Object> 
{
	typedef CState<_Object> inherited;	
	
	enum 
	{
		ACTION_GRAVI_STARTED,
		ACTION_GRAVI_CONTINUE,
		ACTION_GRAVI_FIRE,
		ACTION_WAIT_TRIPLE_END,
		ACTION_COMPLETED,
	} 
	m_action;

	u32			time_gravi_started;

public:
							CStateBurcsAttackGravi	(_Object *obj);

		virtual	void		initialize				();
		virtual	void		execute					();
		virtual void		finalize				();
		virtual void		critical_finalize		();
		virtual void		remove_links			(IGObj* object) { inherited::remove_links(object);}

		virtual bool		check_start_conditions	();
		virtual bool		check_completion		();

private:
				// выполнять состояние
				void		ExecuteGraviStart		();
				void		ExecuteGraviContinue	();
				void		ExecuteGraviFire		();

};

#include "Burcs_state_attack_gravi_inline.h"

