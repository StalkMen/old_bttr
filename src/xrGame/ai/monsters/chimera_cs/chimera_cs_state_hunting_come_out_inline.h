#pragma once

#define CStateChimecsHuntingMoveToCoverAbstract CStateChimecsHuntingMoveToCover<_Object>

template <typename _Object>
CStateChimecsHuntingMoveToCoverAbstract::CStateChimecsHuntingMoveToCover(_Object *obj) : inherited(obj)
{
}


template <typename _Object>
bool CStateChimecsHuntingMoveToCoverAbstract::check_start_conditions()
{
	return true;
}

template <typename _Object>
bool CStateChimecsHuntingMoveToCoverAbstract::check_completion()
{
	return false;
}

template <typename _Object>
void CStateChimecsHuntingMoveToCoverAbstract::reselect_state()
{
	if (prev_substate == u32(-1))					select_state(eStateMoveToCover);
	else if (prev_substate == eStateMoveToCover)	select_state(eStateComeOut);
	else											select_state(eStateMoveToCover);
}

#undef CStateChimecsHuntingMoveToCoverAbstract
