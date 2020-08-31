#pragma once

#define CStateBloodsuckerVampireApproachAbstract CStateBloodsucksocVampireApproach<_Object>

template <typename _Object>
CStateBloodsuckerVampireApproachAbstract::CStateBloodsucksocVampireApproach(_Object* obj) : inherited(obj) {}
template <typename _Object>
CStateBloodsuckerVampireApproachAbstract::~CStateBloodsucksocVampireApproach() {}
template <typename _Object>
void CStateBloodsuckerVampireApproachAbstract::initialize()
{
    inherited::initialize();
    object->path().prepare_builder();
}

template <typename _Object>
void CStateBloodsuckerVampireApproachAbstract::execute()
{
    // установка параметров функциональных блоков
    object->set_action(ACT_RUN);
    object->anim().accel_activate(eAT_Aggressive);
    object->anim().accel_set_braking(false);

    u32 const target_vertex = object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
    Fvector const target_pos = ai().level_graph().vertex_position(target_vertex);

    object->path().set_target_point(target_pos, target_vertex);
    object->path().set_rebuild_time(object->get_attack_rebuild_time());
    object->path().set_use_covers(false);
    object->path().set_distance_to_end(0.1f);
    object->set_state_sound(MonsterSound::eMonsterSoundAggressive);
}