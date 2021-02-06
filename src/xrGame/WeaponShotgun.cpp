#include "stdafx.h"
#include "weaponshotgun.h"
#include "entity.h"
#include "ParticlesObject.h"
#include "xr_level_controller.h"
#include "inventory.h"
#include "level.h"
#include "actor.h"

extern int g_sprint_reload_wpn;

CWeaponShotgun::CWeaponShotgun()
{
	m_eSoundClose			= ESoundTypes(SOUND_TYPE_WEAPON_SHOOTING);
	m_eSoundAddCartridge	= ESoundTypes(SOUND_TYPE_WEAPON_SHOOTING);
}

CWeaponShotgun::~CWeaponShotgun()
{
}

void CWeaponShotgun::net_Destroy()
{
	inherited::net_Destroy();
}

void CWeaponShotgun::Load	(LPCSTR section)
{
	inherited::Load		(section);

	if(pSettings->line_exist(section, "tri_state_reload")){
		m_bTriStateReload = !!pSettings->r_bool(section, "tri_state_reload");
	};
	if(m_bTriStateReload){
		m_sounds.LoadSound(section, "snd_open_weapon", "sndOpen", false, m_eSoundOpen);

		m_sounds.LoadSound(section, "snd_add_cartridge", "sndAddCartridge", false, m_eSoundAddCartridge);

		m_sounds.LoadSound(section, "snd_close_weapon", "sndClose", false, m_eSoundClose);
		m_sounds.LoadSound(section, "snd_close_weapon_empty", "sndCloseEmpty", false, m_eSoundClose);
	};

}

void CWeaponShotgun::switch2_Fire	()
{
	inherited::switch2_Fire	();
	bWorking = false;
}


bool CWeaponShotgun::Action			(u16 cmd, u32 flags) 
{
	if(inherited::Action(cmd, flags)) return true;

	if(	m_bTriStateReload && GetState()==eReload &&
		cmd==kWPN_FIRE && flags&CMD_START &&
		m_sub_state==eSubstateReloadInProcess		)//остановить перезагрузку
	{
		AddCartridge(1);
		m_sub_state = eSubstateReloadEnd;
		return true;
	}
	return false;
}

void CWeaponShotgun::OnAnimationEnd(u32 state) 
{
	if(!m_bTriStateReload || state != eReload)
		return inherited::OnAnimationEnd(state);

	switch(m_sub_state){
		case eSubstateReloadBegin:{
			m_sub_state = eSubstateReloadInProcess;
			SwitchState(eReload);
		}break;

		case eSubstateReloadInProcess:{
			if( 0 != AddCartridge(1) ){
				m_sub_state = eSubstateReloadEnd;
			}
			SwitchState(eReload);
		}break;

		case eSubstateReloadEnd:{
			m_sub_state = eSubstateReloadBegin;
			SwitchState(eIdle);

			if (g_sprint_reload_wpn && smart_cast<CActor*>(H_Parent()) != NULL)
				Actor()->SetCantRunState(false); 
		}break;
		
	};
}

void CWeaponShotgun::Reload() 
{
	if(m_bTriStateReload){
		TriStateReload();
	}else
		inherited::Reload();
}

void CWeaponShotgun::TriStateReload()
{
	if( m_magazine.size() == (u32)iMagazineSize ||  !HaveCartridgeInInventory(1) )return;
	CWeapon::Reload		();
	m_sub_state			= eSubstateReloadBegin;
	SwitchState			(eReload);

	if (g_sprint_reload_wpn && smart_cast<CActor*>(H_Parent()) != NULL)
		Actor()->SetCantRunState(true);
}

void CWeaponShotgun::OnStateSwitch	(u32 S, u32 oldState)
{
	if(!m_bTriStateReload || S != eReload){
		inherited::OnStateSwitch(S, oldState);
		return;
	}

	CWeapon::OnStateSwitch(S, oldState);

	if( m_magazine.size() == (u32)iMagazineSize || !HaveCartridgeInInventory(1) ){
			switch2_EndReload		();
			m_sub_state = eSubstateReloadEnd;
			return;
	};

	switch (m_sub_state)
	{
	case eSubstateReloadBegin:
		if( HaveCartridgeInInventory(1) )
			switch2_StartReload	();
		break;
	case eSubstateReloadInProcess:
			if( HaveCartridgeInInventory(1) )
				switch2_AddCartgidge	();
		break;
	case eSubstateReloadEnd:
			switch2_EndReload		();
		break;
	};
}

