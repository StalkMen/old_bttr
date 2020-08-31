#pragma once
#include "../states/state_hide_from_point.h"
#include "bloodsucksoc_predator.h"

#define CStateBloodsuckerVampireHideAbstract CStateBloodsucksocVampireHide<_Object>

template <typename _Object>
CStateBloodsuckerVampireHideAbstract::CStateBloodsucksocVampireHide(_Object* obj) : inherited(obj)
{
    add_state(eStateVampire_RunAway, new CStateMonsterHideFromPoint<_Object>(obj));
    add_state(eStatePredator, new CStateBloodsuckerPredator<_Object>(obj));
}

template <typename _Object>
void CStateBloodsuckerVampireHideAbstract::reselect_state()
{
    if (prev_substate == eStateVampire_RunAway)
    {
        if (get_state(eStatePredator)->check_start_conditions())
        {
            select_state(eStatePredator);
            return;
        }
    }

    select_state(eStateVampire_RunAway);
}

template <typename _Object>
void CStateBloodsuckerVampireHideAbstract::setup_substates()
{
    state_ptr state = get_state_current();

    if (current_substate == eStateVampire_RunAway)
    {
        SStateHideFromPoint data;
        data.point = object->EnemyMan.get_enemy_position();
        data.accelerated = true;
        data.braking = false;
        data.accel_type = eAT_Aggressive;
        data.distance = 50.f;
        data.action.action = ACT_RUN;
        data.action.sound_type = MonsterSound::eMonsterSoundAggressive;
        data.action.sound_delay = object->db().m_dwAttackSndDelay;
        data.action.time_out = 15000;

        state->fill_data_with(&data, sizeof(SStateHideFromPoint));

        return;
    }
}

template <typename _Object>
bool CStateBloodsuckerVampireHideAbstract::check_completion()
{
    if ((current_substate == eStatePredator) && get_state_current()->check_completion())
        return true;

    return false;
}

#undef CStateBloodsuckerVampireHideAbstract
