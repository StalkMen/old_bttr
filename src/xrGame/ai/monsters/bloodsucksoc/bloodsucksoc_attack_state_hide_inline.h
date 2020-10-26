#pragma once

#include "../states/state_move_to_point.h"
#include "bloodsucksoc_predator_lite.h"

#include "../../../cover_point.h"
#include "../monster_cover_manager.h"
#include "../monster_home.h"

#include "../../../actor.h"
#include "../../../actor_memory.h"
#include "../../../visual_memory_manager.h"

#define CBloodsucksocStateAttackHideAbstract CBloodsucksocStateAttackHide<_Object>

template <typename _Object>
CBloodsucksocStateAttackHideAbstract::CBloodsucksocStateAttackHide(_Object *obj) : inherited(obj)
{
	add_state	(eStateAttack_HideInCover, xr_new<CStateMonsterMoveToPointEx<_Object>> (obj));
	add_state	(eStateAttack_CampInCover, xr_new<CStateBloodsucksocPredatorLite<_Object>>(obj));
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::reinit()
{
	inherited::reinit	();
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::initialize()
{
	inherited::initialize	();

	m_target_node			= u32(-1);

	object->start_invisible_predator();
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::reselect_state()
{
	if (prev_substate == u32(-1)) {
		select_state(eStateAttack_HideInCover);
		return;
	}

	select_state(eStateAttack_CampInCover);
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::finalize()
{
	inherited::finalize							();

	if (m_target_node != u32(-1))
		monster_squad().get_squad(object)->unlock_cover(m_target_node);
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::critical_finalize()
{
	inherited::critical_finalize				();

	if (m_target_node != u32(-1))
		monster_squad().get_squad(object)->unlock_cover(m_target_node);
}


template <typename _Object>
bool CBloodsucksocStateAttackHideAbstract::check_completion()
{
	if (current_substate == eStateAttack_CampInCover)
		return (get_state_current()->check_completion());

	return false;
}


template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::setup_substates()
{
	state_ptr state = get_state_current();

	if (current_substate == eStateAttack_HideInCover) {
		select_camp_point();

		SStateDataMoveToPointEx data;

		data.vertex				= m_target_node;
		data.point				= ai().level_graph().vertex_position(data.vertex);
		data.action.action		= ACT_RUN;
		data.action.time_out	= 0;		// do not use time out
		data.completion_dist	= 0.f;		// get exactly to the point
		data.time_to_rebuild	= 0;		// do not rebuild
		data.accelerated		= true;
		data.braking			= true;
		data.accel_type 		= eAT_Aggressive;
		data.action.sound_type	= MonsterSound::eMonsterSoundIdle;
		data.action.sound_delay = object->db().m_dwIdleSndDelay;

		state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
		return;
	}

}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::check_force_state()
{
}

template <typename _Object>
void CBloodsucksocStateAttackHideAbstract::select_camp_point()
{
	if (m_target_node != u32(-1))
		monster_squad().get_squad(object)->unlock_cover(m_target_node);

	m_target_node = u32(-1);
	if (object->Home->has_home()) {
		m_target_node							= object->Home->get_place_in_cover();
		if (m_target_node == u32(-1)) {
			m_target_node						= object->Home->get_place();
		}
	} 

	if (m_target_node == u32(-1)) {
		const CCoverPoint	*point = object->CoverMan->find_cover(object->Position(),10.f,30.f);
		if (point) {
			m_target_node				= point->level_vertex_id	();
		} 
	}

	if (m_target_node == u32(-1)) 
		m_target_node = object->ai_location().level_vertex_id();


	monster_squad().get_squad(object)->lock_cover(m_target_node);
}


#undef CBloodsucksocStateAttackHideAbstract

