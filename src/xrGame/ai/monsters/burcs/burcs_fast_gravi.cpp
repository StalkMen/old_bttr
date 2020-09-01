#include "stdafx.h"
#include "Burcs_fast_gravi.h"
#include "Burcs.h"
#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"

bool CBurcsFastGravi::check_start_conditions()
{
	if (is_active())				return false;	
	if (m_man->is_captured_pure())	return false;
	if (!m_object->EnemyMan.get_enemy()) return false;
	
	return true;
}

void CBurcsFastGravi::activate()
{
	CBurcs *Burcs = smart_cast<CBurcs *>(m_object);
	m_man->subscribe	(this, ControlCom::eventTAChange);
	m_object->com_man().ta_activate(Burcs->anim_triple_gravi);

	m_object->dir().face_target(m_object->EnemyMan.get_enemy());
}

void CBurcsFastGravi::deactivate()
{
	m_man->unsubscribe					(this, ControlCom::eventTAChange);
}

void CBurcsFastGravi::on_event(ControlCom::EEventType type, ControlCom::IEventData *data)
{
	if (type == ControlCom::eventTAChange) {
		STripleAnimEventData *event_data = (STripleAnimEventData *)data;
		if (event_data->m_current_state == eStateExecute) {
			process_hit();
			m_object->com_man().ta_pointbreak();
			m_man->deactivate	(this);
		}
	}
}

void CBurcsFastGravi::process_hit()
{
	m_object->HitEntity(m_object->EnemyMan.get_enemy(), 1.f, 100.f, m_object->Direction());
}

