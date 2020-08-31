#pragma once

#include "bloodsucksoc_vampire_execute.h"
#include "ai/Monsters/states/state_hide_from_point.h"
#include "bloodsucksoc_vampire_approach.h"
#include "bloodsucksoc_vampire_hide.h"

#define CStateBloodsuckerVampireAbstract CStateBloodsucksocVampire<_Object>

#define RUN_AWAY_DISTANCE 50.f

template <typename _Object>
CStateBloodsuckerVampireAbstract::CStateBloodsucksocVampire(_Object* obj) : inherited(obj)
{
    add_state(eStateVampire_ApproachEnemy, new CStateBloodsucksocVampireApproach<_Object>(obj));
    add_state(eStateVampire_Execute, new CStateBloodsucksocVampireExecute<_Object>(obj));
    add_state(eStateVampire_RunAway, new CStateMonsterHideFromPoint<_Object>(obj));
    add_state(eStateVampire_Hide, new CStateBloodsucksocVampireHide<_Object>(obj));
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::reinit() { inherited::reinit(); }
template <typename _Object>
void CStateBloodsuckerVampireAbstract::initialize()
{
    inherited::initialize();
    object->set_visibility_state(CAI_Bloodsucksoc::partial_visibility);

    enemy = object->EnemyMan.get_enemy();

    object->sound().play(CAI_Bloodsucksoc::eVampireStartHunt);
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::reselect_state()
{
    u32 state_id = u32(-1);

    // check if we can start execute
    if (prev_substate == eStateVampire_ApproachEnemy)
    {
        if (get_state(eStateVampire_Execute)->check_start_conditions())
            state_id = eStateVampire_Execute;
    }

    // check if we executed
    if (prev_substate == eStateVampire_Execute)
        state_id = eStateVampire_Hide;

    // check if reach time in vampire state is out - then hide
    if (prev_substate == eStateVampire_ApproachEnemy)
        state_id = eStateVampire_Hide;

    // check if we hiding - then hide again
    if (prev_substate == eStateVampire_Hide)
        state_id = eStateVampire_Hide;

    // else just
    if (state_id == u32(-1))
        state_id = eStateVampire_ApproachEnemy;

    select_state(state_id);
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::check_force_state()
{
    // check if we can start execute
    if (prev_substate == eStateVampire_ApproachEnemy)
    {
        if (get_state(eStateVampire_Execute)->check_start_conditions())
            current_substate = u32(-1);
    }
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::finalize()
{
    inherited::finalize();

    object->set_visibility_state(CAI_Bloodsucksoc::full_visibility);
    CAI_Bloodsucksoc::m_time_last_vampire = Device.dwTimeGlobal;
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::critical_finalize()
{
    inherited::critical_finalize();

    object->set_visibility_state(CAI_Bloodsucksoc::full_visibility);
    CAI_Bloodsucksoc::m_time_last_vampire = Device.dwTimeGlobal;
}

template <typename _Object>
bool CStateBloodsuckerVampireAbstract::check_start_conditions()
{
    if (!object->WantVampire())
        return false;
    if (object->berserk_always)
        return false;

    // является ли враг актером
    const CEntityAlive* enemy = object->EnemyMan.get_enemy();
    if (!smart_cast<CActor const*>(enemy))
        return false;
    if (!object->EnemyMan.see_enemy_now())
        return false;
    if (object->CControlledActor::is_controlling())
        return false;

    const CActor* actor = smart_cast<const CActor*>(enemy);
    VERIFY(actor);
    if (actor->input_external_handler_installed())
        return false;

    if (CAI_Bloodsucker::m_time_last_vampire + object->m_vampire_min_delay > Device.dwTimeGlobal)
        return false;

    return true;
}

template <typename _Object>
bool CStateBloodsuckerVampireAbstract::check_completion()
{
    // если убежал
    if ((current_substate == eStateVampire_Hide) && get_state_current()->check_completion())
        return true;

    // если враг изменился
    if (enemy != object->EnemyMan.get_enemy())
        return true;

    // если актера уже контролит другой кровосос
    if ((current_substate != eStateVampire_Execute) && object->CControlledActor::is_controlling())
        return true;

    return false;
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::setup_substates()
{
    state_ptr state = get_state_current();

    if (current_substate == eStateVampire_RunAway)
    {
        SStateHideFromPoint data;
        data.point = object->EnemyMan.get_enemy_position();
        data.accelerated = true;
        data.braking = false;
        data.accel_type = eAT_Aggressive;
        data.distance = RUN_AWAY_DISTANCE;
        data.action.action = ACT_RUN;
        data.action.sound_type = MonsterSound::eMonsterSoundAggressive;
        data.action.sound_delay = object->db().m_dwAttackSndDelay;
        data.action.time_out = 15000;

        state->fill_data_with(&data, sizeof(SStateHideFromPoint));

        return;
    }
}

template <typename _Object>
void CStateBloodsuckerVampireAbstract::remove_links(CObject* object)
{
    if (enemy == object)
        enemy = 0;
}

#undef CStateBloodsuckerVampireAbstract
