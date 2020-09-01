#pragma once

#define CStateChimecsHuntingMoveToCoverAbstract CStateChimecsHuntingMoveToCover<_Object>

template <typename _Object>
CStateChimecsHuntingMoveToCoverAbstract::CStateChimecsHuntingMoveToCover(_Object *obj) : inherited(obj)
{
}

template <typename _Object>
void CStateChimecsHuntingMoveToCoverAbstract::initialize()
{
	inherited::initialize();
	
	
}

template <typename _Object>
bool CStateChimecsHuntingMoveToCoverAbstract::check_completion()
{
	return false;
}

template <typename _Object>
void CStateChimecsHuntingMoveToCoverAbstract::execute()
{
	
}

#undef CStateChimecsHuntingMoveToCoverAbstract
