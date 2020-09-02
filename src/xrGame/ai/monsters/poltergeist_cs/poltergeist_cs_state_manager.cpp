#include "stdafx.h"

#include "poltergeist_cs.h"
#include "poltergeist_cs_state_manager.h"

#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"
#include "../control_path_builder_base.h"

#include "poltergeist_cs_state_rest.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_attack.h"
#include "../states/monster_state_panic.h"
#include "poltergeist_cs_state_attack_hidden.h"
#include "../states/monster_state_hear_int_sound.h"
#include "../states/monster_state_hear_danger_sound.h"
#include "../states/monster_state_hitted.h"
#include "../../../entitycondition.h"

CStateManagerPoltergeist_cs::CStateManagerPoltergeist_cs(CPoltergeist_cs* obj) : inherited(obj)
{
    CStateMonsterAttackMoveToHomePoint<CPoltergeist_cs>* PolterAttack_Hands = xr_new<CStateMonsterAttackMoveToHomePoint<CPoltergeist_cs>>(obj);
    add_state(eStateRest, xr_new<CPoltergeist_csStateRest<CPoltergeist_cs>>(obj));
    add_state(eStateEat, xr_new<CStateMonsterEat<CPoltergeist_cs>>(obj));
    add_state(eStateAttack, xr_new<CStateMonsterAttack<CPoltergeist_cs>>(obj, PolterAttack_Hands));
    add_state(eStateAttack_AttackHidden, xr_new<CStatePoltergeistAttackHidden<CPoltergeist_cs>>(obj));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CPoltergeist_cs>>(obj));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CPoltergeist_cs>>(obj));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CPoltergeist_cs>>(obj));
    add_state(eStateHearDangerousSound, xr_new<CStateMonsterHearDangerousSound<CPoltergeist_cs>>(obj));
}

CStateManagerPoltergeist_cs::~CStateManagerPoltergeist_cs() {}

void CStateManagerPoltergeist_cs::reinit()
{
    inherited::reinit();

    time_next_flame_attack = 0;
    time_next_tele_attack = 0;
    time_next_scare_attack = 0;
}

void CStateManagerPoltergeist_cs::execute()
{
    u32 state_id = u32(-1);
    const CEntityAlive* enemy = object->EnemyMan.get_enemy();
// --' OldSerpskiStalker

#ifdef NEW_AI_POLTER
    if (m_tele_cs && !object->logic_ghost)
    {
        if (enemy)
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong:
                object->on_activate();
                state_id = eStatePanic;
                break;
            case eWeak:
                if (enemy->conditions().health() < 0.50f) // Видим раненного врага, спустимся на землю и добьем рукой. Относится к атеру, если бежать уже не можем
                {                                         // CStateMonsterAttack<CPoltergeist_cs>(obj, PolterAttack_Hands)
                    object->on_deactivate();
                    state_id = eStateAttack;
                }
                else
                {
                    state_id = eStatePanic;               // У врага больше 50 %, летаем и кидаемся предметами
                    object->on_activate();
                }
                break;
            }
        }
        else if (object->HitMemory.is_hit())
        {
            state_id = eStateHitted;
        }
        else if (object->hear_interesting_sound || object->hear_dangerous_sound)
        {
            state_id = eStateHearDangerousSound;
        }
        else
        {
            if (can_eat())
                state_id = eStateEat; 
            else
            {
                state_id = eStateRest;
                object->on_activate(); // Летаем дальше, пока голод не упадет
            }
        }

        if (state_id == eStateEat) // Если мы кушаем, проверить дист. до трупа, спустится на землю
        {
            if (object->CorpseMan.get_corpse()->Position().distance_to(object->Position()) < 10.f)
            {
                object->on_deactivate();   // Отключить полет
            }
        }
    }
    else 
       state_id = eStateRest;    // Если тип полтергейста огненный, на землю не спускаемся - летаем, отключается логика 
#else
    state_id = eStateRest; // Дефайн отлючен, запустить логику ЗП полтера
#endif

    if (object->logic_ghost)
    {
        if (enemy)
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong:
            case eWeak:
                if (object->state_invisible == false)
                    state_id = eStateAttack;
                else
                    state_id = eStatePanic;       
                break;
            }
        }
        else if (object->HitMemory.is_hit())
            state_id = eStateHitted;
        else if (object->hear_interesting_sound || object->hear_dangerous_sound)
            state_id = eStateHearDangerousSound;
        else
        {
            if (can_eat())
                state_id = eStateEat;
            else
            {
                state_id = eStateRest;
                object->on_activate(); // Летаем дальше, пока голод не упадет
            }
        }

        if (state_id == eStateEat) // Если мы кушаем, проверить дист. до трупа, спустится на землю
        {
            if (object->CorpseMan.get_corpse()->Position().distance_to(object->Position()) < 10.f)
            {
                object->on_deactivate();   // Отключить полет
            }
        }
    }
    {
        select_state(state_id);
        get_state_current()->execute();
        prev_substate = current_substate;
    }
}

