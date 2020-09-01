#pragma once

#include "../state.h"
#include "../states/monster_state_attack.h"

template<typename _Object>
class	CStateBurcsAttackMelee : public CStateMonsterAttack<_Object> 
{
	typedef CStateMonsterAttack<_Object>	inherited;

public:
						CStateBurcsAttackMelee			(_Object *obj);
	virtual	bool		check_start_conditions			();
	virtual	bool		check_completion				();
};

#include "Burcs_state_attack_melee_inline.h"