void CWeaponShotgun::switch2_StartReload()
{
	PlaySound			("sndOpen",get_LastFP());
	PlayAnimOpenWeapon	();
	SetPending			(TRUE);
}

void CWeaponShotgun::switch2_AddCartgidge	()
{
	PlaySound	("sndAddCartridge",get_LastFP());
	PlayAnimAddOneCartridgeWeapon();
	SetPending			(TRUE);
}

void CWeaponShotgun::switch2_EndReload	()
{
	SetPending			(FALSE);
	if (m_ammoElapsed.type1 == 0)
		PlaySound("sndCloseEmpty",get_LastFP());
	else
		PlaySound("sndClose", get_LastFP());
	PlayAnimCloseWeapon	();
}

void CWeaponShotgun::PlayAnimOpenWeapon()
{
	VERIFY(GetState()==eReload);
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_open_empty", FALSE, this, GetState());
	else
		PlayHUDMotion("anm_open", FALSE, this, GetState());

	if (g_sprint_reload_wpn && smart_cast<CActor*>(H_Parent()) != NULL && Actor()->BlockCounter() > 0 || Actor()->BlockCounter() <= 0)
		Actor()->BlockCounterSet(0);
}
void CWeaponShotgun::PlayAnimAddOneCartridgeWeapon()
{
	VERIFY(GetState()==eReload);
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_add_cartridge_empty",FALSE,this,GetState());
	else
		PlayHUDMotion("anm_add_cartridge", FALSE, this, GetState());
}
void CWeaponShotgun::PlayAnimCloseWeapon()
{
	VERIFY(GetState()==eReload);

	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_close_empty",FALSE,this,GetState());
	else
		PlayHUDMotion("anm_close", FALSE, this, GetState());
}

bool CWeaponShotgun::HaveCartridgeInInventory(u8 cnt)
{
	if (unlimited_ammo())	return true;
	if(!m_pInventory)		return false;

	u32 ac = GetAmmoCount(m_ammoType.type1);
	if(ac<cnt)
	{
		for(u8 i = 0; i < u8(m_ammoTypes.size()); ++i) 
		{
			if(m_ammoType.type1==i) continue;
			ac	+= GetAmmoCount(i);
			if(ac >= cnt)
			{
				m_ammoType.type1 = i;
				break; 
			}
		}
	}
	return ac>=cnt;
}

u8 CWeaponShotgun::AddCartridge		(u8 cnt)
{
	if(IsMisfire())	bMisfire = false;

	if ( m_set_next_ammoType_on_reload != undefined_ammo_type )
	{
		m_ammoType.type1 = m_set_next_ammoType_on_reload;
		m_set_next_ammoType_on_reload	= undefined_ammo_type;
	}

	if( !HaveCartridgeInInventory(1) )
		return 0;

	m_pCurrentAmmo = smart_cast<CWeaponAmmo*>(m_pInventory->GetAny( m_ammoTypes[m_ammoType.type1].c_str() ));
	VERIFY((u32)m_ammoElapsed.type1 == m_magazine.size());

	if (m_DefaultCartridge.m_LocalAmmoType != m_ammoType.type1)
		m_DefaultCartridge.Load(m_ammoTypes[m_ammoType.type1].c_str(), m_ammoType.type1, m_APk);

	CCartridge l_cartridge = m_DefaultCartridge;
	while(cnt)
	{
		if (!unlimited_ammo())
		{
			if (!m_pCurrentAmmo->Get(l_cartridge)) break;
		}
		--cnt;
		++m_ammoElapsed.type1;
		l_cartridge.m_LocalAmmoType = m_ammoType.type1;
		m_magazine.push_back(l_cartridge);
//		m_fCurrentCartirdgeDisp = l_cartridge.m_kDisp;
	}

	VERIFY((u32)m_ammoElapsed.type1 == m_magazine.size());

	//выкинуть коробку патронов, если она пустая
	if(m_pCurrentAmmo && !m_pCurrentAmmo->m_boxCurr && OnServer()) 
		m_pCurrentAmmo->SetDropManual(TRUE);

	return cnt;
}

void	CWeaponShotgun::net_Export	(NET_Packet& P)
{
	inherited::net_Export(P);	
	P.w_u8(u8(m_magazine.size()));	
	for (u32 i=0; i<m_magazine.size(); i++)
	{
		CCartridge& l_cartridge = *(m_magazine.begin()+i);
		P.w_u8(l_cartridge.m_LocalAmmoType);
	}
}

