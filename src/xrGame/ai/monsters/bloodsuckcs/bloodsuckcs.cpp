#include "stdafx.h"
#include "bloodsuckcs.h"
#include "bloodsuckcs_state_manager.h"
#include "../../../actor.h"
#include "../../../ActorEffector.h"
#include "../../../../Include/xrRender/KinematicsAnimated.h"
#include "../../../level.h"
#include "../../../material_manager.h"
#include "bloodsuckcs_vampire_effector.h"
#include "../../../detail_path_manager.h"
#include "../../../level_debug.h"
#include "../monster_velocity_space.h"
#include "../../../gamepersistent.h"
#include "../../../game_object_space.h"

#include "../control_animation_base.h"
#include "../control_movement_base.h"
#include "../control_rotation_jump.h"

#include "../../../sound_player.h"
#include "../../../../xrEngine/camerabase.h"
#include "../../../xr_level_controller.h"
#include "../../../ActorCondition.h"

#include "../../../PHDestroyable.h"
#include "../../../CharacterPhysicsSupport.h"


namespace detail
{
namespace bloodsuckcs
{
	// default hit settings
	const float default_critical_hit_chance = 0.25f;
	const float default_hit_camera_effector_angle = 0;
	const float default_critical_hit_camera_effector_angle = 3.1415f / 6;

	const float default_camera_effector_move_angular_speed = 1.5f;
	const u32   default_invisibility_activate_delay = 4000;
} 
} 

CAI_Bloodsuckcs::CAI_Bloodsuckcs()
{
	StateMan			= xr_new<CStateManagerBloodsuckcs>(this);
	m_alien_control.init_external	(this);
	m_drag_anim_jump	= false;
	m_animated			= false;
	collision_off		= false;

	using namespace detail::bloodsuckcs;

	m_last_critical_hit_tick             = 0;
	m_critical_hit_chance                = default_critical_hit_chance; 
	m_critical_hit_camera_effector_angle = default_critical_hit_camera_effector_angle;
	m_camera_effector_start_move_tick    = 0;
}

CAI_Bloodsuckcs::~CAI_Bloodsuckcs()
{
	xr_delete	(StateMan);
}

