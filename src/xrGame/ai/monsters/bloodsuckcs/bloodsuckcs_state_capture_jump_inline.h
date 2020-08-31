#pragma once

#include "../states/state_custom_action.h"

#define CStateCaptureJumpBloodsuckcsAbstract CStateCaptureJumpBloodsuckcs<_Object>


template <typename _Object>
CStateCaptureJumpBloodsuckcsAbstract::CStateCaptureJumpBloodsuckcs(_Object *obj) : inherited(obj)
{
	add_state(eStateCustom,				xr_new<CStateMonsterCustomAction<_Object>>		(obj));
}

template <typename _Object>
CStateCaptureJumpBloodsuckcsAbstract::~CStateCaptureJumpBloodsuckcs	()
{
}

template <typename _Object>
void CStateCaptureJumpBloodsuckcsAbstract::execute()
{
	// check alife control
	select_state	(eStateCustom);

	get_state_current()->execute();
	prev_substate = current_substate;
}
template <typename _Object>
void CStateCaptureJumpBloodsuckcsAbstract::setup_substates()
{
	state_ptr state = get_state_current();
	if (current_substate == eStateCustom) {
		SStateDataAction data;

		data.action		= ACT_STAND_IDLE;
		data.time_out	= 0;			// do not use time out
		state->fill_data_with(&data, sizeof(SStateDataAction));

		return;
	}
}

#undef CStateCaptureJumpBloodsuckcsAbstract
