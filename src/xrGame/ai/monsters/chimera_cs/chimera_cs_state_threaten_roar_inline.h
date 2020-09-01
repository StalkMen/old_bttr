#pragma once

#define CStateChimecsThreatenRoarAbstract CStateChimecsThreatenRoar<_Object>

template <typename _Object>
void CStateChimecsThreatenRoarAbstract::initialize()
{
	inherited::initialize	();

}

template <typename _Object>
void CStateChimecsThreatenRoarAbstract::execute()
{

	object->set_action				(ACT_STAND_IDLE);
	object->anim().SetSpecParams	(ASP_THREATEN);
	object->set_state_sound			(MonsterSound::eMonsterSoundThreaten);
	object->dir().face_target		(object->EnemyMan.get_enemy(), 1200);
}

#define STATE_TIME_OUT	4000

template <typename _Object>
bool CStateChimecsThreatenRoarAbstract::check_completion()
{	
	if (time_state_started + STATE_TIME_OUT < Device.dwTimeGlobal) return true;
	return false;
}


#undef CStateChimecsThreatenRoarAbstract