void CAI_Bloodsuckcs::Load(LPCSTR section) 
{
	inherited::Load(section);

	if(pSettings->line_exist(section,"collision_hit_off")){
		collision_hit_off = true;
	}else collision_hit_off = false;
	if(!pSettings->line_exist(section,"is_friendly"))
		com_man().add_ability			(ControlCom::eControlRunAttack);	
	com_man().add_ability			(ControlCom::eControlRotationJump);
	com_man().add_ability			(ControlCom::eControlJump);

	invisible_vel.set				(0.1f, 0.1f);

	EnemyMemory.init_external		(this, 40000);

	anim().AddReplacedAnim(&m_bDamaged,			eAnimRun,		eAnimRunDamaged);
	anim().AddReplacedAnim(&m_bDamaged,			eAnimWalkFwd,	eAnimWalkDamaged);
	anim().AddReplacedAnim(&m_bDamaged,			eAnimStandIdle,	eAnimStandDamaged);
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

	anim().AddAnim(eAnimStandIdle,		"stand_idle_",			-1, &velocity_none,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimStandDamaged,	"stand_damaged_",		-1, &velocity_none,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimStandTurnLeft,	"stand_turn_ls_",		-1, &velocity_turn,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimStandTurnRight,	"stand_turn_rs_",		-1, &velocity_turn,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimSleep,			"lie_sleep_",			-1, &velocity_none,		PS_LIE,	  	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimWalkFwd,		"stand_walk_fwd_",		-1, &velocity_walk,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimWalkDamaged,	"stand_walk_fwd_dmg_",	-1, &velocity_walk_dmg,	PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimRun,			"stand_run_",			-1,	&velocity_run,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimRunDamaged,		"stand_run_dmg_",		-1,	&velocity_run_dmg,	PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

		
	anim().AddAnim(eAnimRunTurnLeft,	"stand_run_turn_left_",	-1, &velocity_run,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimRunTurnRight,	"stand_run_turn_right_",-1, &velocity_run,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimScared,			"stand_scared_",		-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");	

	anim().AddAnim(eAnimCheckCorpse,	"stand_check_corpse_",	-1,	&velocity_none,		PS_STAND, 	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimEat,			"sit_eat_",				-1, &velocity_none,		PS_SIT,		"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		
	anim().AddAnim(eAnimDie,			"stand_idle_",			-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		
	anim().AddAnim(eAnimAttack,			"stand_attack_",		-1, &velocity_turn,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimAttackRun,		"stand_attack_run_",	-1, &velocity_run,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");

	anim().AddAnim(eAnimLookAround,		"stand_look_around_",	-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimSitIdle,		"sit_idle_",			-1, &velocity_none,		PS_SIT,		"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimSitStandUp,		"sit_stand_up_",		-1, &velocity_none,		PS_SIT,		"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimSitToSleep,		"sit_sleep_down_",		-1, &velocity_none,		PS_SIT,		"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimStandSitDown,	"stand_sit_down_",		-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		
	anim().AddAnim(eAnimSteal,			"stand_steal_",			-1, &velocity_steal,	PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
		
	anim().AddAnim(eAnimThreaten,		"stand_threaten_",		-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");
	anim().AddAnim(eAnimMiscAction_00,	"stand_to_aggressive_",	-1, &velocity_none,		PS_STAND,	"fx_run_f", "fx_stand_b", "fx_stand_l", "fx_stand_r");	
	
	// define transitions
	anim().AddTransition(eAnimStandSitDown,	eAnimSleep,		eAnimSitToSleep,	false);
	anim().AddTransition(PS_STAND,			eAnimSleep,		eAnimStandSitDown,	true);
	anim().AddTransition(PS_STAND,			PS_SIT,			eAnimStandSitDown,	false);
	anim().AddTransition(PS_STAND,			PS_LIE,			eAnimStandSitDown,	false);
	anim().AddTransition(PS_SIT,			PS_STAND,		eAnimSitStandUp,	false);
	anim().AddTransition(PS_LIE,			PS_STAND,		eAnimSitStandUp,	false);

	// define links from Action to animations
	anim().LinkAction(ACT_STAND_IDLE,	eAnimStandIdle);
	anim().LinkAction(ACT_SIT_IDLE,		eAnimSitIdle);
	anim().LinkAction(ACT_LIE_IDLE,		eAnimSitIdle);
	anim().LinkAction(ACT_WALK_FWD,		eAnimWalkFwd);
	anim().LinkAction(ACT_WALK_BKWD,	eAnimWalkBkwd);
	anim().LinkAction(ACT_RUN,			eAnimRun);
	anim().LinkAction(ACT_EAT,			eAnimEat);
	anim().LinkAction(ACT_SLEEP,		eAnimSleep);
	anim().LinkAction(ACT_REST,			eAnimSitIdle);
	anim().LinkAction(ACT_ATTACK,		eAnimAttack);
	anim().LinkAction(ACT_STEAL,		eAnimSteal);
	anim().LinkAction(ACT_LOOK_AROUND,	eAnimLookAround); 

	// load other misc stuff
	invisible_vel.set				(pSettings->r_float(section,"Velocity_Invisible_Linear"),pSettings->r_float(section,"Velocity_Invisible_Angular"));
	movement().detail().add_velocity(MonsterMovement::eVelocityParameterInvisible,CDetailPathManager::STravelParams(invisible_vel.linear, invisible_vel.angular));

	invisible_particle_name			= pSettings->r_string(section,"Particle_Invisible");

	using namespace detail::bloodsuckcs;

	READ_IF_EXISTS(pSettings, r_float, section, "separate_factor", 0.f);

	m_critical_hit_chance			     = READ_IF_EXISTS(pSettings, r_float, section, "critical_hit_chance",  
		                                   default_critical_hit_chance);

	m_critical_hit_camera_effector_angle = READ_IF_EXISTS(pSettings, r_float, section, "critical_hit_camera_effector_angle",  
										   default_critical_hit_camera_effector_angle);

	m_camera_effector_move_angular_speed = READ_IF_EXISTS(pSettings, r_float, section, "camera_effector_move_angular_speed",  
										   default_camera_effector_move_angular_speed);

	m_invisibility_activate_delay        = READ_IF_EXISTS(pSettings, r_u32, section, "invisibility_activate_delay",  
										   default_invisibility_activate_delay);
	
}

void CAI_Bloodsuckcs::reinit()
{
	inherited::reinit			();
	CControlledActor::reinit	();
	if(CCustomMonster::use_simplified_visual())	return;

	Bones.Reset					();

	m_alien_control.reinit();
	
	state_invisible				= false;

	com_man().add_rotation_jump_data("run_turn_l_0","run_turn_l_1","run_turn_r_0","run_turn_r_1", PI_DIV_2);
	
	com_man().load_jump_data("boloto_jump_prepare",0, "boloto_jump_fly", "boloto_jump_end", u32(-1), MonsterMovement::eBloodsuckerVelocityParameterJumpGround,0);

	// save visual	
	m_visual_default			= cNameVisual();

	m_predator					= false;
	m_vis_state					= 0;

	start_invisible_predator();
}

void CAI_Bloodsuckcs::reload(LPCSTR section)
{
	inherited::reload(section);
	sound().add(pSettings->r_string(section,"Sound_Invisibility_Change_State"),	DEFAULT_SAMPLE_COUNT,	SOUND_TYPE_MONSTER_ATTACKING, MonsterSound::eNormalPriority,	MonsterSound::eChannelIndependent << 1,	eChangeVisibility,	"bip01_head");
	sound().add(pSettings->r_string(section,"Sound_Growl"),						DEFAULT_SAMPLE_COUNT,	SOUND_TYPE_MONSTER_ATTACKING, MonsterSound::eHighPriority + 6,	MonsterSound::eBaseChannel,	eGrowl,							"bip01_head");
	sound().add(pSettings->r_string(section,"Sound_Alien"),						DEFAULT_SAMPLE_COUNT,	SOUND_TYPE_MONSTER_ATTACKING, MonsterSound::eCriticalPriority,	u32(MonsterSound::eCaptureAllChannels),	eAlien,				"bip01_head");
}

void CAI_Bloodsuckcs::BoneCallback(CBoneInstance *B)
{
	CAI_Bloodsuckcs*	this_class = static_cast<CAI_Bloodsuckcs*> (B->callback_param());

	this_class->Bones.Update(B, Device.dwTimeGlobal);
}

void CAI_Bloodsuckcs::vfAssignBones()
{
	// Установка callback на кости

	bone_spine =	&smart_cast<IKinematics*>(Visual())->LL_GetBoneInstance(smart_cast<IKinematics*>(Visual())->LL_BoneID("bip01_spine"));
	bone_head =		&smart_cast<IKinematics*>(Visual())->LL_GetBoneInstance(smart_cast<IKinematics*>(Visual())->LL_BoneID("bip01_head"));
	if(!PPhysicsShell())//нельзя ставить колбеки, если создан физ шел - у него стоят свои колбеки!!!
	{
		bone_spine->set_callback(bctCustom,BoneCallback,this);
		bone_head->set_callback(bctCustom,BoneCallback,this);
	}

	// Bones settings
	Bones.Reset();
	Bones.AddBone(bone_spine, AXIS_X);	Bones.AddBone(bone_spine, AXIS_Y);
	Bones.AddBone(bone_head, AXIS_X);	Bones.AddBone(bone_head, AXIS_Y);
}

void CAI_Bloodsuckcs::CheckSpecParams(u32 spec_params)
{
	if ((spec_params & ASP_CHECK_CORPSE) == ASP_CHECK_CORPSE) {
		com_man().seq_run(anim().get_motion_id(eAnimCheckCorpse));
	}

	if ((spec_params & ASP_THREATEN) == ASP_THREATEN) {
		anim().SetCurAnim(eAnimThreaten);
		return;
	}

	if ((spec_params & ASP_STAND_SCARED) == ASP_STAND_SCARED) {
		anim().SetCurAnim(eAnimLookAround);
		return;
	}

}

BOOL CAI_Bloodsuckcs::net_Spawn (CSE_Abstract* DC) 
{
	if (!inherited::net_Spawn(DC))
		return(FALSE);

	vfAssignBones();

	return(TRUE);
}

void CAI_Bloodsuckcs::apply_actor_camera_effector()
{
	if ( !m_camera_effector_start_move_tick || 
		  m_camera_effector_last_move_tick >= 
		  m_camera_effector_start_move_tick + m_camera_effector_move_time_milis )
	{
		return;
	}

	if ( !Actor()->cam_Active() ) 
	{
		return;
	}

	const TTime current_tick   = _min(time(), m_camera_effector_start_move_tick + 
		                                      m_camera_effector_move_time_milis);

	const float relative_time1 = (m_camera_effector_last_move_tick - m_camera_effector_start_move_tick)
		                                                      /
		                                     (float)m_camera_effector_move_time_milis;

	const float relative_time2 = (current_tick - m_camera_effector_start_move_tick)
		                                           /
		                         (float)m_camera_effector_move_time_milis;

	m_camera_effector_last_move_tick = current_tick;

	const float angle_relative = (relative_time2-relative_time1) * (2-relative_time1-relative_time2);

	Actor()->cam_Active()->Move(m_camera_effector_hor_angle > 0 ? kRIGHT : kLEFT, 
		                        _abs(m_camera_effector_hor_angle)*angle_relative);

	Actor()->cam_Active()->Move(m_camera_effector_ver_angle > 0 ? kUP	 : kDOWN, 
		                        _abs(m_camera_effector_ver_angle)*angle_relative);
}

void CAI_Bloodsuckcs::UpdateCL()
{
	inherited::UpdateCL				();
	CControlledActor::frame_update	();
	character_physics_support()->movement()->CollisionEnable(!is_collision_off());
}

void CAI_Bloodsuckcs::shedule_Update(u32 dt)
{
	inherited::shedule_Update(dt);
	
	if (!g_Alive())	
	{
		setVisible(TRUE);
		if ( state_invisible )
		{
			stop_invisible_predator();
		}
	}

	if (m_alien_control.active())	sound().play(eAlien);
}

void CAI_Bloodsuckcs::Die(CObject* who)
{
	inherited::Die(who);
	stop_invisible_predator();
}

void CAI_Bloodsuckcs::post_fsm_update()
{
	inherited::post_fsm_update();
}

bool CAI_Bloodsuckcs::check_start_conditions(ControlCom::EControlType type)
{
	if ( type == ControlCom::eControlJump )
	{
		return false;
	}

	if ( !inherited::check_start_conditions(type) )
	{
		return false;
	}

	if ( type == ControlCom::eControlRunAttack )
	{
		return !state_invisible;
	}

	return true;
}

void CAI_Bloodsuckcs::set_alien_control(bool val)
{
	val ? m_alien_control.activate() : m_alien_control.deactivate();
}

void CAI_Bloodsuckcs::set_vis()
{
	m_vis_state = 1;
	predator_stop();
}

void CAI_Bloodsuckcs::set_invis()
{
	m_vis_state = -1;
	predator_start();
}

void CAI_Bloodsuckcs::set_collision_off(bool b_collision)
{
	collision_off = b_collision;
}


bool CAI_Bloodsuckcs::is_collision_off()
{
	return collision_off;
}
void CAI_Bloodsuckcs::jump(const Fvector &position, float factor)
{
	com_man().script_jump	(position, factor);
	sound().play			(MonsterSound::eMonsterSoundAggressive);
}

void CAI_Bloodsuckcs::set_drag_jump(CEntityAlive* e, LPCSTR s, const Fvector &position, float factor)
{
	j_position = position;
	j_factor = factor;
	m_cob = e;
	m_str_cel = s;
	m_drag_anim_jump = true;
	m_animated = true;
}
bool CAI_Bloodsuckcs::is_drag_anim_jump()
{
	return m_drag_anim_jump;
}

bool CAI_Bloodsuckcs::is_animated()
{
	return m_animated;
}

void CAI_Bloodsuckcs::start_drag()
{
	if(m_animated){
		com_man().script_capture(ControlCom::eControlAnimation);
		smart_cast<IKinematicsAnimated*>(Visual())->PlayCycle("boloto_attack_link_bone",TRUE,animation_end_jump,this);
		m_animated = false;
	}
}

void CAI_Bloodsuckcs::animation_end_jump(CBlend* B)
{
	((CAI_Bloodsuckcs*)B->CallbackParam)->set_invis();
	((CAI_Bloodsuckcs*)B->CallbackParam)->jump(((CAI_Bloodsuckcs*)B->CallbackParam)->j_position, ((CAI_Bloodsuckcs*)B->CallbackParam)->j_factor);
}

void CAI_Bloodsuckcs::predator_start()
{
	if( m_vis_state!=0 )
	{
		if( m_vis_state==1 )
		{
			return;
		}
		m_predator = false;
	}
	if (m_predator)					return;

	cNameVisual_set(m_visual_predator);
	CDamageManager::reload(*cNameSect(),"damage",pSettings);

	control().animation().restart	();
	
	CParticlesPlayer::StartParticles(invisible_particle_name,Fvector().set(0.0f,0.1f,0.0f),ID());		
	sound().play					(CAI_Bloodsuckcs::eChangeVisibility);

	m_predator						= true;
}

void CAI_Bloodsuckcs::predator_stop()
{
	if( m_vis_state != 0 )
	{
		if( m_vis_state == -1)
		{
			return;
		}

		m_predator = true;
	}

	if ( !m_predator )
	{
		return;
	}
	
	cNameVisual_set(*m_visual_default);
	character_physics_support()->in_ChangeVisual();

	CDamageManager::reload(*cNameSect(),"damage",pSettings);

	control().animation().restart	();
	
	CParticlesPlayer::StartParticles(invisible_particle_name,Fvector().set(0.0f,0.1f,0.0f),ID());		
	sound().play					(CAI_Bloodsuckcs::eChangeVisibility);
	m_predator						= false;
}

void CAI_Bloodsuckcs::predator_freeze()
{
	control().animation().freeze	();
}

void CAI_Bloodsuckcs::predator_unfreeze()
{
	control().animation().unfreeze();
}

void CAI_Bloodsuckcs::move_actor_cam (float angle)
{
	if ( Actor()->cam_Active() ) 
	{
		Actor()->cam_Active()->Move(Random.randI(2) ? kRIGHT : kLEFT, angle);
		Actor()->cam_Active()->Move(Random.randI(2) ? kUP	 : kDOWN, angle);
	}
}

void CAI_Bloodsuckcs::HitEntity(const CEntity *pEntity, float fDamage, float impulse, Fvector &dir)
{
	bool is_critical = rand()/(float)RAND_MAX <= m_critical_hit_chance;

	if ( is_critical )
	{
		impulse *= 10.f;

		if ( pEntity == Actor() )
		{
			m_last_critical_hit_tick          = 
			m_camera_effector_start_move_tick = 
			m_camera_effector_last_move_tick  = time();

			m_camera_effector_move_time_milis = 
				(u32)(1000 * (m_critical_hit_camera_effector_angle / m_camera_effector_move_angular_speed));

			m_camera_effector_hor_angle = (rand()%2 ? 1 : -1) * m_critical_hit_camera_effector_angle;
			m_camera_effector_ver_angle = (rand()%2 ? 1 : -1) * m_critical_hit_camera_effector_angle;
		}
	}

	inherited::HitEntity(pEntity, fDamage, impulse, dir);
}

bool CAI_Bloodsuckcs::in_solid_state ()
{
	return !state_invisible;
}

void CAI_Bloodsuckcs::Hit(SHit* pHDS)
{
	if ( state_invisible )
	{
		return;
	}
	if ( !collision_hit_off )
	{
		inherited::Hit(pHDS);
	}
}

void CAI_Bloodsuckcs::start_invisible_predator()
{
	state_invisible	= true;
	predator_start();
}
void CAI_Bloodsuckcs::stop_invisible_predator()
{
	state_invisible	= false;
	predator_stop();
}

void CAI_Bloodsuckcs::manual_activate()
{
	state_invisible = true;
	setVisible		(FALSE);
}

void CAI_Bloodsuckcs::manual_deactivate()
{
	state_invisible = false;
	setVisible		(TRUE);
}



