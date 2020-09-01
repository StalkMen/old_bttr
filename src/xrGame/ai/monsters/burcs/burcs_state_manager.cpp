#include "stdafx.h"
#include "Burcs.h"
#include "Burcs_state_manager.h"

#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"
#include "../control_path_builder_base.h"

#include "../states/monster_state_rest.h"
#include "../states/monster_state_panic.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_hear_int_sound.h"
#include "../states/monster_state_hear_danger_sound.h"
#include "../states/monster_state_hitted.h"
#include "../states/state_custom_action.h"

#include "Burcs_state_attack.h"


CStateManagerBurcs::CStateManagerBurcs(CBurcs *monster) : inherited(monster)
{
	add_state(eStateRest,					xr_new CStateMonsterRest<CBurcs> 					(monster));
	add_state(eStatePanic,					xr_new CStateMonsterPanic<CBurcs> 					(monster));
	add_state(eStateAttack,					xr_new CStateBurcsAttack<CBurcs> 					(monster));
	add_state(eStateEat,					xr_new CStateMonsterEat<CBurcs> 					(monster));
	add_state(eStateHearInterestingSound,	xr_new CStateMonsterHearInterestingSound<CBurcs> 	(monster));
	add_state(eStateHearDangerousSound,		xr_new CStateMonsterHearDangerousSound<CBurcs> 	(monster));
	add_state(eStateHitted,					xr_new CStateMonsterHitted<CBurcs> 				(monster));
	add_state(eStateBurcsScanning,			xr_new CStateMonsterCustomAction<CBurcs> 				(monster));
}

#define SCAN_STATE_TIME 4000

void CStateManagerBurcs::execute()
{
	u32 state = u32(-1);

	if (object->EnemyMan.get_enemy()) 
	{
		switch (object->EnemyMan.get_danger_type()) 
		{
				case eStrong:	state = eStatePanic; break;
				case eWeak:		state = eStateAttack; break;
		}
	} 
	else if (object->HitMemory.is_hit() && (object->HitMemory.get_last_hit_time() + 10000 > Device.dwTimeGlobal)) 
		state = eStateHitted;
	else if (object->hear_dangerous_sound || object->hear_interesting_sound) 
	{
		state = eStateHearInterestingSound;
	} 
	else if (object->time_last_scan + SCAN_STATE_TIME > Device.dwTimeGlobal)
	{
		state = eStateBurcsScanning;
	} 
	else if (can_eat()) 
	{
			state = eStateEat;
	} 
	else	state = eStateRest;

	select_state(state); 
	
	// выполнить текущее состояние
	get_state_current()->execute();

	prev_substate = current_substate;
}

void CStateManagerBurcs::setup_substates()
{
	if (current_substate == eStateBurcsScanning) {
		SStateDataAction	data;
		
		data.action			= ACT_LOOK_AROUND;
		data.sound_type		= MonsterSound::eMonsterSoundIdle;
		data.sound_delay	= object->db().m_dwIdleSndDelay;
		
		get_state_current()->fill_data_with(&data, sizeof(SStateDataAction));
		return;
	}
}
