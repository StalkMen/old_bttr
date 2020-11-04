#include "stdafx.h"
#include "weaponpistol.h"
#include "ParticlesObject.h"
#include "actor.h"

CWeaponPistol::CWeaponPistol()
{
    m_eSoundClose = ESoundTypes(SOUND_TYPE_WEAPON_RECHARGING);
    SetPending(FALSE);
}

CWeaponPistol::~CWeaponPistol(void)
{}

void CWeaponPistol::net_Destroy()
{
    inherited::net_Destroy();
}

void CWeaponPistol::Load(LPCSTR section)
{
    inherited::Load(section);

    m_sounds.LoadSound(section, "snd_close", "sndClose", false, m_eSoundClose);
}

void CWeaponPistol::OnH_B_Chield()
{
    inherited::OnH_B_Chield();
}

void CWeaponPistol::PlayAnimShow()
{
    VERIFY(GetState() == eShowing);

    if (m_ammoElapsed.type1 == 0)
        PlayHUDMotion("anm_show_empty", FALSE, this, GetState());
    else
        inherited::PlayAnimShow();
}

void CWeaponPistol::PlayAnimBore()
{
    if (m_ammoElapsed.type1 == 0)
        PlayHUDMotion("anm_bore_empty", TRUE, this, GetState());
    else
        inherited::PlayAnimBore();
}

void CWeaponPistol::PlayAnimIdleSprint()
{
    if (m_ammoElapsed.type1 == 0)
    {
        PlayHUDMotion("anm_idle_sprint_empty", TRUE, NULL, GetState());
    }
    else
    {
        inherited::PlayAnimIdleSprint();
    }
}

void CWeaponPistol::PlayAnimIdleMoving()
{
    if (m_ammoElapsed.type1 == 0)
    {
        PlayHUDMotion("anm_idle_moving_empty", TRUE, NULL, GetState());
    }
    else
    {
        inherited::PlayAnimIdleMoving();
    }
}

void CWeaponPistol::PlayAnimIdle()
{
    if (TryPlayAnimIdle()) return;

    if (m_ammoElapsed.type1 == 0)
    {
        PlayHUDMotion("anm_idle_empty", TRUE, NULL, GetState());
    }
    else
    {
        inherited::PlayAnimIdle();
    }
}

void CWeaponPistol::PlayAnimCrouchIdleMoving()
{
    if (m_ammoElapsed.type1 == 0)
        PlayHUDMotion("anm_idle_moving_crouch_empty", TRUE, NULL, GetState());
    else
        PlayHUDMotion("anm_idle_moving_crouch", TRUE, NULL, GetState());
}

void CWeaponPistol::PlayAnimMovingSlow()
{
    if (m_ammoElapsed.type1 == 0)
        PlayHUDMotion("anm_idle_moving_slow_empty", TRUE, NULL, GetState());
    else
        PlayHUDMotion("anm_idle_moving_slow", TRUE, NULL, GetState());
}

void CWeaponPistol::PlayAnimAim()
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
                if (HudAnimationExist("anim_idle_aim_moving_empty"))
                    PlayHUDMotion("anim_idle_aim_moving_empty", TRUE, NULL, GetState());
                else
                    PlayHUDMotion("anim_idle_aim_empty", TRUE, NULL, GetState());
            }
            else
                PlayHUDMotion("anim_idle_aim_empty", TRUE, NULL, GetState());
        }
    }
    else
        inherited::PlayAnimAim();
}

void CWeaponPistol::PlayAnimReload()
{
    inherited::PlayAnimReload(); //AVO: refactored to use grand-parent (CWeaponMagazined) function
}

void CWeaponPistol::PlayAnimHide()
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

void CWeaponPistol::PlayAnimShoot()
{
    VERIFY(GetState() == eFire);
    if (m_ammoElapsed.type1 > 1)
    {
        PlayHUDMotion("anm_shots", FALSE, this, GetState());
    }
    else
    {
        PlayHUDMotion("anm_shot_l", FALSE, this, GetState());
    }
}

void CWeaponPistol::switch2_Reload()
{
    inherited::switch2_Reload();
}

void CWeaponPistol::OnAnimationEnd(u32 state)
{
    inherited::OnAnimationEnd(state);
}

void CWeaponPistol::OnShot()
{
	inherited::OnShot(); //Alundaio: not changed from inherited, so instead of copying changes from weaponmagazined, we just do this
}

void CWeaponPistol::UpdateSounds()
{
    inherited::UpdateSounds();
    m_sounds.SetPosition("sndClose", get_LastFP());
}