#pragma once

#include "chimera_cs_state_hunting_move_to_cover.h"
#include "chimera_cs_state_hunting_come_out.h"

#define CStateChimecsHuntingAbstract CStateChimecsHunting<_Object>

template <typename _Object>
CStateChimecsHuntingAbstract::CStateChimecsHunting(_Object *obj) : inherited(obj)
{
	add_state(eStateMoveToCover,	xr_new<CStateChimecsHuntingMoveToCover<_Object> >	(obj));
	add_state(eStateComeOut,		xr_new<CStateChimecsHuntingComeOut<_Object> >		(obj));
}


template <typename _Object>
bool CStateChimecsHuntingAbstract::check_start_conditions()
{
	return true;
}

template <typename _Object>
bool CStateChimecsHuntingAbstract::check_completion()
{
	return false;
}

template <typename _Object>
void CStateChimecsHuntingAbstract::reselect_state()
{
	if (prev_substate == u32(-1))					select_state(eStateMoveToCover);
	else if (prev_substate == eStateMoveToCover)	select_state(eStateComeOut);
	else											select_state(eStateMoveToCover);
}


#undef CStateChimecsHuntingAbstract
