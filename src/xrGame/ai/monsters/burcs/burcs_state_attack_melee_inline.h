#pragma once

#define CStateBurcsAttackMeleeAbstract CStateBurcsAttackMelee<_Object>

#define MIN_DIST_MELEE_ATTACK	5.f
#define MAX_DIST_MELEE_ATTACK	9.f

template <typename _Object>
CStateBurcsAttackMeleeAbstract::CStateBurcsAttackMelee(_Object *obj) : inherited(obj)
{
}

template <typename _Object>
bool CStateBurcsAttackMeleeAbstract::check_start_conditions()
{
	float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
	if (dist > MIN_DIST_MELEE_ATTACK) return false;

	return true;
}

template <typename _Object>
bool CStateBurcsAttackMeleeAbstract::check_completion()
{
	float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
	if (dist < MAX_DIST_MELEE_ATTACK) return false;

	return true;

}


#undef CStateBurcsAttackMeleeAbstract
