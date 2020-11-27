#include "stdafx.h"
#include "WeaponBinoculars.h"

#include "xr_level_controller.h"

#include "level.h"
#include "ui\UIFrameWindow.h"
#include "WeaponBinocularsVision.h"
#include "object_broker.h"
#include "inventory.h"

CWeaponBinoculars::CWeaponBinoculars()
{
	m_binoc_vision	= NULL;
	m_bVision		= false;
}

CWeaponBinoculars::~CWeaponBinoculars()
{
	xr_delete				(m_binoc_vision);
}

void CWeaponBinoculars::Load	(LPCSTR section)
{
	inherited::Load(section);

	// Sounds
	m_sounds.LoadSound(section, "snd_zoomin",  "sndZoomIn",		false, SOUND_TYPE_ITEM_USING);
	m_sounds.LoadSound(section, "snd_zoomout", "sndZoomOut",	false, SOUND_TYPE_ITEM_USING);
	m_bVision = !!pSettings->r_bool(section,"vision_present");

	dynamics_zoom = READ_IF_EXISTS(pSettings, r_bool, section, "zoom_enabled_binoc", true);
}

bool CWeaponBinoculars::Action(u16 cmd, u32 flags) 
{
	switch(cmd) 
	{
	case kWPN_FIRE : 
		return inherited::Action(kWPN_ZOOM, flags);
	}

	return inherited::Action(cmd, flags);
}

void CWeaponBinoculars::OnZoomIn		()
{
	if(H_Parent() && !IsZoomed())
	{
		m_sounds.StopSound("sndZoomOut");
		bool b_hud_mode = (Level().CurrentEntity() == H_Parent());
		m_sounds.PlaySound("sndZoomIn", H_Parent()->Position(), H_Parent(), b_hud_mode);
		if(m_bVision && !m_binoc_vision) 
		{
			//.VERIFY			(!m_binoc_vision);
			m_binoc_vision	= xr_new<CBinocularsVision>(cNameSect());
		}
	}
	inherited::OnZoomIn		();
}

void CWeaponBinoculars::OnZoomOut		()
{
	if(H_Parent() && IsZoomed() && !IsRotatingToZoom())
	{
		m_sounds.StopSound("sndZoomIn");
		bool b_hud_mode = (Level().CurrentEntity() == H_Parent());	
		m_sounds.PlaySound("sndZoomOut", H_Parent()->Position(), H_Parent(), b_hud_mode);
		VERIFY			(m_binoc_vision);
		xr_delete		(m_binoc_vision);
	}


	inherited::OnZoomOut();
}

BOOL CWeaponBinoculars::net_Spawn(CSE_Abstract* DC)
{
	inherited::net_Spawn	(DC);
	return					TRUE;
}

void	CWeaponBinoculars::net_Destroy()
{
	inherited::net_Destroy();
	xr_delete(m_binoc_vision);
}

void	CWeaponBinoculars::UpdateCL()
{
	inherited::UpdateCL();
	//manage visible entities here...
	if(H_Parent() && IsZoomed() && !IsRotatingToZoom() && m_binoc_vision)
		m_binoc_vision->Update();
}

bool CWeaponBinoculars::render_item_ui_query()
{
	bool b_is_active_item = m_pInventory->ActiveItem()==this;
	return b_is_active_item && H_Parent() && IsZoomed() && !IsRotatingToZoom() && m_binoc_vision;
}

void CWeaponBinoculars::render_item_ui()
{
	m_binoc_vision->Draw();
	inherited::render_item_ui	();
}

void CWeaponBinoculars::ZoomInc()
{
	if (dynamics_zoom == false)
		return;

	float delta,min_zoom_factor;
	GetZoomData(m_zoom_params.m_fScopeZoomFactor, delta, min_zoom_factor);

	float f					= GetZoomFactor()-delta;
	clamp					(f,m_zoom_params.m_fScopeZoomFactor,min_zoom_factor);
	SetZoomFactor			( f );

	if (f == m_zoom_params.m_fScopeZoomFactor)
		return;
	else
		snd_zoom_inc();
}

void CWeaponBinoculars::ZoomDec()
{
	if (dynamics_zoom == false)
		return;

	float delta,min_zoom_factor;
	GetZoomData(m_zoom_params.m_fScopeZoomFactor,delta,min_zoom_factor);

	float f					= GetZoomFactor()+delta;
	clamp					(f,m_zoom_params.m_fScopeZoomFactor,min_zoom_factor);
	SetZoomFactor			( f );

	if (f == min_zoom_factor)
		return;
	else
		snd_zoom_dec();
}

void CWeaponBinoculars::save(NET_Packet &output_packet)
{
	inherited::save(output_packet);
	save_data		(m_fRTZoomFactor,output_packet);
}

void CWeaponBinoculars::load(IReader &input_packet)
{
	inherited::load(input_packet);
	load_data		(m_fRTZoomFactor,input_packet);
}

bool CWeaponBinoculars::GetBriefInfo( II_BriefInfo& info )
{
	info.clear();
	info.name._set( m_nameShort );
	info.icon._set( cNameSect() );
	return true;
}

void CWeaponBinoculars::net_Relcase	(CObject *object)
{
	if (!m_binoc_vision)
		return;

	m_binoc_vision->remove_links	(object);
}

bool CWeaponBinoculars::can_kill	() const
{
	return			(false);
}

void CWeaponBinoculars::GetBriefInfo_ammo(xr_string& str_name, xr_string& icon_sect_name, xr_string& str_count, string16& fire_mode)
{
	str_name = NameShort();
	str_count = "";
	icon_sect_name = *cNameSect();
}