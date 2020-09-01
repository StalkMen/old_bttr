#pragma once

#define CStateChimecsAttackRunAbstract CStateChimecsAttackRun<_Object>

template <typename _Object>
CStateChimecsAttackRunAbstract::CStateChimecsAttackRun(_Object *obj) : inherited(obj)
{
	add_state(eStateAttack_MoveToHomePoint,	new CStateMonsterAttackMoveToHomePoint<CChimecs> (obj));	
 	add_state(eStateAttack_Run_chimera,				new CStateMonsterAttackRun<CChimecs> 			(obj));
 	add_state(eStateAttack_Melee_chimera,			new CStateMonsterAttackMelee<CChimecs> 			(obj));	
}

template <typename _Object>
CStateChimecsAttackRunAbstract::~CStateChimecsAttackRun()
{
}

template <typename _Object>
bool CStateChimecsAttackRunAbstract::check_home_point_cs()
{
	if (prev_substate != eStateAttack_MoveToHomePoint) {
		if (get_state(eStateAttack_MoveToHomePoint)->check_start_conditions())	return true;
	} else {
		if (!get_state(eStateAttack_MoveToHomePoint)->check_completion())		return true;
	}

	return false;
}


template <typename _Object>
void CStateChimecsAttackRunAbstract::initialize()
{
	inherited::initialize			();
}

template <typename _Object>
void CStateChimecsAttackRunAbstract::execute()
{
	object->anim().clear_override_animation	();
	
	if	( check_home_point_cs() )
	{
		select_state					(eStateAttack_MoveToHomePoint);
		get_state_current()->execute	();
		prev_substate				=	current_substate;
		return;
	}	
	
	EMonsterState		state_id	=	eStateUnknown;
	const CEntityAlive* enemy		=	object->EnemyMan.get_enemy();

	if (current_substate == eStateAttack_Melee_chimera)
	{
		if (get_state(eStateAttack_Melee_chimera)->check_completion())
			state_id = eStateAttack_Run_chimera;
		else
			state_id = eStateAttack_Melee_chimera;
	}
	else
	{
		if ( get_state(eStateAttack_Melee_chimera)->check_start_conditions() )
			state_id = eStateAttack_Melee_chimera;
		else
			state_id = eStateAttack_Run_chimera;
	}
	
	select_state						(state_id);
	get_state_current()->execute		();
	prev_substate					=	current_substate;	
	
}

template <typename _Object>
void CStateChimecsAttackRunAbstract::choose_action()
{
}

template <typename _Object>
void CStateChimecsAttackRunAbstract::finalize_cs()
{
	inherited::finalize();
}

template <typename _Object>
void CStateChimecsAttackRunAbstract::critical_finalize_cs()
{
	inherited::critical_finalize();
}

#undef CStateChimecsAttackRunAbstract