void	CWeaponShotgun::net_Import	(NET_Packet& P)
{
	inherited::net_Import(P);	
	u8 AmmoCount = P.r_u8();
	for (u32 i=0; i<AmmoCount; i++)
	{
		u8 LocalAmmoType = P.r_u8();
		if (i>=m_magazine.size()) continue;
		CCartridge& l_cartridge = *(m_magazine.begin()+i);
		if (LocalAmmoType == l_cartridge.m_LocalAmmoType) continue;
#ifdef DEBUG
		Msg("! %s reload to %s", *l_cartridge.m_ammoSect, m_ammoTypes[LocalAmmoType].c_str());
#endif
		l_cartridge.Load( m_ammoTypes[LocalAmmoType].c_str(), LocalAmmoType, m_APk );
	}
}

void CWeaponShotgun::PlayAnimShow()
{
	VERIFY(GetState() == eShowing);

	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_show_empty", FALSE, this, GetState());
	else
		inherited::PlayAnimShow();
}

void CWeaponShotgun::PlayAnimBore()
{
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_bore_empty", TRUE, this, GetState());
	else
		inherited::PlayAnimBore();
}

void CWeaponShotgun::PlayAnimIdleSprint()
{
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_idle_sprint_empty", TRUE, NULL, GetState());
	else
		inherited::PlayAnimIdleSprint();
}

void CWeaponShotgun::PlayAnimIdleMoving()
{
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_idle_moving_empty", TRUE, NULL, GetState());
	else
		inherited::PlayAnimIdleMoving();
}

void CWeaponShotgun::PlayAnimIdle()
{
	if (TryPlayAnimIdle()) return;

	if (IsZoomed())
		PlayAnimAim();
	else
	{
		if (m_ammoElapsed.type1 == 0)
			PlayHUDMotion("anm_idle_empty", TRUE, NULL, GetState());
		else
			inherited::PlayAnimIdle();
	}
}

void CWeaponShotgun::PlayAnimCrouchIdleMoving()
{
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_idle_moving_crouch_empty", TRUE, NULL, GetState());
	else
		PlayHUDMotion("anm_idle_moving_crouch", TRUE, NULL, GetState());
}

void CWeaponShotgun::PlayAnimMovingSlow()
{
	if (m_ammoElapsed.type1 == 0)
		PlayHUDMotion("anm_idle_moving_slow_empty", TRUE, NULL, GetState());
	else
		PlayHUDMotion("anm_idle_moving_slow", TRUE, NULL, GetState());
}

void CWeaponShotgun::PlayAnimAim()
{
	if (m_ammoElapsed.type1 == 0)
	{
		CActor* pActor = smart_cast<CActor*>(H_Parent());
		if (pActor)
		{
			CEntity::SEntityState st;
			pActor->g_State(st);
			if (pActor->AnyMove())
			{
				if (HudAnimationExist("anm_idle_aim_moving_empty"))
					PlayHUDMotion("anm_idle_aim_moving_empty", TRUE, NULL, GetState());
				else
					PlayHUDMotion("anm_idle_aim_empty", TRUE, NULL, GetState());
			}
			else
				PlayHUDMotion("anm_idle_aim_empty", TRUE, NULL, GetState());
		}
	}
	else
		inherited::PlayAnimAim();
}

void CWeaponShotgun::PlayAnimHide()
{
	VERIFY(GetState() == eHiding);
	if (m_ammoElapsed.type1 == 0)
	{
		PlaySound("sndClose", get_LastFP());
		PlayHUDMotion("anm_hide_empty", TRUE, this, GetState());
	}
	else
		inherited::PlayAnimHide();
}

void CWeaponShotgun::PlayAnimShoot()
{
	VERIFY(GetState() == eFire);
	if (m_ammoElapsed.type1 > 1)
		inherited::PlayAnimShoot();
	else
	{
		if (IsZoomed())
		{
			if (HudAnimationExist("anm_shot_l_when_aim"))
				PlayHUDMotion("anm_shot_l_when_aim", FALSE, this, GetState());
			else
				PlayHUDMotion("anm_shot_l", FALSE, this, GetState());
		}
		else
			PlayHUDMotion("anm_shot_l", FALSE, this, GetState());
	}
}