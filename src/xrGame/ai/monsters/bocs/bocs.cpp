#include "stdafx.h"
#include "bocs.h"
#include "bocs_state_manager.h"
#include "../monster_velocity_space.h"
#include "../control_animation_base.h"
#include "../control_movement_base.h"


CAI_CBocs::CAI_CBocs()
{
	StateMan = xr_new<CStateManagerBocs>(this);
}

CAI_CBocs::~CAI_CBocs()
{
	xr_delete(StateMan);
}

void CAI_CBocs::Load(LPCSTR section)
{
	inherited::Load	(section);

	
	anim().AddReplacedAnim(&m_bDamaged,			eAnimRun,		eAnimRunDamaged);
	anim().AddReplacedAnim(&m_bDamaged,			eAnimWalkFwd,	eAnimWalkDamaged);
	anim().AddReplacedAnim(&m_bRunTurnLeft,		eAnimRun,		eAnimRunTurnLeft);
	anim().AddReplacedAnim(&m_bRunTurnRight,	eAnimRun,		eAnimRunTurnRight);


	anim().accel_load			(section);
	anim().accel_chain_add		(eAnimWalkFwd,		eAnimRun);
	anim().accel_chain_add		(eAnimWalkFwd,		eAnimRunTurnLeft);
	anim().accel_chain_add		(eAnimWalkFwd,		eAnimRunTurnRight);
	anim().accel_chain_add		(eAnimWalkDamaged,	eAnimRunDamaged);

	SVelocityParam &velocity_none		= move().get_velocity(MonsterMovement::eVelocityParameterIdle);	
	SVelocityParam &velocity_turn		= move().get_velocity(MonsterMovement::eVelocityParameterStand);
	SVelocityParam &velocity_walk		= move().get_velocity(MonsterMovement::eVelocityParameterWalkNormal);
	SVelocityParam &velocity_run		= move().get_velocity(MonsterMovement::eVelocityParameterRunNormal);
	SVelocityParam &velocity_walk_dmg	= move().get_velocity(MonsterMovement::eVelocityParameterWalkDamaged);
	SVelocityParam &velocity_run_dmg	= move().get_velocity(MonsterMovement::eVelocityParameterRunDamaged);
	SVelocityParam &velocity_steal		= move().get_velocity(MonsterMovement::eVelocityParameterSteal);
	SVelocityParam &velocity_drag		= move().get_velocity(MonsterMovement::eVelocityParameterDrag);

	anim().AddAnim(eAnimStandIdle,		"stand_idle_",			-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimStandTurnLeft,	"stand_turn_ls_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimStandTurnRight,	"stand_turn_rs_",		-1, &velocity_turn,		PS_STAND);

	anim().AddAnim(eAnimLieIdle,			"lie_sleep_",			-1, &velocity_none,		PS_LIE	);
	anim().AddAnim(eAnimSleep,			"lie_sleep_",			-1, &velocity_none,		PS_LIE	);

	anim().AddAnim(eAnimWalkFwd,			"stand_walk_fwd_",		-1, &velocity_walk,		PS_STAND);
	anim().AddAnim(eAnimWalkDamaged,		"stand_walk_fwd_dmg_",	-1, &velocity_walk_dmg,	PS_STAND);
	anim().AddAnim(eAnimRun,				"stand_run_fwd_",		-1,	&velocity_run,		PS_STAND);
	anim().AddAnim(eAnimRunDamaged,		"stand_run_dmg_",		-1,	&velocity_run_dmg,	PS_STAND);
	anim().AddAnim(eAnimCheckCorpse,		"stand_check_corpse_",	-1,	&velocity_none,		PS_STAND);
	anim().AddAnim(eAnimEat,				"stand_eat_",			-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimAttack,			"stand_attack_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimStandLieDown,	"stand_lie_down_",		-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimLieStandUp,		"lie_stand_up_",		-1, &velocity_none,		PS_LIE	);
	anim().AddAnim(eAnimLieToSleep,		"lie_to_sleep_",		-1, &velocity_none,		PS_LIE	);
	anim().AddAnim(eAnimDragCorpse,		"stand_drag_",			-1, &velocity_drag,		PS_STAND);
	anim().AddAnim(eAnimLookAround,		"stand_idle_",			 2, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimSteal,			"stand_steal_",			-1, &velocity_steal,	PS_STAND);
	anim().AddAnim(eAnimDie,				"stand_idle_",			-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimJumpLeft,		"stand_jump_left_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimJumpRight,		"stand_jump_right_",	-1, &velocity_turn,		PS_STAND);

	anim().AddAnim(eAnimRunTurnLeft,	"stand_run_look_left_",	 -1, &velocity_run,	PS_STAND);
	anim().AddAnim(eAnimRunTurnRight,	"stand_run_look_right_", -1, &velocity_run,	PS_STAND);

																										
	anim().AddTransition(eAnimStandLieDown,	eAnimSleep,		eAnimLieToSleep,		false);										
	anim().AddTransition(PS_STAND,			eAnimSleep,		eAnimStandLieDown,		true);
	anim().AddTransition(PS_STAND,			PS_LIE,			eAnimStandLieDown,		false);
	anim().AddTransition(PS_LIE,				PS_STAND,		eAnimLieStandUp,		false, SKIP_IF_AGGRESSIVE);


	anim().LinkAction(ACT_STAND_IDLE,	eAnimStandIdle);
	anim().LinkAction(ACT_SIT_IDLE,		eAnimLieIdle);
	anim().LinkAction(ACT_LIE_IDLE,		eAnimLieIdle);
	anim().LinkAction(ACT_WALK_FWD,		eAnimWalkFwd);
	anim().LinkAction(ACT_WALK_BKWD,		eAnimDragCorpse);
	anim().LinkAction(ACT_RUN,			eAnimRun);
	anim().LinkAction(ACT_EAT,			eAnimEat);
	anim().LinkAction(ACT_SLEEP,			eAnimSleep);
	anim().LinkAction(ACT_REST,			eAnimLieIdle);
	anim().LinkAction(ACT_DRAG,			eAnimDragCorpse);
	anim().LinkAction(ACT_ATTACK,		eAnimAttack);
	anim().LinkAction(ACT_STEAL,			eAnimSteal);
	anim().LinkAction(ACT_LOOK_AROUND,	eAnimLookAround);

}

void CAI_CBocs::reinit()
{
	inherited::reinit();
	if(CCustomMonster::use_simplified_visual())	return;
	com_man().add_rotation_jump_data("stand_jump_left_0",0,"stand_jump_right_0",0, PI - PI_DIV_6, SControlRotationJumpData::eStopAtOnce | SControlRotationJumpData::eRotateOnce);
}

void CAI_CBocs::CheckSpecParams(u32 spec_params)
{
	if ((spec_params & ASP_CHECK_CORPSE) == ASP_CHECK_CORPSE) {
		com_man().seq_run(anim().get_motion_id(eAnimCheckCorpse));
	}

	if ((spec_params & ASP_STAND_SCARED) == ASP_STAND_SCARED) {
		anim().SetCurAnim(eAnimLookAround);
		return;
	}
}


