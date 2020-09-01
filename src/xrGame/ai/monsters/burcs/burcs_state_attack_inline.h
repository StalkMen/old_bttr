#pragma once

#include "Burcs_state_attack_tele.h"
#include "Burcs_state_attack_gravi.h"
#include "Burcs_state_attack_melee.h"
#include "../states/state_look_point.h"
#include "../states/state_move_to_restrictor.h"
#include "Burcs_state_attack_run_around.h"

#define GRAVI_PERCENT		80
#define TELE_PERCENT		50
#define RUN_AROUND_PERCENT	20

#define CStateBurcsAttackAbstract CStateBurcsAttack<_Object>

template <typename _Object>
CStateBurcsAttackAbstract::CStateBurcsAttack(_Object *obj) : inherited(obj)
{
	add_state(eStateBurcsAttack_Tele,		xr_new CStateBurcsAttackTele<_Object> 		(obj));
	add_state(eStateBurcsAttack_Gravi,		xr_new CStateBurcsAttackGravi<_Object> 	(obj));
	add_state(eStateBurcsAttack_Melee,		xr_new CStateBurcsAttackMelee<_Object> 	(obj));
	
	add_state(eStateBurcsAttack_FaceEnemy,	xr_new CStateMonsterLookToPoint<_Object> 	(obj));
	add_state(eStateBurcsAttack_RunAround,	xr_new CStateBurcsAttackRunAround<_Object> (obj));

	add_state(eStateCustomMoveToRestrictor,	xr_new CStateMonsterMoveToRestrictor<_Object> (obj));
}

template <typename _Object>
void CStateBurcsAttackAbstract::initialize()
{
	inherited::initialize	();
	m_force_gravi			= false;
}

template <typename _Object>
void CStateBurcsAttackAbstract::reselect_state()
{
	if (get_state(eStateBurcsAttack_Melee)->check_start_conditions()) 
	{
		select_state(eStateBurcsAttack_Melee);
		return;
	}

	if (m_force_gravi) {
		m_force_gravi = false;

		if (get_state(eStateBurcsAttack_Gravi)->check_start_conditions()) 
		{
			select_state		(eStateBurcsAttack_Gravi);
			return;
		}
	}

	if (get_state(eStateCustomMoveToRestrictor)->check_start_conditions()) 
	{
		select_state(eStateCustomMoveToRestrictor);
		return;
	}

	bool enable_gravi	= false;
	bool enable_tele	= get_state(eStateBurcsAttack_Tele)->check_start_conditions		();

	if (!enable_gravi && !enable_tele) 
	{
		if (prev_substate == eStateBurcsAttack_RunAround) 
			select_state(eStateBurcsAttack_FaceEnemy);
		else 	
			select_state(eStateBurcsAttack_RunAround);
		return;
	}

	if (enable_gravi && enable_tele) {

		u32 rnd_val = ::Random.randI(GRAVI_PERCENT + TELE_PERCENT + RUN_AROUND_PERCENT);
		u32 cur_val = GRAVI_PERCENT;

		if (rnd_val < cur_val) 
		{
			select_state(eStateBurcsAttack_Gravi);
			return;
		}

		cur_val += TELE_PERCENT;
		if (rnd_val < cur_val) 
		{
			select_state(eStateBurcsAttack_Tele);
			return;
		}

		select_state(eStateBurcsAttack_RunAround);
		return;
	}

	if ((prev_substate == eStateBurcsAttack_RunAround) || (prev_substate == eStateBurcsAttack_FaceEnemy)) {
		if (enable_gravi) select_state(eStateBurcsAttack_Gravi);
		else select_state(eStateBurcsAttack_Tele);
	} else {
		select_state(eStateBurcsAttack_RunAround);
	}
}

template <typename _Object>
void CStateBurcsAttackAbstract::setup_substates()
{
	state_ptr state = get_state_current();

	if (current_substate == eStateBurcsAttack_FaceEnemy) {
		SStateDataLookToPoint data;
		
		data.point				= object->EnemyMan.get_enemy()->Position(); 
		data.action.action		= ACT_STAND_IDLE;
		data.action.sound_type	= MonsterSound::eMonsterSoundAggressive;
		data.action.sound_delay = object->db().m_dwAttackSndDelay;

		state->fill_data_with	(&data, sizeof(SStateDataLookToPoint));
		return;
	}

}

template <typename _Object>
void CStateBurcsAttackAbstract::check_force_state()
{
	// check if we can start execute
	if ((current_substate == eStateCustomMoveToRestrictor) || (prev_substate == eStateBurcsAttack_RunAround)) {
		if (get_state(eStateBurcsAttack_Gravi)->check_start_conditions()) {
			current_substate	= u32(-1);
			m_force_gravi		= true;
		}
	}
}

#undef CStateBurcsAttackAbstract
