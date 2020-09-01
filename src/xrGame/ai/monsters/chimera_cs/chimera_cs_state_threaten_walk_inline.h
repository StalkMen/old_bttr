#pragma once

#define CStateChimecsThreatenWalkAbstract CStateChimecsThreatenWalk<_Object>

template <typename _Object>
void CStateChimecsThreatenWalkAbstract::initialize()
{
	inherited::initialize();

	object->SetUpperState	();

	data.point				= object->EnemyMan.get_enemy_position	();
	data.vertex				= object->EnemyMan.get_enemy_vertex		();

	data.action.action		= ACT_WALK_FWD;

	data.accelerated		= true;
	data.braking			= false;
	data.accel_type 		= eAT_Calm;

	data.completion_dist	= 2.f;
	data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
	data.action.sound_delay = object->db().m_dwIdleSndDelay;
	data.time_to_rebuild	= 1500;
}


template <typename _Object>
void CStateChimecsThreatenWalkAbstract::execute()
{
	data.point				= object->EnemyMan.get_enemy_position	();
	data.vertex				= object->EnemyMan.get_enemy_vertex		();

	inherited::execute();
}

#define DISTANCE_TO_ENEMY		5.f

template <typename _Object>
bool CStateChimecsThreatenWalkAbstract::check_completion()
{	
	if (inherited::check_completion()) return true;

	float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
	if (dist_to_enemy < DISTANCE_TO_ENEMY) return true;

	return false;
}

#define MAX_DISTANCE_TO_ENEMY	8.f

template <typename _Object>
bool CStateChimecsThreatenWalkAbstract::check_start_conditions()
{
	float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
	if (dist_to_enemy < MAX_DISTANCE_TO_ENEMY) return true;
	return false;
}


#undef CStateChimecsThreatenWalkAbstract

