#include "stdafx.h"
#include "dogsoc.h"
#include "dogsoc_state_manager.h"
#include "../monster_velocity_space.h"
#include "../control_animation_base.h"
#include "../control_movement_base.h"

CAI_Dogsoc::CAI_Dogsoc()
{
	StateMan = xr_new<CStateManagerdogsoc>(this);
	
	CControlled::init_external	(this);

	com_man().add_ability(ControlCom::eControlRotationJump);
	com_man().add_ability(ControlCom::eControlMeleeJump);
}

CAI_Dogsoc::~CAI_Dogsoc()
{
	xr_delete(StateMan);
}

void CAI_Dogsoc::Load(LPCSTR section)
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

	// define animation set
	anim().AddAnim(eAnimStandIdle,		"stand_idle_",			-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimStandTurnLeft,	"stand_turn_ls_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimStandTurnRight,	"stand_turn_rs_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimEat,			"stand_eat_",			-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimSleep,			"lie_sleep_",			-1, &velocity_none,		PS_LIE);
	anim().AddAnim(eAnimLieIdle,		"lie_idle_",			-1, &velocity_none,		PS_LIE);
	anim().AddAnim(eAnimSitIdle,		"sit_idle_",			-1, &velocity_none,		PS_SIT);
	anim().AddAnim(eAnimAttack,			"stand_attack_",		-1, &velocity_turn,		PS_STAND);
	anim().AddAnim(eAnimWalkFwd,		"stand_walk_fwd_",		-1, &velocity_walk,		PS_STAND);
	anim().AddAnim(eAnimWalkDamaged,	"stand_walk_dmg_",		-1, &velocity_walk_dmg,	PS_STAND);
	anim().AddAnim(eAnimRun,			"stand_run_",			-1,	&velocity_run,		PS_STAND);
	anim().AddAnim(eAnimRunDamaged,		"stand_run_dmg_",		-1, &velocity_run_dmg,	PS_STAND);

	anim().AddAnim(eAnimRunTurnLeft,	"stand_run_turn_left_",	-1, &velocity_run,		PS_STAND);
	anim().AddAnim(eAnimRunTurnRight,	"stand_run_turn_right_",-1, &velocity_run,		PS_STAND);

	anim().AddAnim(eAnimCheckCorpse,	"stand_check_corpse_",	-1,	&velocity_none,		PS_STAND);
	anim().AddAnim(eAnimDragCorpse,		"stand_drag_",			-1, &velocity_drag,		PS_STAND);
	anim().AddAnim(eAnimSteal,			"stand_steal_",			-1, &velocity_steal,	PS_STAND);
	anim().AddAnim(eAnimThreaten,		"stand_threaten_",		-1, &velocity_none,		PS_STAND);

	anim().AddAnim(eAnimSitLieDown,		"sit_lie_down_",		-1, &velocity_none,		PS_SIT);
	anim().AddAnim(eAnimStandSitDown,	"stand_sit_down_",		-1, &velocity_none,		PS_STAND);	
	anim().AddAnim(eAnimSitStandUp,		"sit_stand_up_",		-1, &velocity_none,		PS_SIT);
	anim().AddAnim(eAnimLieSitUp,		"lie_to_sit_",		-1, &velocity_none,		PS_LIE);

	anim().AddAnim(eAnimJumpLeft,		"stand_jump_left_",		-1, &velocity_none,		PS_STAND);
	anim().AddAnim(eAnimJumpRight,		"stand_jump_right_",	-1, &velocity_none,		PS_STAND);

	anim().AddTransition(PS_SIT,		PS_LIE,		eAnimSitLieDown,		false);
	anim().AddTransition(PS_STAND,		PS_SIT,		eAnimStandSitDown,		false);
	anim().AddTransition(PS_SIT,		PS_STAND,	eAnimSitStandUp,		false, SKIP_IF_AGGRESSIVE);
	anim().AddTransition(PS_LIE,		PS_SIT,		eAnimLieSitUp,			false, SKIP_IF_AGGRESSIVE);

	anim().LinkAction(ACT_STAND_IDLE,	eAnimStandIdle);
	anim().LinkAction(ACT_SIT_IDLE,		eAnimSitIdle);
	anim().LinkAction(ACT_LIE_IDLE,		eAnimLieIdle);
	anim().LinkAction(ACT_WALK_FWD,		eAnimWalkFwd);
	anim().LinkAction(ACT_WALK_BKWD,	eAnimWalkFwd);
	anim().LinkAction(ACT_RUN,			eAnimRun); 
	anim().LinkAction(ACT_EAT,			eAnimEat);
	anim().LinkAction(ACT_SLEEP,		eAnimSleep);
	anim().LinkAction(ACT_REST,			eAnimSitIdle);
	anim().LinkAction(ACT_DRAG,			eAnimDragCorpse);
	anim().LinkAction(ACT_ATTACK,		eAnimAttack);
	anim().LinkAction(ACT_STEAL,		eAnimSteal);	
	anim().LinkAction(ACT_LOOK_AROUND,	eAnimStandIdle);

}

void CAI_Dogsoc::reinit()
{
	inherited::reinit();

	if(CCustomMonster::use_simplified_visual())	return;

	com_man().add_rotation_jump_data("1","2","3","4", PI_DIV_2);
	com_man().add_rotation_jump_data("5","6","7","8", deg(179));
	com_man().add_melee_jump_data("5","jump_right_0");
}


void CAI_Dogsoc::CheckSpecParams(u32 spec_params)
{
	if ((spec_params & ASP_CHECK_CORPSE) == ASP_CHECK_CORPSE) {
		com_man().seq_run(anim().get_motion_id(eAnimCheckCorpse));
	}
	
	if ((spec_params & ASP_THREATEN) == ASP_THREATEN) {
		anim().SetCurAnim(eAnimThreaten);
	}
}
