#include "stdafx.h"
#pragma hdrstop

#include "IGame_Persistent.h"

#include "environment.h"
# include "x_ray.h"
# include "IGame_Level.h"
# include "XR_IOConsole.h"
# include "Render.h"
# include "ps_instance.h"
# include "CustomHUD.h"
# include "Discord.h"

#ifdef INGAME_EDITOR
# include "editor_environment_manager.hpp"
#endif // INGAME_EDITOR

ENGINE_API IGame_Persistent* g_pGamePersistent = NULL;

bool IsMainMenuActive() { return  g_pGamePersistent && g_pGamePersistent->m_pMainMenu && g_pGamePersistent->m_pMainMenu->IsActive(); }

IGame_Persistent::IGame_Persistent()
{
    RDEVICE.seqAppStart.Add(this);
    RDEVICE.seqAppEnd.Add(this);
    RDEVICE.seqFrame.Add(this, REG_PRIORITY_HIGH + 1);
    RDEVICE.seqAppActivate.Add(this);
    RDEVICE.seqAppDeactivate.Add(this);

    m_pMainMenu = NULL;
	
	m_pGShaderConstants = xr_new<ShadersExternalData>(); //--#SM+#--
    m_DataExport        = xr_new<DATARENDER::ExteranData>();

#ifndef INGAME_EDITOR
    pEnvironment = xr_new<CEnvironment>();
#else // #ifdef INGAME_EDITOR
    if (RDEVICE.editor())
        pEnvironment = xr_new<editor::environment::manager>();
    else
        pEnvironment = xr_new<CEnvironment>();
#endif // #ifdef INGAME_EDITOR
}

IGame_Persistent::~IGame_Persistent()
{
    RDEVICE.seqFrame.Remove(this);
    RDEVICE.seqAppStart.Remove(this);
    RDEVICE.seqAppEnd.Remove(this);
    RDEVICE.seqAppActivate.Remove(this);
    RDEVICE.seqAppDeactivate.Remove(this);
    xr_delete(pEnvironment);
	xr_delete(m_pGShaderConstants); //--#SM+#--
}

void IGame_Persistent::OnAppActivate()
{
}

void IGame_Persistent::OnAppDeactivate()
{
}

void IGame_Persistent::OnAppStart()
{
    Environment().load();
}

void IGame_Persistent::OnAppEnd()
{
    Environment().unload();
    OnGameEnd();
    DEL_INSTANCE(g_hud);
}

void IGame_Persistent::PreStart(LPCSTR op)
{
    string256 prev_type;
    params new_game_params;
    xr_strcpy(prev_type, m_game_params.m_game_type);
    new_game_params.parse_cmd_line(op);

    // change game type
    if (0 != xr_strcmp(prev_type, new_game_params.m_game_type))
    {
        OnGameEnd();
    }
}
void IGame_Persistent::Start(LPCSTR op)
{
    string256 prev_type;
    xr_strcpy(prev_type, m_game_params.m_game_type);
    m_game_params.parse_cmd_line(op);
    // change game type
    if ((0 != xr_strcmp(prev_type, m_game_params.m_game_type)))
    {
        if (*m_game_params.m_game_type)
            OnGameStart();
#ifndef _EDITOR
        if (g_hud)
            DEL_INSTANCE(g_hud);
#endif
    }
    else UpdateGameType();

    VERIFY(ps_destroy.empty());
}

void IGame_Persistent::Disconnect()
{
    // clear "need to play" particles
    destroy_particles(true);

    if (g_hud)
        DEL_INSTANCE(g_hud);

    // Kill object - save memory
    ObjectPool.clear();
    EnvCryRay.Render->models_Clear(TRUE);
}

extern BOOL xrengint_noprefetch;
void IGame_Persistent::OnGameStart()
{
    LoadTitle();
    if (xrengint_noprefetch)
        Prefetch();
}

void IGame_Persistent::Prefetch()
{
    // prefetch game objects & models
    float p_time = 1000.f*Device.GetTimerGlobal()->GetElapsed_sec();
	size_t mem_0 = Memory.mem_usage();

    Log("- Loading objects...");
    ObjectPool.prefetch();
    Log("- Loading models...");
    EnvCryRay.Render->models_Prefetch();

    Device.m_pRender->ResourcesDeferredUpload();

    p_time = 1000.f*Device.GetTimerGlobal()->GetElapsed_sec() - p_time;
	size_t p_mem = Memory.mem_usage() - mem_0;

    Msg("# [prefetch] time:   %d ms", iFloor(p_time));
    Msg("# [prefetch] memory: %lldKb", p_mem / 1024);
}

void IGame_Persistent::OnGameEnd()
{
    ObjectPool.clear();
    EnvCryRay.Render->models_Clear(TRUE);
}

void IGame_Persistent::OnFrame()
{
    if (!Device.Paused() || Device.dwPrecacheFrame)
        Environment().OnFrame();


    Device.Statistic->Particles_starting = ps_needtoplay.size();
    Device.Statistic->Particles_active = ps_active.size();
    Device.Statistic->Particles_destroy = ps_destroy.size();

    // Play req particle systems
    while (ps_needtoplay.size())
    {
        CPS_Instance* psi = ps_needtoplay.back();
        ps_needtoplay.pop_back();
        psi->Play(false);
    }
    // Destroy inactive particle systems
    while (ps_destroy.size())
    {
        // u32 cnt = ps_destroy.size();
        CPS_Instance* psi = ps_destroy.back();
        VERIFY(psi);
        if (psi->Locked())
        {
            Log("--locked");
            break;
        }
        ps_destroy.pop_back();
        psi->PSI_internal_delete();
    }
}

void IGame_Persistent::destroy_particles(const bool& all_particles)
{
    ps_needtoplay.clear();

    xr_set<CPS_Instance*>::iterator I = ps_active.begin();
    xr_set<CPS_Instance*>::iterator E = ps_active.end();
    for (; I != E; ++I)
    {
        if (all_particles || (*I)->destroy_on_game_load())
            (*I)->PSI_destroy();
    }

    while (ps_destroy.size())
    {
        CPS_Instance* psi = ps_destroy.back();
        VERIFY(psi);
        VERIFY(!psi->Locked());
        ps_destroy.pop_back();
        psi->PSI_internal_delete();
    }

    VERIFY(ps_needtoplay.empty() && ps_destroy.empty() && (!all_particles || ps_active.empty()));
}

void IGame_Persistent::OnAssetsChanged()
{
    Device.m_pRender->OnAssetsChanged();
}
