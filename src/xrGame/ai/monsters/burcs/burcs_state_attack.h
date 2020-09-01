#pragma once
#include "../state.h"

template<typename _Object>
class	CStateBurcsAttack : public CState<_Object> {
	typedef CState<_Object> inherited;
	typedef CState<_Object> *state_ptr;

	bool				m_force_gravi;
public:
						CStateBurcsAttack		(_Object *obj);

	virtual	void		initialize				();
							
	virtual	void		reselect_state			();
	virtual void		setup_substates			();
	virtual void		check_force_state		();
	virtual void		remove_links			(CObject* object) { inherited::remove_links(object);}
};

#include "Burcs_state_attack_inline.h"
